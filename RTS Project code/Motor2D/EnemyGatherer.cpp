#include "Brofiler/Brofiler.h"

#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Collisions.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
#include "GuiManager.h"
#include "UI.h"
#include "UI_Healthbar.h"
#include "EntityManager.h"
#include "SceneManager.h"

#include "EnemyGatherer.h"


EnemyGatherer::EnemyGatherer(int x, int y, ENTITY_TYPE type, int level) : Dynamic_Object(x, y, type, level)
{
	InitEntity();
}

EnemyGatherer::~EnemyGatherer()
{

}

bool EnemyGatherer::Awake(pugi::xml_node&)
{
	return true;
}

bool EnemyGatherer::Start()
{
	return true;
}

bool EnemyGatherer::PreUpdate()
{
	return true;
}

bool EnemyGatherer::Update(float dt, bool doLogic)
{
	BROFILER_CATEGORY("EnemyGatherer Update", Profiler::Color::Black);

	HandleMovement(dt);
	//HandleFx();
	DataMapSafetyCheck();

	UpdateUnitSpriteSection();

	selection_collider.x = (int)pixel_position.x + 10;
	selection_collider.y = (int)pixel_position.y + 10;

	if (doLogic)
	{
		if (target == nullptr && !path_full)
		{
			//SetGatheringTarget(App->player->mouse_tile);
			SetGatheringTarget(App->player->cursor_tile);
		}
	}

	if (target != nullptr)
	{
		//path_full = false;
		if (target != nullptr)
		{
			if (TargetIsInRange())
			{
				GatherResource();
			}
		}
	}

	center_point = fPoint(pixel_position.x, pixel_position.y + App->map->data.tile_height * 0.5f);

	return true;
}

bool EnemyGatherer::PostUpdate()
{
	if (current_health <= 0)
	{
		App->entity_manager->DeleteEntity(this);
	}

	return true;
}

bool EnemyGatherer::CleanUp()
{
	App->pathfinding->ChangeWalkability(occupied_tile, this, WALKABLE);		//The entity is cleared from the walkability map.
	App->entity_manager->ChangeEntityMap(tile_position, this, true);		//The entity is cleared from the entity_map.

	entity_sprite = nullptr;

	if (collider != nullptr)
	{
		collider->to_delete = true;
	}

	App->gui_manager->DeleteGuiElement(healthbar);

	return true;
}

void EnemyGatherer::Draw()
{
	App->render->Blit(this->entity_sprite, (int)pixel_position.x, (int)pixel_position.y - 14, &entity_sprite_section);

	if (App->player->god_mode)
	{
		App->render->DrawQuad(selection_collider, 255, 255, 0, 100);
	}
}

void EnemyGatherer::InitEntity()
{
	entity_sprite = App->entity_manager->GetEnemyGathererTexture();

	InitUnitSpriteSections();

	is_selectable = false;
	is_selected = false;
	path_full = false;

	target = nullptr;
	gather_in_cooldown = false;
	accumulated_cooldown = 0.0f;

	speed = 400.0f;

	max_health = 150;
	current_health = max_health;

	gathering_speed = 1.0f;
	gathering_amount_data = 30;
	gathering_amount_electricity = 15;

	attack_range = 1;

	if (App->entity_manager->CheckTileAvailability(tile_position, this))
	{
		AttachHealthbarToEntity();
	}

	center_point = fPoint(pixel_position.x, pixel_position.y + App->map->data.tile_height * 0.5f);
}

void EnemyGatherer::AttachHealthbarToEntity()
{
	healthbar_position_offset.x = -6;										//Magic
	healthbar_position_offset.y = -6;

	healthbar_background_rect = { 967, 1, MAX_UNIT_HEALTHBAR_WIDTH, 6 };
	healthbar_rect = { 967, 7, MAX_UNIT_HEALTHBAR_WIDTH, 6 };

	int healthbar_position_x = (int)pixel_position.x + healthbar_position_offset.x;					// X and Y position of the healthbar's hitbox.
	int healthbar_position_y = (int)pixel_position.y + healthbar_position_offset.y;					// The healthbar's position is already calculated in UI_Healthbar.

	healthbar = (UI_Healthbar*)App->gui_manager->CreateHealthbar(UI_ELEMENT::HEALTHBAR, healthbar_position_x, healthbar_position_y, true, &healthbar_rect, &healthbar_background_rect, this);
}

void EnemyGatherer::InitUnitSpriteSections()
{
	entity_sprite_section = { 52, 0, 52, 49 };

	pathing_up_section = { 0, 49, 39, 42 };
	pathing_down_section = { 39, 49, 38, 45 };
	pathing_rigth_section = { 123, 49, 43, 42 };
	pathing_left_section = { 78, 49, 43, 42 };
	pathing_up_right_section = { 104, 0, 52, 49 };
	pathing_up_left_section = { 156, 0, 52, 49 };
	pathing_down_right_section = { 52, 0, 52 ,49 };
	pathing_down_left_section = { 0, 0, 52, 49 };
}

void EnemyGatherer::UpdateUnitSpriteSection()
{
	//change section according to pathing. 
	switch (unit_state)
	{
	case ENTITY_STATE::PATHING_UP:
		entity_sprite_section = pathing_up_section;
		break;
	case ENTITY_STATE::PATHING_DOWN:
		entity_sprite_section = pathing_down_section;
		break;
	case ENTITY_STATE::PATHING_RIGHT:
		entity_sprite_section = pathing_rigth_section;
		break;
	case ENTITY_STATE::PATHING_LEFT:
		entity_sprite_section = pathing_left_section;
		break;
	case ENTITY_STATE::PATHING_UP_RIGHT:
		entity_sprite_section = pathing_up_right_section;
		break;
	case ENTITY_STATE::PATHING_UP_LEFT:
		entity_sprite_section = pathing_up_left_section;
		break;
	case ENTITY_STATE::PATHING_DOWN_RIGHT:
		entity_sprite_section = pathing_down_right_section;
		break;
	case ENTITY_STATE::PATHING_DOWN_LEFT:
		entity_sprite_section = pathing_down_left_section;
		break;
	}
}

bool EnemyGatherer::TargetIsInRange()
{
	if (target != nullptr)
	{
		float distance = tile_position.DistanceNoSqrt(target->tile_position) * 0.1f;

		if (distance <= attack_range)
		{
			return true;
		}
	}

	return false;
}


void EnemyGatherer::SetGatheringTarget(const iPoint& tile_position)
{
	std::vector<Entity*>::iterator item = App->entity_manager->entities.begin();

	for (; item != App->entity_manager->entities.end(); ++item)
	{
		if (App->entity_manager->IsResource((*item)))
		{
			if (App->entity_manager->GetEntityPos(*item) == tile_position)
			{
				target = (*item);
				break;
			}
		}
	}
}

void EnemyGatherer::PathToGatheringTarget()
{
	std::vector<iPoint> tmp;

	for (int i = 0; i < (int)entity_path.size(); ++i)
	{
		tmp.push_back(entity_path[i]);

		if ((entity_path[i].DistanceNoSqrt(target->tile_position) * 0.1f) <= attack_range)
		{
			entity_path.clear();

			entity_path = tmp;

			target_tile = entity_path.back();
			current_path_tile = entity_path.begin();

			tmp.clear();

			break;
		}
	}
}


void EnemyGatherer::GatherResource()
{
	if (!gather_in_cooldown)
	{
		if (App->entity_manager->IsResource(target))
		{
			ApplyDamage(target);
			App->audio->PlayFx(App->entity_manager->gather_fx);
			gather_in_cooldown = true;

			if (target->type == ENTITY_TYPE::ROCK)
			{
				App->entity_manager->resource_data += gathering_amount_data;
				LOG("Data gathered: %d", App->entity_manager->resource_data);
			}
			else if (target->type == ENTITY_TYPE::TREE)
			{
				App->entity_manager->resource_electricity += gathering_amount_electricity;
				LOG("Electricity gathered: %d", App->entity_manager->resource_electricity);
			}
		}
		if (target->current_health <= 0)
		{
			target = nullptr;
		}
	}
	else
	{
		accumulated_cooldown += App->GetDt();

		if (accumulated_cooldown >= gathering_speed)
		{
			gather_in_cooldown = false;
			accumulated_cooldown = 0.0f;
		}
	}
}

void EnemyGatherer::OnCollision(Collider* C1, Collider* C2)
{

}
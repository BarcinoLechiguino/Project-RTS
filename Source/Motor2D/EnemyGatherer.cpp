#include "Dependencies/Brofiler/Brofiler.h"

#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
#include "GuiManager.h"
#include "GuiElement.h"
#include "GuiHealthbar.h"
#include "SceneManager.h"
#include "FowManager.h"
#include "EnemyAIManager.h"
#include "EntityManager.h"

#include "EnemyGatherer.h"


EnemyGatherer::EnemyGatherer(int x, int y, ENTITY_TYPE type, int level) : EnemyUnit(x, y, type, level)
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

bool EnemyGatherer::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("EnemyGatherer Update", Profiler::Color::Black);

	HandleMovement(dt);

	DataMapSafetyCheck();

	UpdateUnitSpriteSection();

	UpdateRedState();

	if (do_logic)
	{
		if (target == nullptr && entity_path.empty())
		{
			SetGatheringTarget(App->player->cursor_tile);
		}
	}

	if (target != nullptr)
	{
		if (TargetIsInRange())
		{
			GatherResource();
		}
	}

	center_point = fPoint(pixel_position.x + App->map->data.tile_width * 0.5f, pixel_position.y + App->map->data.tile_height * 0.5f);

	// FOG OF WAR
	is_visible = fow_entity->is_visible;

	fow_entity->SetPos(tile_position);

	return true;
}

bool EnemyGatherer::PostUpdate()
{
	if (current_health <= 0)
	{
		App->entity_manager->kill_count++;
		App->entity_manager->DeleteEntity(this);
	}

	return true;
}

bool EnemyGatherer::CleanUp()
{
	App->pathfinding->ChangeWalkability(occupied_tile, this, WALKABLE);		//The entity is cleared from the walkability map.
	App->entity_manager->ChangeEntityMap(tile_position, this, true);		//The entity is cleared from the entity_map.

	entity_sprite = nullptr;

	if (is_selected)
	{
		App->player->DeleteEntityFromBuffers(this);
	}

	App->gui_manager->DeleteGuiElement(healthbar);

	App->fow_manager->DeleteFowEntity(fow_entity);

	App->enemy_AI_manager->DeleteEnemyAIEntity(enemy_AI_entity);

	return true;
}

void EnemyGatherer::Draw()
{

	if (this->red_state == false) {
		App->render->Blit(this->entity_sprite, (int)pixel_position.x, (int)pixel_position.y - 14, &entity_sprite_section);
	}
	if (this->red_state == true) {
		App->render->Blit(this->entity_sprite, (int)pixel_position.x, (int)pixel_position.y - 14, &entity_sprite_section, false, 1.0F, 1.0F, 0.0, 0, 0, App->render->renderer, { 255, 128, 128, 255 });
	}
}

void EnemyGatherer::InitEntity()
{
	// POSITION VARIABLES
	center_point = fPoint(pixel_position.x, pixel_position.y + App->map->data.tile_height * 0.5f);
	
	// TEXTURE & SECTIONS
	entity_sprite = App->entity_manager->GetEnemyGathererTexture();
	InitUnitSpriteSections();

	// FLAGS
	is_selectable = false;
	is_selected = false;
	path_full = false;

	target = nullptr;
	gather_in_cooldown = false;
	accumulated_cooldown = 0.0f;

	// STATS
	speed = 255.0f;

	max_health = 150;
	current_health = max_health;

	gathering_speed = 1.0f;
	gathering_amount_data = 30;
	gathering_amount_electricity = 15;

	attack_range = 1;

	// HEALTHBAR
	if (App->entity_manager->CheckTileAvailability(tile_position, this))
	{
		AttachHealthbarToEntity();
	}

	// FOG OF WAR
	is_visible = false;
	is_neutral = false;
	provides_visibility = false;

	fow_entity = App->fow_manager->CreateFowEntity(tile_position, is_neutral, provides_visibility);

	// ENEMY AI
	enemy_AI_entity = App->enemy_AI_manager->CreateEnemyAIEntity(this);
}

void EnemyGatherer::AttachHealthbarToEntity()
{
	healthbar_position_offset.x = -6;										//Magic
	healthbar_position_offset.y = -6;

	healthbar_background_rect = { 967, 1, MAX_UNIT_HEALTHBAR_WIDTH, 6 };
	healthbar_rect = { 967, 7, MAX_UNIT_HEALTHBAR_WIDTH, 6 };

	int healthbar_position_x = (int)pixel_position.x + healthbar_position_offset.x;					// X and Y position of the healthbar's hitbox.
	int healthbar_position_y = (int)pixel_position.y + healthbar_position_offset.y;					// The healthbar's position is already calculated in GuiHealthbar.

	healthbar = (GuiHealthbar*)App->gui_manager->CreateHealthbar(GUI_ELEMENT_TYPE::HEALTHBAR, healthbar_position_x, healthbar_position_y, true, &healthbar_rect, &healthbar_background_rect, this);
}

void EnemyGatherer::InitUnitSpriteSections()
{
	/*entity_sprite_section = { 52, 0, 52, 49 };

	pathing_up_section = { 0, 49, 39, 42 };
	pathing_down_section = { 39, 49, 38, 45 };
	pathing_rigth_section = { 123, 49, 43, 42 };
	pathing_left_section = { 78, 49, 43, 42 };
	pathing_up_right_section = { 104, 0, 52, 49 };
	pathing_up_left_section = { 156, 0, 52, 49 };
	pathing_down_right_section = { 52, 0, 52 ,49 };
	pathing_down_left_section = { 0, 0, 52, 49 };*/

	//	 --- LOADING FROM XML ---
	pugi::xml_node sections = App->entities_file.child("entities").child("units").child("enemies").child("enemy_gatherer").child("sprite_sections");

	pathing_up_section.x = sections.child("pathing_up").attribute("x").as_int();
	pathing_up_section.y = sections.child("pathing_up").attribute("y").as_int();
	pathing_up_section.w = sections.child("pathing_up").attribute("w").as_int();
	pathing_up_section.h = sections.child("pathing_up").attribute("h").as_int();

	pathing_down_section.x = sections.child("pathing_down").attribute("x").as_int();
	pathing_down_section.y = sections.child("pathing_down").attribute("y").as_int();
	pathing_down_section.w = sections.child("pathing_down").attribute("w").as_int();
	pathing_down_section.h = sections.child("pathing_down").attribute("h").as_int();

	pathing_rigth_section.x = sections.child("pathing_right").attribute("x").as_int();
	pathing_rigth_section.y = sections.child("pathing_right").attribute("y").as_int();
	pathing_rigth_section.w = sections.child("pathing_right").attribute("w").as_int();
	pathing_rigth_section.h = sections.child("pathing_right").attribute("h").as_int();

	pathing_left_section.x = sections.child("pathing_left").attribute("x").as_int();
	pathing_left_section.y = sections.child("pathing_left").attribute("y").as_int();
	pathing_left_section.w = sections.child("pathing_left").attribute("w").as_int();
	pathing_left_section.h = sections.child("pathing_left").attribute("h").as_int();

	pathing_up_right_section.x = sections.child("pathing_up_right").attribute("x").as_int();
	pathing_up_right_section.y = sections.child("pathing_up_right").attribute("y").as_int();
	pathing_up_right_section.w = sections.child("pathing_up_right").attribute("w").as_int();
	pathing_up_right_section.h = sections.child("pathing_up_right").attribute("h").as_int();

	pathing_up_left_section.x = sections.child("pathing_up_left").attribute("x").as_int();
	pathing_up_left_section.y = sections.child("pathing_up_left").attribute("y").as_int();
	pathing_up_left_section.w = sections.child("pathing_up_left").attribute("w").as_int();
	pathing_up_left_section.h = sections.child("pathing_up_left").attribute("h").as_int();

	pathing_down_right_section.x = sections.child("pathing_down_right").attribute("x").as_int();
	pathing_down_right_section.y = sections.child("pathing_down_right").attribute("y").as_int();
	pathing_down_right_section.w = sections.child("pathing_down_right").attribute("w").as_int();
	pathing_down_right_section.h = sections.child("pathing_down_right").attribute("h").as_int();

	pathing_down_left_section.x = sections.child("pathing_down_left").attribute("x").as_int();
	pathing_down_left_section.y = sections.child("pathing_down_left").attribute("y").as_int();
	pathing_down_left_section.w = sections.child("pathing_down_left").attribute("w").as_int();
	pathing_down_left_section.h = sections.child("pathing_down_left").attribute("h").as_int();

	//Default section
	entity_sprite_section = pathing_down_right_section;
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
	if (target->current_health > 0)
	{
		if (!gather_in_cooldown)
		{
			if (App->entity_manager->IsResource(target))
			{
				ApplyDamage(target);
				App->audio->PlayFx(App->entity_manager->gatherer_gathering_fx);
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
				else if (target->type == ENTITY_TYPE::OBELISK)
				{
					App->entity_manager->resource_bits += gathering_amount_bits;
					LOG("Electricity gathered: %d", App->entity_manager->resource_bits);
				}
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
	else
	{
		target = nullptr;

		gather_in_cooldown = false;										// Reseting the attack for the next time the unit has a target.
		accumulated_cooldown = 0.0f;

		return;
	}
}

Entity* EnemyGatherer::GetTarget()
{
	return target;
}

int  EnemyGatherer::GetAttackRange()
{
	return attack_range;
}
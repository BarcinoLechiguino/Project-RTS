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
#include "GuiCreationBar.h"
#include "FowManager.h"
#include "EnemyAIManager.h"
#include "EntityManager.h"
#include "Gatherer.h"
#include "Scout.h"

#include "TownHall.h"


TownHall::TownHall(int x, int y, ENTITY_TYPE type, int level) : Building(x, y, type, level)
{
	InitEntity();	
}

TownHall::~TownHall()
{

}

bool TownHall::Awake(pugi::xml_node&)
{
	return true;
}

bool TownHall::Start()
{
	App->pathfinding->ChangeWalkability(tile_position, this, NON_WALKABLE);

	return true;
}

bool TownHall::PreUpdate()
{
	return true;
}

bool TownHall::Update(float dt, bool do_logic)
{
	UpdateRedState();
	
	if (building_state == BUILDING_STATE::CONSTRUCTING)
	{
		ConstructBuilding();
	}

	if (construction_finished)
	{
		if (creation_queue.size() != 0)
		{
			if (!creating_unit)
			{
				StartUnitCreation();
			}
			else
			{
				creation_bar->UpdateCreationBarValue();

				if (creation_bar->creation_finished)
				{
					GenerateUnitByType(created_unit_type);

					created_unit_type = ENTITY_TYPE::UNKNOWN;

					creation_queue.erase(creation_queue.begin());

					creating_unit = false;
				}
			}
		}
		else
		{
			creation_bar->is_visible = false;
		}
	}

	// FOG OF WAR
	is_visible = fow_entity->is_visible;									// No fow_entity->SetPos(tile_position) as, obviously, a StaticObject entity will never move.
	
	return true;
}

bool TownHall::PostUpdate()
{
	if (current_health <= 0)
	{
		App->entity_manager->DeleteEntity(this);
	}

	return true;
}

bool TownHall::CleanUp()
{
	App->pathfinding->ChangeWalkability(tile_position, this, WALKABLE);		//The entity is cleared from the walkability map.
	App->entity_manager->ChangeEntityMap(tile_position, this, true);		//The entity is cleared from the entity_map.

	entity_sprite = nullptr;

	if (is_selected)
	{
		App->player->DeleteEntityFromBuffers(this);
	}

	App->gui_manager->DeleteGuiElement(healthbar);
	App->gui_manager->DeleteGuiElement(creation_bar);

	App->fow_manager->DeleteFowEntity(fow_entity);

	App->enemy_AI_manager->TownhallReassignment();

	return true;
}

void TownHall::Draw()
{
	if (is_selected) 
	{
		App->render->Blit(App->entity_manager->select_townhall_tex, (int)pixel_position.x - 69, (int)pixel_position.y - 20);
	}

	if (construction_finished)
	{
		if (this->red_state == false)
		{
			App->render->Blit(entity_sprite, (int)pixel_position.x - 51, (int)pixel_position.y - 20, &hall_rect); //Magic
		}

		if (this->red_state == true)
		{
			App->render->Blit(entity_sprite, (int)pixel_position.x - 51, (int)pixel_position.y - 20, &hall_rect, false, 1.0F, 1.0F, 0.0, 0, 0, App->render->renderer, { 255, 192, 192, 255 });
		}
	}
}

void TownHall::StartUnitCreation()
{
	creation_bar->is_visible = true;

	creating_unit = true;

	created_unit_type = (*creation_queue.begin());

	switch (created_unit_type)															// Used a switch taking into account scalability.
	{
	case ENTITY_TYPE::GATHERER:
		creation_bar->SetNewCreationTime(gatherer_creation_time);
		break;

	case ENTITY_TYPE::SCOUT:
		creation_bar->SetNewCreationTime(scout_creation_time);
		break;
	}
}

void TownHall::GenerateUnit(ENTITY_TYPE type, int level)
{
	iPoint pos = App->pathfinding->FindNearbyPoint(iPoint(tile_position.x, tile_position.y + 2));

	switch (type)
	{
	case ENTITY_TYPE::GATHERER:
		(Gatherer*)App->entity_manager->CreateEntity(ENTITY_TYPE::GATHERER, pos.x, pos.y, level);
		break;

	case ENTITY_TYPE::SCOUT:
		(Scout*)App->entity_manager->CreateEntity(ENTITY_TYPE::SCOUT, pos.x, pos.y, level);
		break;
	}
}

void TownHall::GenerateUnitByType(ENTITY_TYPE type)
{
	iPoint pos = App->pathfinding->FindNearbyPoint(iPoint(tile_position.x, tile_position.y + 2));

	switch (type)
	{
	case ENTITY_TYPE::GATHERER:
		(Gatherer*)App->entity_manager->CreateEntity(ENTITY_TYPE::GATHERER, pos.x, pos.y, gatherer_level);
		break;

	case ENTITY_TYPE::SCOUT:
		(Scout*)App->entity_manager->CreateEntity(ENTITY_TYPE::SCOUT, pos.x, pos.y, scout_level);
		break;
	}
}

void TownHall::LevelChanges()						//Updates the building stats when leveling up
{
	switch (level)
	{
	case 1:
		hall_rect = hall_rect_1;
		break;
	case 2:
		hall_rect = hall_rect_2;
		max_health = 1200;
		current_health = max_health;
		break;
	default:
		hall_rect = hall_rect_2;
		break;
	}
}

void TownHall::ConstructBuilding()
{
	if (!constructing_building)
	{
		creation_bar->is_visible = true;
		creation_bar->SetNewCreationTime(construction_time);

		fow_entity->provides_visibility = false;

		constructing_building = true;
	}
	else
	{
		creation_bar->UpdateCreationBarValue();

		for (int y = 0; y != tiles_occupied.y; ++y)
		{
			for (int x = 0; x != tiles_occupied.x; ++x)
			{
				int pos_y = tile_position.y + y;
				int pos_x = tile_position.x + x;

				iPoint draw_position = App->map->MapToWorld(pos_x, pos_y);

				if (App->player->buildable_tile_tex != nullptr)
				{
					App->render->Blit(App->player->buildable_tile_tex, draw_position.x + 2, draw_position.y + 2, nullptr);
				}
			}
		}

		/*if (App->player->townhall_build_tex != nullptr)
		{
			SDL_SetTextureAlphaMod(App->player->townhall_build_tex, (Uint8)(current_rate * 255.0f));
		}*/

		if (creation_bar->creation_finished)
		{	
			fow_entity->provides_visibility = true;
			
			building_state = BUILDING_STATE::IDLE;
			construction_finished = true;
			constructing_building = false;
		}
	}
}

void TownHall::InitEntity()
{
	// POSITION & SIZE
	iPoint world_position = App->map->MapToWorld(tile_position.x, tile_position.y);

	pixel_position.x = (float)world_position.x;
	pixel_position.y = (float)world_position.y;

	center_point = fPoint(pixel_position.x, pixel_position.y + App->map->data.tile_height + App->map->data.tile_height * 0.5f);

	tiles_occupied.x = 3;
	tiles_occupied.y = 3;
	
	// TEXTURE & SECTIONS
	entity_sprite = App->entity_manager->GetTownHallTexture();
	hall_rect_1 = { 0, 0, 155, 138 };
	hall_rect_2 = { 155, 0, 155, 138 };
	hall_rect = hall_rect_1;

	// FLAGS
	is_selected = false;

	// BUILDING CONSTRUCTION VARIABLES
	construction_time = 1.0f;

	// UNIT CREATION VARIABLES
	creating_unit = false;
	created_unit_type = ENTITY_TYPE::UNKNOWN;
	
	gatherer_creation_time = 1.0f;														//Magic
	scout_creation_time = 2.0f;

	// STATS
	max_health = 900;
	current_health = max_health;

	gatherer_level = 1;
	scout_level = 1;

	max_gatherer_level = 2;
	max_scout_level = 2;

	gatherer_data_cost = 100;
	gatherer_electricity_cost = 50;

	scout_data_cost = 100;
	scout_electricity_cost = 50;

	max_level = 2;

	// HEALTHBAR & CREATION BAR
	if (App->entity_manager->CheckTileAvailability(tile_position, this))
	{
		AttachHealthbarToEntity();
		AttachCreationBarToEntity();
	}

	// FOG OF WAR
	is_visible = true;
	is_neutral = false;
	provides_visibility = true;
	range_of_vision = 8;

	fow_entity = App->fow_manager->CreateFowEntity(tile_position + iPoint(1, 1), is_neutral, provides_visibility);

	//fow_entity->frontier = App->fow_manager->CreateRectangularFrontier(range_of_vision, range_of_vision, tile_position + iPoint(1, 1));		// Getting the center of the hall.
	fow_entity->frontier = App->fow_manager->CreateCircularFrontier(range_of_vision, tile_position + iPoint(1, 1));

	fow_entity->line_of_sight = App->fow_manager->GetLineOfSight(fow_entity->frontier);

	// ENEMY AI
	App->enemy_AI_manager->ally_townhall = this;
}

void TownHall::AttachHealthbarToEntity()
{
	// HP Healthbar
	healthbar_position_offset.x = -30;
	healthbar_position_offset.y = -6;

	healthbar_background_rect = { 618, 1, MAX_BUILDING_HEALTHBAR_WIDTH, 9 };
	healthbar_rect = { 618, 23, MAX_BUILDING_HEALTHBAR_WIDTH, 9 };

	int healthbar_position_x = (int)pixel_position.x + healthbar_position_offset.x;					// X and Y position of the healthbar's hitbox.
	int healthbar_position_y = (int)pixel_position.y + healthbar_position_offset.y;					// The healthbar's position is already calculated in GuiHealthbar.

	healthbar = (GuiHealthbar*)App->gui_manager->CreateHealthbar(GUI_ELEMENT_TYPE::HEALTHBAR, healthbar_position_x, healthbar_position_y, true, &healthbar_rect, &healthbar_background_rect, this);

}

void TownHall::AttachCreationBarToEntity()
{
	creation_bar_position_offset.x = -30;															// Magic
	creation_bar_position_offset.y = 6;

	creation_bar_background_rect = { 618, 1, MAX_CREATION_BAR_WIDTH, 9 };
	creation_bar_rect = { 618, 23, MAX_CREATION_BAR_WIDTH, 9 };

	int creation_bar_position_x = (int)pixel_position.x + creation_bar_position_offset.x;
	int creation_bar_position_y = (int)pixel_position.y + creation_bar_position_offset.y;

	creation_bar = (GuiCreationBar*)App->gui_manager->CreateCreationBar(GUI_ELEMENT_TYPE::CREATIONBAR, creation_bar_position_x, creation_bar_position_y, false
		, &creation_bar_rect, &creation_bar_background_rect, this);
}
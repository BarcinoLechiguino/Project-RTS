#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Map.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Gui.h"

Entity::Entity() : tile_position(0, 0), type(ENTITY_TYPE::UNKNOWN_TYPE), entity_sprite(nullptr), collider(nullptr), animation(nullptr)
{

}

Entity::Entity(int x, int y, ENTITY_TYPE type) : tile_position(x, y), type(type), entity_sprite(nullptr), collider(nullptr), animation(nullptr)	//Initializes the variables to the specified values when the constructor is called.
{
	return;
}

//j1Entity::~j1Entity()
//{
//	App->tex->UnLoad(entity_sprite);
//	entity_sprite = nullptr;
//	if (collider != nullptr)
//	{
//		collider->delete_collider = true;
//	}
//
//}

bool Entity::Awake(pugi::xml_node& config)
{
	return true;
}

bool Entity::Start()
{
	return true;
}

bool Entity::PreUpdate()
{
	return true;
}

bool Entity::Update(float dt, bool doLogic)
{
	return true;
}

bool Entity::PostUpdate()
{
	return true;
}

bool Entity::CleanUp()
{
	return true;
}

//-------------------- LOAD AND SAVE --------------------
bool Entity::Load(pugi::xml_node&)
{
	return true;
}

bool Entity::Save(pugi::xml_node&) const
{
	return true;
}

//------------------- ENTITY METHODS -------------------
void Entity::Restart()
{
	return;
}

void Entity::BlitEntity(int x, int y, SDL_Rect entity_rect, bool flip)
{
	//App->render->Blit(entity_sprite, x, y, &entity_rect, flip);

	//App->render->Blit(entity_sprite, x, y, &entity_rect, flip, SDL_FLIP_HORIZONTAL);	//Check out how to use the flip already integrated in the Blit.
}

void Entity::OnCollision(Collider* C1, Collider* C2)
{
	return;
}

void Entity::ApplyDamage(Entity* target)
{
	target->current_health -= damage;
	
	target->healthbar->ModifyHealthbar();

	/*int new_width = (MAX_UNIT_HEALTHBAR_WIDTH * target->current_health) / target->max_health;
	
	target->healthbar_rect.w = new_width;
	
	target->health_bar->SetScreenRect(target->healthbar_rect);*/

	return;
}


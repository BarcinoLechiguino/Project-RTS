#include "Dependencies\Brofiler\Brofiler.h"

#include "Definitions.h"
#include "Log.h"

#include "Application.h"
#include "Window.h"
#include "Render.h"
#include "Map.h"

#include "EntityManager.h"		

#define VSYNC true

Render::Render() : Module()
{
	name = ("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)									//Renderer VSync --> Flags turn on and of the VSync
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	//App->vsyncIsActive = config.child("vsync").attribute("value").as_bool();

	if (config.child("vsync").attribute("value").as_bool(true) == true)	//If vsync is true in the xml file.
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		App->vsync_is_active = true;
	}
	else
	{
		App->vsync_is_active = false;
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = (int)config.child("camera").attribute("x").as_float();
		camera.y = (int)config.child("camera").attribute("y").as_float();

		//cam.camera_debug = config.child("camera_debug").attribute("value").as_bool();				//Loads the cameraLimit bool state from the config xml file.
		//cam.camera_debug_speed = config.child("camera_debug").attribute("speed").as_float();		//Loads the camera's movement speed value while in debug mode  from the config xml file.
		//cam.smoothingSpeed = config.child("camera").attribute("smoothing_speed").as_float();		//Loads the camera smoothing speed value from the config xml file.
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	BROFILER_CATEGORY("Render PreUpdate", Profiler::Color::DeepSkyBlue);
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	BROFILER_CATEGORY("Render Update", Profiler::Color::DeepSkyBlue);
	
	
	return true;
}

bool Render::PostUpdate()
{
	BROFILER_CATEGORY("Render PostUpdate", Profiler::Color::DeepSkyBlue);
	
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& collider)
{
	SDL_RenderSetViewport(renderer, &collider);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = (int)App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Blit to screen
bool Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, bool flip, float speed, float render_scale, double angle, int pivot_x, int pivot_y, SDL_Renderer* renderer, SDL_Color color) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_Rect rect;

	rect.x = (int)(camera.x * speed) + x * (int)scale;
	rect.y = (int)(camera.y * speed) + y * (int)scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= /*(int)*/(scale * render_scale);
	rect.h *= /*(int)*/(scale * render_scale);

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}
	
	SDL_Color original;
	if (color.a != 0) {
		
		SDL_GetTextureColorMod(texture, &original.r, &original.g, &original.b);

		SDL_SetTextureColorMod(texture, color.r, color.g, color.b) == 0;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	if (color.a != 0) {
		SDL_SetTextureColorMod(texture, original.r, original.g, original.b) == 0;
	}


 	return ret;
}

bool Render::ColoredBlit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, const SDL_Rect* rectSize, SDL_Color color, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (rectSize != NULL)
	{
		rect.w = rectSize->w;
		rect.h = rectSize->h;
	}
	else
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	SDL_SetTextureColorMod(texture, color.r, color.g, color.b) == 0;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD) == 0;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawQuad(const SDL_Rect& collider, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = (int)App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(collider);
	if(use_camera)
	{
		rec.x = (int)(camera.x + collider.x * scale);
		rec.y = (int)(camera.y + collider.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = (int)App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = (int)App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void Render::SetCameraPosition(iPoint position)
{
	camera.x = position.x;
	camera.y = position.y;
}
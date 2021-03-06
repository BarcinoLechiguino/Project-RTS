#include "Definitions.h"
#include "Log.h"
#include "Application.h"
#include "Textures.h"
#include "Fonts.h"
#include "AssetManager.h"

#include "Dependencies\SDL\include\SDL.h"
#include "Dependencies\SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "Dependencies/SDL_ttf/libx86/SDL2_ttf.lib" )

Fonts::Fonts() : Module()
{
	name = ("fonts");
}

// Destructor
Fonts::~Fonts()
{}

// Called before render is available
bool Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	return ret;
}

bool Fonts::Start()
{
	default				= Load("fonts/arial.ttf", 12);
	default_bigger		= Load("fonts/arial.ttf", 15);
	main_title			= Load("fonts/Old School Adventures.ttf", 18);
	title_buttons		= Load("fonts/kenvector_future.ttf", 19);
	subtitle			= Load("fonts/Old School Adventures.ttf", 11);
	hud					= Load("fonts/CopperPlate_BoldItalic.ttf", 15);
	titles_other		= Load("fonts/OpenSans-BoldItalic.ttf", 25);
	main_title_buttons	= Load("fonts/kenvector_future.ttf", 30);

	return true;
}

// Called before quitting
bool Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");

	std::vector<TTF_Font*>::iterator item = fonts.begin();

	for (; item != fonts.end(); ++item)
	{
		TTF_CloseFont((*item));
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFontRW(App->asset_manager->Load(path), 1 ,size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		//LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// Print text using font
SDL_Texture* Fonts::Print(const char* text, SDL_Color color, _TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{//fix
		//LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError()); 
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}
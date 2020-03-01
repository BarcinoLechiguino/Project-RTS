#include "p2Defs.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1App.h"


#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{

	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	volume = config.child("volume").attribute("value").as_int(96);
	volume_fx = config.child("volume_fx").attribute("value").as_int(26);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	music_folder.create(config.child("folder").child_value());

	return ret;
}

bool j1Audio::PreUpdate(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
	{
		volume++;
	}
	else if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT)
	{
		volume--;
	}
	return true;
}

bool j1Audio::Update(float dt)
{
	Mix_VolumeMusic(volume);

	for (std::list<Mix_Chunk*>::iterator chunk_item = fx.begin() ; chunk_item != fx.end() ; chunk_item++)
	{
		Mix_VolumeChunk((*chunk_item), volume_fx);
	}

	return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (std::list<Mix_Chunk*>::iterator chunk_item = fx.begin(); chunk_item != fx.end(); chunk_item++)
	{
		Mix_FreeChunk((*chunk_item));
	}

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	p2SString tmp("%s%s", music_folder.GetString(), path);
	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(tmp.GetString());

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	p2SString tmp("%s%s", sfx_folder.GetString(), path);

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);


	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator fx_item = fx.begin();

		std::advance(fx_item, id - 1);

		Mix_PlayChannel(-1, (*fx_item) , repeat);
	}

	return ret;
}

bool j1Audio::Load(pugi::xml_node & data)
{
	volume = data.child("volume").attribute("value").as_uint();
	return true;
}

bool j1Audio::Save(pugi::xml_node& data) const
{
	pugi::xml_node vol = data.append_child("volume");

	vol.append_attribute("value") = volume;

	return true;
}
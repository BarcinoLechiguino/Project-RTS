#include "Log.h"

#include "Application.h"
#include "Window.h"
#include "Fonts.h"
#include "Input.h"
#include "GuiManager.h"
#include "UI.h"
#include "UI_Button.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "Textures.h"
#include "Audio.h"
#include "LogoScene.h"

LogoScene::LogoScene() : Scene(SCENES::LOGO_SCENE), accumulated_time(0.f), background_rect({ 0, 0, 0, 0 }), background_texture(nullptr), logo_channel(0), logo_fx(0), logo_scene_duration(0.f)
{
	name_tag = ("Logo");
}

LogoScene::~LogoScene()
{

}

bool LogoScene::Awake(pugi::xml_node&)
{
	return true;
}

bool LogoScene::Start()
{
	InitScene();	

	LOG("Logo scene start");

	return true;	//crash
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{
	App->render->Blit(background_texture, 0, 0, &background_rect, false, 0.0f);
	return true;
}

bool LogoScene::PostUpdate()
{
	//Load Scene / Unload LogoScene
	ExecuteTransition();

	ExecuteDebugTransition();

	return true;
}

bool LogoScene::CleanUp()
{
	App->tex->UnLoad(background_texture);

	Mix_FadeOutChannel(logo_channel, 500); //crash

	App->gui_manager->DestroyGuiElements();

	return true;
}

void LogoScene::ExecuteTransition()
{
	if (App->pause)
	{
		App->pause = false;
	}
	
	accumulated_time += App->GetDt();

	if (accumulated_time >= logo_scene_duration)
	{
		App->transition_manager->CreateFadeToColour(SCENES::MAIN_MENU_SCENE);
		accumulated_time = 0.0f;
	}
}

void LogoScene::OnEventCall(UI* element, UI_EVENT ui_event)
{

}

void LogoScene::LoadGuiElements()
{

}

void LogoScene::InitScene()
{
	//logo_fx = App->audio->LoadFx("audio/fx/LogoScreen_sfx.wav");
	//logo_channel = App->audio->PlayFx(logo_fx, 0);

	logo_scene_duration = 5.0f;
	accumulated_time	= 0.0f;

	App->gui_manager->Start();

	// BACKGROUND
	background_rect = { 0, 0, 1280, 720 };
	background_texture = App->tex->Load("maps/MissingmdScreen.png");

	//LoadGuiElements();
}
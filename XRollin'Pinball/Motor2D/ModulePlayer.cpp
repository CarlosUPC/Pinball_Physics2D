#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	dock = App->physics->CreateDock();
	displacement = { 0, dockPosY };

	score = 0;
	lives = 4;

	flipper_fx = App->audio->LoadFx("pinball/flipper_fx.wav");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->scene_intro->game_started == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			dockPosY += 0.00001f;
			dock->GetBodyA()->ApplyLinearImpulse(displacement, { 0,0 }, 0);
			dock->EnableMotor(false);
		}
		else
			if (dockPosY != 0) {
				dock->EnableMotor(true);
				dockPosY = 0.0f;

			}
	}

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN))
		flipper_pressed = true;

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		App->physics->FlippersForce(b2Vec2(0, 80), b2Vec2(0, 0), LEFT);
	}

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN))
		flipper_pressed = true;

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
	{
		App->physics->FlippersForce(b2Vec2(0, -80), b2Vec2(0, 0), RIGHT);
	}

	if (flipper_pressed)
	{
		App->audio->PlayFx(flipper_fx);
		flipper_pressed = false;
	}

	return UPDATE_CONTINUE;
}




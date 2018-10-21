#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	
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
	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		App->physics->FlippersForce(b2Vec2(0, 50), b2Vec2(0, 0), LEFT);
	}

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
	{
		App->physics->FlippersForce(b2Vec2(0, -50), b2Vec2(0, 0), RIGHT);
	}

	if (is_dead)
	{
		LOG("IS DEAD!!!!");
	}
	return UPDATE_CONTINUE;
}




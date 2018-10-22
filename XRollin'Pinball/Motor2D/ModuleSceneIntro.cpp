#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = left_flipper = left_flipper = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	ball_texture = App->textures->Load("textures/Ball.png");
	map_texture = App->textures->Load("textures/Map.png");
	dock_texture = App->textures->Load("textures/Dock.png");
	startgame_texture = App->textures->Load("textures/start_game.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	//Load Lower flippers
	left_flipper = App->textures->Load("textures/LeftFlipper.png");
	right_flipper = App->textures->Load("textures/RightFlipper.png");

	//Load Upper flippers
	Upleft_flipper = App->textures->Load("textures/LeftUpperFlipper.png");
	Upright_flipper = App->textures->Load("textures/RightUpperFlipper.png");

	innerCircles.add(App->physics->CreateInnerCircle(108, 71, 10.75f));
	innerCircles.getLast()->data->listener = this;
	innerCircles.add(App->physics->CreateInnerCircle(142, 69, 10.75f));
	innerCircles.getLast()->data->listener = this;


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(map_texture, 0, 0);
	App->renderer->Blit(startgame_texture, 365, 276);

	//Print Dock texture
	int x, y;
	App->physics->dock->GetPosition(x,y);
	App->renderer->Blit(dock_texture, x, y);

	if (game_started)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 6.5f));
			circles.getLast()->data->listener = this;
		}
	}
	
	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball_texture, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = leftFlippers->getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		//Print Lower Flipper
		if (c == leftFlippers->getFirst())
		App->renderer->Blit(left_flipper, x - 5, y - 7, NULL, 1.0f, c->data->GetRotation() - 30);

		//Print Upper Flipper
		if (c == leftFlippers->getLast())
		App->renderer->Blit(Upleft_flipper, x - 3, y - 3, NULL, 1.0f, c->data->GetRotation() - 20);

		c = c->next;
	}

	c = rightFlippers->getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		//Print Lower Flipper
		if(c == rightFlippers->getFirst())
		App->renderer->Blit(right_flipper, x + 3, y - 7, NULL, 1.0f, c->data->GetRotation() - 330);

		//Print Upper Flipper
		if (c == rightFlippers->getLast())
		App->renderer->Blit(Upright_flipper, x + 6, y - 12, NULL, 1.0f, c->data->GetRotation() + 55);

		c = c->next;
	}

	if (App->input->GetMouseX() > 365 && App->input->GetMouseY() > 276 && App->input->GetMouseX() < 426 && App->input->GetMouseY() < 361)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			game_started = true;
			startgame_texture = nullptr;
		}
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}

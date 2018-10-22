#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleWindow.h"
#include "ModuleFonts.h"
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
	insertCoin_texture = App->textures->Load("textures/InsertCoin.png");
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

	// UI FONTS
	score_font = App->fonts->Load("textures/Score.png", "0123456789", 1);
	lifes_font = App->fonts->Load("textures/Lives.png", "0123", 1);

	//Death collider
	end = App->physics->CreateRectangleSensor(110, 440, 100, 10);
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	//Freeing fonts
	App->fonts->UnLoad(score_font);
	App->fonts->UnLoad(lifes_font);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(map_texture, 0, 0);


	if (App->input->GetMouseX() > 365 && App->input->GetMouseY() > 276 && App->input->GetMouseX() < 426 && App->input->GetMouseY() < 361 && App->player->lives == 4)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			//GAME STARTS & CREATES A BALL
			game_started = true;
			create_ball = true;
			App->player->lives = 1;
		}
	}

	if (!game_started)
	{
		App->renderer->Blit(insertCoin_texture, 257, 104);
		App->renderer->Blit(startgame_texture, 365, 276);
	}
	

	//Print Dock texture
	int x, y;
	App->physics->dock->GetPosition(x,y);
	App->renderer->Blit(dock_texture, x, y);

	if (game_started == true && create_ball && App->player->lives < 4)
	{
		PlayerBall();
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


	//Print Font
	sprintf_s(score_text, 10, "%7d", App->player->score);
	sprintf_s(lifes_text, 10, "%7d", App->player->lives);

	App->fonts->BlitText(325, 250, score_font, score_text);
	App->fonts->BlitText(323, 282, lifes_font, lifes_text);


	//Screen display
	char score[64];
	char lives[4];
	char Title[64] = "PinBall Score: ";
	char Num_lives[32] = "Lives: ";
	
	sprintf_s(score, "%d  ", App->player->score);
	sprintf_s(lives, "%d", App->player->lives);

	strcat_s(Title, score);
	strcat_s(Title, Num_lives);
	strcat_s(Title, lives);

	App->window->SetTitle(Title);
	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == end) {
		

		App->physics->world->DestroyBody(circles.getLast()->data->body);
		circles.getLast()->data = nullptr;
		circles.clear();
		App->player->lives++;
		create_ball = true;

		if (App->player->lives == 4)
			game_started = false;

		
	}

	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::PlayerBall() {
	circles.add(App->physics->CreateCircle(234, 350, 6.5f));
	circles.getLast()->data->listener = this;
	//circles.getLast()->data->body->SetBullet(true);

	create_ball = false;
}
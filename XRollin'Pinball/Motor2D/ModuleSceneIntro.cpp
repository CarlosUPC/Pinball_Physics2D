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

	//Blue sensors--------------------------------------------
	blueSensor.PushBack({ 20,33,9,9 });
	blueSensor.speed = 0.01f;

	//Small blue sensors---------------------------------------
	smallBlueSensor.PushBack({ 32,35,5,5 });
	smallBlueSensor.speed = 0.01f;

	
	//Shiny Sensors--------------------------------------------
	ShinySensor1.PushBack({1,31,15,15});
	ShinySensor1.speed = 0.2f;

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
	sensors_texture = App->textures->Load("textures/Sensors.png");
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

	//--------------------------Death Sensor-------------------------------//
	end = App->physics->CreateRectangleSensor(110, 440, 100, 10);
	
	// FX

	red_circle_fx = App->audio->LoadFx("pinball/red_circle_collision.wav");
	red_circle_sensor1 = App->physics->CreateCircleSensor(108, 71, 11);
	red_circle_sensor2 = App->physics->CreateCircleSensor(142, 70, 11);

	blue_circle_fx = App->audio->LoadFx("pinball/boing_collision.wav");
	blue_circle_sensor1 = App->physics->CreateCircleSensor(8.5f, 362, 6.5f);
	blue_circle_sensor2 = App->physics->CreateCircleSensor(199, 362, 6.5f);

	car_start_fx = App->audio->LoadFx("pinball/car_start.wav");

	//-------------------------------Sensors------------------------------//

	BlueSensors[0] = App->physics->CreateRectangleSensor(180, 17, 10, 11);
	BlueSensors[1] = App->physics->CreateRectangleSensor(162, 199, 10, 10);
	BlueSensors[2] = App->physics->CreateRectangleSensor(113, 30, 10, 10);
	BlueSensors[3] = App->physics->CreateRectangleSensor(131, 28, 10, 10);
	BlueSensors[4] = App->physics->CreateRectangleSensor(149, 28, 10, 10);

	SmallBlueSensors[0] = App->physics->CreateRectangleSensor(175, 59, 5, 5);
	SmallBlueSensors[1] = App->physics->CreateRectangleSensor(187, 67, 5, 5);
	SmallBlueSensors[2] = App->physics->CreateRectangleSensor(199, 75, 5, 5);
	SmallBlueSensors[3] = App->physics->CreateRectangleSensor(92, 212, 5, 5);
	SmallBlueSensors[4] = App->physics->CreateRectangleSensor(105, 219, 5, 5);
	SmallBlueSensors[5] = App->physics->CreateRectangleSensor(32, 217, 5, 5);
	SmallBlueSensors[6] = App->physics->CreateRectangleSensor(27, 229, 5, 5);
	SmallBlueSensors[7] = App->physics->CreateRectangleSensor(22, 242, 5, 5);

	ShinySensors[0] = App->physics->CreateRectangleSensor(106, 71, 20, 20);
	ShinySensors[1] = App->physics->CreateRectangleSensor(141, 68, 20, 20);

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
			App->audio->PlayFx(car_start_fx);
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
	
	//------------------------------Print Blue Sensors---------------------------------//

	if (BlueSensor_1 == true) {
		App->renderer->Blit(sensors_texture, 175, 13, &(blueSensor.GetCurrentFrame()));
		if (blueSensor.current_frame == 0) {
			blueSensor.Reset();
			BlueSensor_1 = false;
		}
	}
	if (BlueSensor_2 == true) {
		App->renderer->Blit(sensors_texture,158,196, &(blueSensor.GetCurrentFrame()));
		if (blueSensor.current_frame == 0) {
			blueSensor.Reset();
			BlueSensor_2 = false;
		}
	}
	if (BlueSensor_3 == true) {
		App->renderer->Blit(sensors_texture,109, 26, &(blueSensor.GetCurrentFrame()));
		if (blueSensor.current_frame == 0) {
			blueSensor.Reset();
			BlueSensor_3 = false;
		}

	}
	if (BlueSensor_4 == true) {
		App->renderer->Blit(sensors_texture, 127, 24, &(blueSensor.GetCurrentFrame()));
		if (blueSensor.current_frame == 0) {
			blueSensor.Reset();
			BlueSensor_4 = false;
		}
	}
	if (BlueSensor_5 == true) {
		App->renderer->Blit(sensors_texture, 145, 25, &(blueSensor.GetCurrentFrame()));
		if (blueSensor.current_frame == 0) {
			blueSensor.Reset();
			BlueSensor_5 = false;
		}
	}

	//-----------------------------Print Small Blue Sensors---------------------------------------//

	if (SmallBlueSensor_1 == true) {
		App->renderer->Blit(sensors_texture, 173, 58, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_2 == true) {
		App->renderer->Blit(sensors_texture, 186, 66, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_3 == true) {
		App->renderer->Blit(sensors_texture, 198, 74, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_4 == true) {
		App->renderer->Blit(sensors_texture, 91, 211, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_5 == true) {
		App->renderer->Blit(sensors_texture, 104, 218, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_6 == true) {
		App->renderer->Blit(sensors_texture, 31, 216, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_7 == true) {
		App->renderer->Blit(sensors_texture, 26, 228, &(smallBlueSensor.GetCurrentFrame()));
	}

	if (SmallBlueSensor_8 == true) {
		App->renderer->Blit(sensors_texture, 21, 241, &(smallBlueSensor.GetCurrentFrame()));
	}


	//--------------------------------Print Shiny Sensors--------------------------------------//
	if (ShinySensor_1 == true) {
		App->renderer->Blit(sensors_texture, 99, 64, &(ShinySensor1.GetCurrentFrame()));
		if (ShinySensor1.current_frame == 0) {
			ShinySensor1.Reset();
			ShinySensor_1 = false;
		}
	}
	if (ShinySensor_2 == true) {
		App->renderer->Blit(sensors_texture, 134, 62, &(ShinySensor1.GetCurrentFrame()));
		if (ShinySensor1.current_frame == 0) {
			ShinySensor1.Reset();
			ShinySensor_2 = false;
		}
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

	Destroy();

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
		
		toDestroy = circles.getLast()->data;
		
		circles.clear();
		App->player->lives++;
		create_ball = true;

		if (App->player->lives == 4)
			game_started = false;
<<<<<<< HEAD
	
	}

	// Blue Sensors----------------------------------------------------------
	if (bodyB == BlueSensors[0]) {
		BlueSensor_1 = true;
		
		App->player->score += 5;
	}
	if (bodyB == BlueSensors[1]) {
		BlueSensor_2 = true;
		
		App->player->score += 5;
	}
	if (bodyB == BlueSensors[2]) {
		BlueSensor_3 = true;
		
		App->player->score += 5;
	}
	if (bodyB == BlueSensors[3]) {
		BlueSensor_4 = true;
		
		App->player->score += 5;
	}

	if (bodyB == BlueSensors[4]) {
		BlueSensor_5 = true;
		App->player->score += 5;
	}

	// Small Blue Sensors-------------------------------------------------------
	if (bodyB == SmallBlueSensors[0]) {
		SmallBlueSensor_1 = true;

		App->player->score += 5;
	}
	if (bodyB == SmallBlueSensors[1]) {
		SmallBlueSensor_2 = true;

		App->player->score += 5;
=======
>>>>>>> e0ab3e61b1d97087cd5270ed8dc2096a9cbf1d12
	}
	if (bodyB == SmallBlueSensors[2]) {
		SmallBlueSensor_3 = true;

		App->player->score += 5;
	}
	if (bodyB == SmallBlueSensors[3]) {
		SmallBlueSensor_4 = true;

		App->player->score += 5;
	}

	if (bodyB == SmallBlueSensors[4]) {
		SmallBlueSensor_5 = true;
		App->player->score += 5;
	}
	if (bodyB == SmallBlueSensors[5]) {
		SmallBlueSensor_6 = true;

		App->player->score += 5;
	}
	if (bodyB == SmallBlueSensors[6]) {
		SmallBlueSensor_7 = true;

		App->player->score += 5;
	}

	if (bodyB == SmallBlueSensors[7]) {
		SmallBlueSensor_8 = true;
		App->player->score += 5;
	}


	// Shiny Sensors--------------------------------------------------------------
	if (bodyB == ShinySensors[0]) {
		ShinySensor_1 = true;
		
		App->player->score += 10;
	}
	if (bodyB == ShinySensors[1]) {
		ShinySensor_2 = true;
	
		App->player->score += 10;
	}


	if(bodyB == red_circle_sensor1 || bodyB == red_circle_sensor2)
		App->audio->PlayFx(red_circle_fx);

	if (bodyB == blue_circle_sensor1)
		App->audio->PlayFx(blue_circle_fx);


	//App->audio->PlayFx(bonus_fx);
}


void ModuleSceneIntro::Destroy() {

	if (toDestroy != nullptr) {
		App->physics->world->DestroyBody(toDestroy->body);
		toDestroy = nullptr;
	}
}

void ModuleSceneIntro::PlayerBall() {
	circles.add(App->physics->CreateCircle(234, 350, 6.5f));
	circles.getLast()->data->listener = this;
	circles.getLast()->data->body->SetBullet(true);

	create_ball = false;
}
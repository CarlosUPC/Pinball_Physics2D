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


	arrow1.PushBack({ 15,1,12,9 });
	arrow1.PushBack({ 3,14,11,10 });
	arrow1.PushBack({ 2,1,11,10 });
	arrow1.speed = 0.15f;
	arrow1.loop = true;

	arrow2.PushBack({ 2,1,11,10 });
	arrow2.PushBack({ 15,1,12,9 });
	arrow2.PushBack({ 3,14,11,10 });
	arrow2.speed = 0.15f;
	arrow2.loop = true;

	arrow3.PushBack({ 3,14,11,10 });
	arrow3.PushBack({ 2,1,11,10 });
	arrow3.PushBack({ 15,1,12,9 });
	arrow3.speed = 0.15f;
	arrow3.loop = true;

	arrow4.PushBack({ 54, 2, 9, 10 });
	arrow4.PushBack({ 31, 1, 8, 10 });
	arrow4.PushBack({ 43, 2, 8, 9  });
	arrow4.speed = 0.08f;
	arrow4.loop = true;

	arrow5.PushBack({ 31, 1, 8, 10 });
	arrow5.PushBack({ 43, 2, 8, 9 });
	arrow5.PushBack({ 54, 2, 9, 10 });
	arrow5.speed = 0.08f;
	arrow5.loop = true;

	arrow6.PushBack({ 43, 2, 8, 9 });
	arrow6.PushBack({ 54, 2, 9, 10 });
	arrow6.PushBack({ 31, 1, 8, 10  });
	arrow6.speed = 0.08f;
	arrow6.loop = true;

	arrow7.PushBack({ 45, 17, 7, 7 });
	arrow7.PushBack({ 55, 17, 7, 7 });
	arrow7.PushBack({ 33, 17, 7, 7 });
	arrow7.speed = 0.12f;
	arrow7.loop = true;

	arrow8.PushBack({ 33, 17, 7, 7 });
	arrow8.PushBack({ 45, 17, 7, 7 });
	arrow8.PushBack({ 55, 17, 7, 7 });
	arrow8.speed = 0.12f;
	arrow8.loop = true;

	arrow9.PushBack({ 55, 17, 7, 7 });
	arrow9.PushBack({ 33, 17, 7, 7 });
	arrow9.PushBack({ 45, 17, 7, 7 });
	arrow9.speed = 0.12f;
	arrow9.loop = true;

	

	//Blue sensors--------------------------------------------
	blueSensor.PushBack({ 20,33,9,9 });
	blueSensor.speed = 0.01f;

	//flipper Blue sensors--------------------------------------------
	flipperSensor.PushBack({ 51,51,12,12 });
	flipperSensor.speed = 0.01f;

	//Small blue sensors---------------------------------------
	smallBlueSensor.PushBack({ 32,35,5,5 });
	smallBlueSensor.speed = 0.01f;

	//Medium blue sensors---------------------------------------
	mediumBlueSensor.PushBack({4, 57, 8, 7});
	mediumBlueSensor.speed = 0.01f;
	
	//Shiny Sensors--------------------------------------------
	ShinySensor1.PushBack({1,31,15,15});
	ShinySensor1.speed = 0.2f;

	ShinySensor2.PushBack({5, 71, 21, 49});
	ShinySensor2.speed = 0.2f;

	ShinySensor3.PushBack({ 37, 71, 21, 49 });
	ShinySensor3.speed = 0.2f;

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
	flipperBouncer_texture = App->textures->Load("textures/bouncer_animation.png");
	tunnel_texture = App->textures->Load("textures/tunnel_texture.png");

	//Load Lower flippers
	left_flipper = App->textures->Load("textures/LeftFlipper.png");
	right_flipper = App->textures->Load("textures/RightFlipper.png");

	//Load Upper flippers
	Upleft_flipper = App->textures->Load("textures/LeftUpperFlipper.png");
	Upright_flipper = App->textures->Load("textures/RightUpperFlipper.png");

	
	// UI FONTS
	score_font = App->fonts->Load("textures/Score.png", "0123456789", 1);
	maxScore_font = App->fonts->Load("textures/MaxScore.png", "0123456789", 1);
	lifes_font = App->fonts->Load("textures/Lives.png", "0123", 1);

	//--------------------------Death Sensor-------------------------------//
	end = App->physics->CreateRectangleSensor(110, 440, 100, 10);
	
	// FX

	red_circle_fx = App->audio->LoadFx("pinball/red_circle_collision.wav");
	

	blue_circle_fx = App->audio->LoadFx("pinball/boing_collision.wav");
	

	car_start_fx = App->audio->LoadFx("pinball/car_start.wav");

	top_blue_sensors_fx = App->audio->LoadFx("pinball/top_blue_sensors_fx.wav");

	//-------------------------------Bouncers------------------------------//
	bounceCircles.add(App->physics->CreateBounceCircle(108, 71, 10.75f));
	bounceCircles.getLast()->data->listener = this;
	bounceCircles.add(App->physics->CreateBounceCircle(142, 69, 10.75f));
	bounceCircles.getLast()->data->listener = this;
	
	bouncerInclined = App->physics->CreateRotateRectangle(65, 147, 35, 3, b2_staticBody, 0.5f);

	StartSensor = App->physics->CreateRotateRectangle(230, 174, 35, 3, b2_staticBody, -1.f);
	StartSensor->body->SetActive(false);
	
	ExitSensorChecker = App->physics->CreateRectangleSensor(230, 150, 25, 3);

	//-------------------------------Sensors------------------------------//
	BlueSensors[0] = App->physics->CreateRectangleSensor(180, 17, 10, 11);
	BlueSensors[1] = App->physics->CreateRectangleSensor(162, 199, 10, 10);
	BlueSensors[2] = App->physics->CreateRectangleSensor(113, 30, 10, 10);
	BlueSensors[3] = App->physics->CreateRectangleSensor(131, 28, 10, 10);
	BlueSensors[4] = App->physics->CreateRectangleSensor(149, 28, 10, 10);
	BlueSensors[5] = App->physics->CreateRectangleSensor(9, 355, 10, 10);
	BlueSensors[6] = App->physics->CreateRectangleSensor(198, 355, 10, 10);

	tp_sensor = App->physics->CreateRotateRectangle(73, 163, 8, 30, b2_staticBody, -1.0f);
	tp_sensor->body->GetFixtureList()->SetSensor(true);


	MediumBlueSensors[0] = App->physics->CreateRectangleSensor(33, 104, 7, 7);
	MediumBlueSensors[1] = App->physics->CreateRectangleSensor(29, 88, 7, 7);
	MediumBlueSensors[2] = App->physics->CreateRectangleSensor(28, 73, 7, 7);
	MediumBlueSensors[3] = App->physics->CreateRectangleSensor(27, 58, 7, 7);
	MediumBlueSensors[4] = App->physics->CreateRectangleSensor(27, 44, 7, 7);
	MediumBlueSensors[5] = App->physics->CreateRectangleSensor(27, 29, 7, 7);
	MediumBlueSensors[6] = App->physics->CreateRectangleSensor(10, 29, 7, 7);
	MediumBlueSensors[7] = App->physics->CreateRectangleSensor(9, 44, 7, 7);
	MediumBlueSensors[8] = App->physics->CreateRectangleSensor(9, 59, 7, 7);
	MediumBlueSensors[9] = App->physics->CreateRectangleSensor(10, 75, 7, 7);
	MediumBlueSensors[10] = App->physics->CreateRectangleSensor(11, 91, 7, 7);
	MediumBlueSensors[11] = App->physics->CreateRectangleSensor(14, 107, 7, 7);
	MediumBlueSensors[12] = App->physics->CreateRectangleSensor(17, 122, 7, 7);
	MediumBlueSensors[13] = App->physics->CreateRectangleSensor(22, 139, 7, 7);
	MediumBlueSensors[14] = App->physics->CreateRectangleSensor(28, 156, 7, 7);

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
	ShinySensors[2] = App->physics->CreateRotateRectangle(50, 322, 8, 53, b2_staticBody, -0.5f);
	ShinySensors[3] = App->physics->CreateRotateRectangle(159, 322, 10, 53, b2_staticBody, 0.5f);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	//Freeing fonts
	App->fonts->UnLoad(score_font);
	App->fonts->UnLoad(lifes_font);
	App->fonts->UnLoad(maxScore_font);

	App->textures->Unload(ball_texture);
	ball_texture = nullptr;

	App->textures->Unload(map_texture);
	map_texture = nullptr;

	App->textures->Unload(dock_texture);
	dock_texture = nullptr;

	App->textures->Unload(startgame_texture);
	startgame_texture = nullptr;

	App->textures->Unload(insertCoin_texture);
	insertCoin_texture = nullptr;

	App->textures->Unload(sensors_texture);
	sensors_texture = nullptr;

	App->textures->Unload(flipperBouncer_texture);
	flipperBouncer_texture = nullptr;

	App->textures->Unload(tunnel_texture);
	tunnel_texture = nullptr;

	App->textures->Unload(left_flipper);
	left_flipper = nullptr;

	App->textures->Unload(right_flipper);
	right_flipper = nullptr;

	App->textures->Unload(Upleft_flipper);
	Upleft_flipper = nullptr;

	App->textures->Unload(Upright_flipper);
	Upright_flipper = nullptr;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{


	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	App->renderer->Blit(map_texture, 0, 0);
	App->renderer->Blit(tunnel_texture, 32, 128);

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
		
		if (circles.getLast() != nullptr)
			circles.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(mouse.x),  PIXEL_TO_METERS(mouse.y) }, 0.0f);
	}


	if (game_started) {
		App->renderer->Blit(sensors_texture, 109, 8, &(arrow1.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 126, 8, &(arrow2.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 144, 8, &(arrow3.GetCurrentFrame()));

		App->renderer->Blit(sensors_texture, 163, 64, &(arrow4.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 176, 73, &(arrow5.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 189, 81, &(arrow6.GetCurrentFrame()));

		App->renderer->Blit(sensors_texture, 38, 219, &(arrow7.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 33, 231, &(arrow8.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 28, 243, &(arrow9.GetCurrentFrame()));

		App->renderer->Blit(sensors_texture, 82, 219, &(arrow6.GetCurrentFrame()));
		App->renderer->Blit(sensors_texture, 96, 225, &(arrow5.GetCurrentFrame()));
	}

	if (App->input->GetMouseX() > 365 && App->input->GetMouseY() > 276 && App->input->GetMouseX() < 426 && App->input->GetMouseY() < 361 && App->player->lives == 4)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			//GAME STARTS & CREATES A BALL
			game_started = true;
			create_ball = true;
			App->player->lives = 1;
			App->player->score = 0;
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
		

	}
	if (BlueSensor_4 == true) {
		App->renderer->Blit(sensors_texture, 127, 24, &(blueSensor.GetCurrentFrame()));
		
	}
	if (BlueSensor_5 == true) {
		App->renderer->Blit(sensors_texture, 145, 25, &(blueSensor.GetCurrentFrame()));
		
	}

	//-----------------------------Print Flipper Blue Sensors---------------------------------//
	if (BlueSensor_6 == true) {

		 App->renderer->Blit(sensors_texture, 1, 355, &(flipperSensor.GetCurrentFrame()));
	}

	if (BlueSensor_7 == true) {

		 App->renderer->Blit(sensors_texture, 192, 355, &(flipperSensor.GetCurrentFrame()));
	}

	//-----------------------------Print Medium Blue Sensors---------------------------------//

	if (MediumBlueSensor_1 == true) {
		App->renderer->Blit(sensors_texture, 29, 99, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_1 = false;
		}
	}

	if (MediumBlueSensor_2 == true) {
		App->renderer->Blit(sensors_texture, 25, 83, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_2 = false;
		}
	}

	if (MediumBlueSensor_3 == true) {
		App->renderer->Blit(sensors_texture, 24, 69, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_3 = false;
		}
	}

	if (MediumBlueSensor_4 == true) {
		App->renderer->Blit(sensors_texture, 23, 54, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_4 = false;
		}
	}

	if (MediumBlueSensor_5 == true) {
		App->renderer->Blit(sensors_texture, 23, 40, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_5 = false;
		}
	}

	if (MediumBlueSensor_6 == true) {
		App->renderer->Blit(sensors_texture, 23, 25, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_6 = false;
		}
	}

	if (MediumBlueSensor_7 == true) {
		App->renderer->Blit(sensors_texture, 6, 25, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_7 = false;
		}
	}

	if (MediumBlueSensor_8 == true) {
		App->renderer->Blit(sensors_texture, 5, 40, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_8 = false;
		}
	}

	if (MediumBlueSensor_9 == true) {
		App->renderer->Blit(sensors_texture, 5, 55, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_9 = false;
		}
	}

	if (MediumBlueSensor_10 == true) {
		App->renderer->Blit(sensors_texture, 6, 71, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_10 = false;
		}
	}

	if (MediumBlueSensor_11 == true) {
		App->renderer->Blit(sensors_texture, 7, 87, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_11 = false;
		}
	}

	if (MediumBlueSensor_12 == true) {
		App->renderer->Blit(sensors_texture, 10, 103, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_12 = false;
		}
	}

	if (MediumBlueSensor_13 == true) {
		App->renderer->Blit(sensors_texture, 13, 118, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_13 = false;
		}
	}

	if (MediumBlueSensor_14 == true) {
		App->renderer->Blit(sensors_texture, 18, 135, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_14 = false;
		}
	}

	if (MediumBlueSensor_15 == true) {
		App->renderer->Blit(sensors_texture, 24, 152, &(mediumBlueSensor.GetCurrentFrame()));
		if (mediumBlueSensor.current_frame == 0) {
			mediumBlueSensor.Reset();
			MediumBlueSensor_15 = false;
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

	if (ShinySensor_3 == true) {
		App->renderer->Blit(sensors_texture, 37, 300, &(ShinySensor2.GetCurrentFrame()));
		if (ShinySensor2.current_frame == 0) {
			ShinySensor2.Reset();
			ShinySensor_3 = false;
		}
	}

	if (ShinySensor_4 == true) {
		App->renderer->Blit(sensors_texture, 150, 300, &(ShinySensor3.GetCurrentFrame()));
		if (ShinySensor3.current_frame == 0) {
			ShinySensor3.Reset();
			ShinySensor_4 = false;
		}
	}

	if (open == true && died == true) {
	
		open = false;
		died = false;
		StartSensor->body->SetActive(true);
		StartSensor->body->GetFixtureList()->SetRestitution(0);
	}


	//--------------------------------Teleport Sensors--------------------------------------//
	if (ChangHole == true) {

		circles.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(135),  PIXEL_TO_METERS(210) }, 0.0f);
		
		if (SDL_GetTicks() > ticks) {
			ChangHole = false;
		}

		
	}


	
	

	
	// Prepare for raycast ------------------------------------------------------
	


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
		App->renderer->Blit(left_flipper, x + 3 , y - 6 , NULL, 1.0f, c->data->GetRotation() - 30);

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
		App->renderer->Blit(right_flipper, x , y - 6, NULL, 1.0f, c->data->GetRotation() - 330);

		//Print Upper Flipper
		if (c == rightFlippers->getLast())
		App->renderer->Blit(Upright_flipper, x + 6, y - 12, NULL, 1.0f, c->data->GetRotation() + 55);

		c = c->next;
	}

	Destroy();

	//Print Font
	sprintf_s(score_text,"%i", App->player->score);
	sprintf_s(lifes_text,"%i", App->player->lives);

	App->fonts->BlitText(325, 250, score_font, score_text);
	App->fonts->BlitText(323, 282, lifes_font, lifes_text);
	
	sprintf_s(highscore_text,"%i", App->player->highscore);
	App->fonts->BlitText(325, 220, maxScore_font, highscore_text);

	if (App->player->lives == 4) {
		
		if (App->player->score > App->player->highscore) {
			App->player->highscore = App->player->score;
		}
	}

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
	
	for (int i = 0; i < 2; i++) {

		if (bodyB == FlipperBouncers[i]) {

			toDestroy = FlipperBouncers[i];
			FlipperBouncers[i] = nullptr;
		}
	}

	if (bodyB == end) {
		
		toDestroy = circles.getLast()->data;
		
		circles.clear();
		App->player->lives++;
		create_ball = true;

		if (App->player->lives == 4)
			game_started = false;
	}

	if (bodyB == tp_sensor) {
		Timer(1500);
		bonusX2 = true;
		ChangHole = true;
	}

	// Blue Sensors----------------------------------------------------------
	if (bodyB == BlueSensors[0]) {
		BlueSensor_1 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == BlueSensors[1]) {
		BlueSensor_2 = true;
		App->audio->PlayFx(top_blue_sensors_fx);

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == BlueSensors[2]) {
		BlueSensor_3 = true;
		App->audio->PlayFx(top_blue_sensors_fx);
		
		if(bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == BlueSensors[3]) {
		BlueSensor_4 = true;
		App->audio->PlayFx(top_blue_sensors_fx);

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == BlueSensors[4]) {
		BlueSensor_5 = true;
		App->audio->PlayFx(top_blue_sensors_fx);

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == BlueSensors[5]) {
		BlueSensor_6 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(blue_circle_fx);
	}

	if (bodyB == BlueSensors[6]) {
		BlueSensor_7 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(blue_circle_fx);
	}

	// Medium Blue Sensors-------------------------------------------------------

	if (bodyB == MediumBlueSensors[0]) {
		MediumBlueSensor_1 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[1]) {
		MediumBlueSensor_2 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[2]) {
		MediumBlueSensor_3 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[3]) {
		MediumBlueSensor_4 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[4]) {
		MediumBlueSensor_5 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[5]) {
		MediumBlueSensor_6 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[6]) {
		MediumBlueSensor_7 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[7]) {
		MediumBlueSensor_8 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[8]) {
		MediumBlueSensor_9 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[9]) {
		MediumBlueSensor_10 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[10]) {
		MediumBlueSensor_11 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[11]) {
		MediumBlueSensor_12 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;

		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[12]) {
		MediumBlueSensor_13 = true;
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[13]) {
		MediumBlueSensor_14 = true;
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
		App->audio->PlayFx(top_blue_sensors_fx);
	}

	if (bodyB == MediumBlueSensors[14]) {
		MediumBlueSensor_15 = true;
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
		App->audio->PlayFx(top_blue_sensors_fx);
	}


	// Small Blue Sensors-------------------------------------------------------
	if (bodyB == SmallBlueSensors[0]) {
		SmallBlueSensor_1 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == SmallBlueSensors[1]) {
		SmallBlueSensor_2 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == SmallBlueSensors[2]) {
		SmallBlueSensor_3 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == SmallBlueSensors[3]) {
		SmallBlueSensor_4 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == SmallBlueSensors[4]) {
		SmallBlueSensor_5 = true;
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == SmallBlueSensors[5]) {
		SmallBlueSensor_6 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == SmallBlueSensors[6]) {
		SmallBlueSensor_7 = true;

		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == SmallBlueSensors[7]) {
		SmallBlueSensor_8 = true;
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}


	// Shiny Sensors--------------------------------------------------------------
	if (bodyB == ShinySensors[0]) {
		ShinySensor_1 = true;
		App->audio->PlayFx(red_circle_fx);
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}
	if (bodyB == ShinySensors[1]) {
		ShinySensor_2 = true;
		App->audio->PlayFx(red_circle_fx);
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == ShinySensors[2]) {
		ShinySensor_3 = true;
		App->audio->PlayFx(red_circle_fx);
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == ShinySensors[3]) {
		ShinySensor_4 = true;
		App->audio->PlayFx(red_circle_fx);
		if (bonusX2)
			App->player->score += 200;

		else App->player->score += 100;
	}

	if (bodyB == ExitSensorChecker && died) {
		open = true;
	}

	//App->audio->PlayFx(bonus_fx);
}


void ModuleSceneIntro::Destroy() {

	if (toDestroy != nullptr) {
		App->physics->world->DestroyBody(toDestroy->body);
		toDestroy = nullptr;


	}
	//died = true;
	//ExitSensor->body->SetActive(false);
	
}

void ModuleSceneIntro::PlayerBall() {
	
	for (int i = 0; i < 2; i++) {
		if (FlipperBouncers[i] != nullptr) {
			App->physics->world->DestroyBody(FlipperBouncers[i]->body);
		}
	}

	BlueSensor_6 = false;
	BlueSensor_7 = false;
	
	circles.add(App->physics->CreateCircle(234, 350, 6.5f));
	circles.getLast()->data->listener = this;
	circles.getLast()->data->body->SetBullet(true);


	FlipperBouncers[0] = App->physics->CreateRectangle(198, 360, 8, 12, b2_staticBody);
	FlipperBouncers[0]->body->GetFixtureList()->SetRestitution(2.0f);
	FlipperBouncers[1] = App->physics->CreateRectangle(9, 360, 8, 12, b2_staticBody);
	FlipperBouncers[1]->body->GetFixtureList()->SetRestitution(2.0f);

	
	create_ball = false;
	StartSensor->body->SetActive(false);
	died = true;
	bonusX2 = false;
}

void ModuleSceneIntro::Timer(int time)
{
	//pause = true;
	ticks = SDL_GetTicks() + time;
}
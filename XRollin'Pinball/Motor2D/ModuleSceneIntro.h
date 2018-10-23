#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void PlayerBall();
	void Destroy();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> innerCircles;

	p2List<PhysBody*>* leftFlippers;
	p2List<PhysBody*>* rightFlippers;


	PhysBody* BlueSensors[5];
	PhysBody* SmallBlueSensors[8];
	PhysBody* ShinySensors[2];

	SDL_Texture* circle = nullptr;
	SDL_Texture* box = nullptr;
	SDL_Texture* rick = nullptr;

	SDL_Texture* map_texture = nullptr;
	SDL_Texture* ball_texture = nullptr;
	SDL_Texture* sensors_texture = nullptr;
	SDL_Texture* startgame_texture = nullptr;
	SDL_Texture* insertCoin_texture = nullptr;
	SDL_Texture* dock_texture = nullptr;

	Animation blueSensor;
	
	Animation smallBlueSensor;
	
	Animation ShinySensor1; //Red Circle Bounce
	Animation ShinySensor2; //Red Triangular Bounce


	
	bool BlueSensor_1 = false;
	bool BlueSensor_2 = false;
	bool BlueSensor_3 = false;
	bool BlueSensor_4 = false;
	bool BlueSensor_5 = false;

	bool SmallBlueSensor_1 = false;
	bool SmallBlueSensor_2 = false;
	bool SmallBlueSensor_3 = false;
	bool SmallBlueSensor_4 = false;
	bool SmallBlueSensor_5 = false;
	bool SmallBlueSensor_6 = false;
	bool SmallBlueSensor_7 = false;
	bool SmallBlueSensor_8 = false;

	bool ShinySensor_1 = false;
	bool ShinySensor_2 = false;
	

	
	bool game_started = false;
	bool create_ball = false;
	
	SDL_Texture* Upleft_flipper = nullptr;
	SDL_Texture* Upright_flipper = nullptr;

	SDL_Texture* left_flipper = nullptr;
	SDL_Texture* right_flipper = nullptr;
	PhysBody* toDestroy;
	uint bonus_fx;
	uint red_circle_fx;
	uint blue_circle_fx;
	uint car_start_fx;

	char score_text[10];
	char lifes_text[10];

	PhysBody* end;
	PhysBody* red_circle_sensor1;
	PhysBody* red_circle_sensor2;

	PhysBody* blue_circle_sensor1;
	PhysBody* blue_circle_sensor2;

	int score_font;
	int lifes_font;
};

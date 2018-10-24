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
	p2List<PhysBody*> bounceCircles;

	p2List<PhysBody*>* leftFlippers;
	p2List<PhysBody*>* rightFlippers;

	PhysBody* StartSensor;
	PhysBody* ExitSensorChecker;
	PhysBody* BlueSensors[7];
	PhysBody* MediumBlueSensors[15];
	PhysBody* SmallBlueSensors[8];

	PhysBody* bouncerInclined;
	PhysBody* FlipperBouncers[2];
	PhysBody* ShinySensors[4];


	SDL_Texture* circle = nullptr;
	SDL_Texture* box = nullptr;
	SDL_Texture* rick = nullptr;

	SDL_Texture* map_texture = nullptr;
	SDL_Texture* ball_texture = nullptr;
	SDL_Texture* sensors_texture = nullptr;
	SDL_Texture* startgame_texture = nullptr;
	SDL_Texture* insertCoin_texture = nullptr;
	SDL_Texture* dock_texture = nullptr;
	SDL_Texture* flipperBouncer_texture = nullptr;
	
	Animation arrow1;
	Animation arrow2;
	Animation arrow3;

	Animation arrow4;
	Animation arrow5;
	Animation arrow6;

	Animation arrow7;
	Animation arrow8;
	Animation arrow9;





	Animation blueSensor;
	Animation flipperSensor;
	Animation mediumBlueSensor; // blue sensors path
	Animation smallBlueSensor;
	
	Animation ShinySensor1; //Red Circle Bounce
	Animation ShinySensor2; //Red Left Triangular Bounce
	Animation ShinySensor3; //Red Right Triangular Bounce

	bool Exit_Sensor = false;
	bool Exit_SensorChecker = false;

	bool BlueSensor_1 = false;
	bool BlueSensor_2 = false;
	bool BlueSensor_3 = false;
	bool BlueSensor_4 = false;
	bool BlueSensor_5 = false;
	bool BlueSensor_6 = false;
	bool BlueSensor_7 = false;

	bool MediumBlueSensor_1 = false;
	bool MediumBlueSensor_2 = false;
	bool MediumBlueSensor_3 = false;
	bool MediumBlueSensor_4 = false;
	bool MediumBlueSensor_5 = false;
	bool MediumBlueSensor_6 = false;
	bool MediumBlueSensor_7 = false;
	bool MediumBlueSensor_8 = false;
	bool MediumBlueSensor_9 = false;
	bool MediumBlueSensor_10 = false;
	bool MediumBlueSensor_11 = false;
	bool MediumBlueSensor_12 = false;
	bool MediumBlueSensor_13 = false;
	bool MediumBlueSensor_14 = false;
	bool MediumBlueSensor_15 = false;

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
	bool ShinySensor_3 = false;
	bool ShinySensor_4 = false;

	
	bool game_started = false;
	bool create_ball = false;

	bool open = false;
	bool died = false;
	
	SDL_Texture* Upleft_flipper = nullptr;
	SDL_Texture* Upright_flipper = nullptr;

	SDL_Texture* left_flipper = nullptr;
	SDL_Texture* right_flipper = nullptr;
	PhysBody* toDestroy;

	uint bonus_fx;
	uint red_circle_fx;
	uint blue_circle_fx;
	uint car_start_fx;
	uint top_blue_sensors_fx;

	char score_text[10];
	char highscore_text[10];
	char lifes_text[10];

	PhysBody* end;
	

	int score_font;
	int lifes_font;
	int maxScore_font;
};

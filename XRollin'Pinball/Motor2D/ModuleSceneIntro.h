#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> innerCircles;

	p2List<PhysBody*>* leftFlippers;
	p2List<PhysBody*>* rightFlippers;

	SDL_Texture* circle = nullptr;
	SDL_Texture* box = nullptr;
	SDL_Texture* rick = nullptr;

	SDL_Texture* map_texture = nullptr;
	SDL_Texture* ball_texture = nullptr;
	SDL_Texture* startgame_texture = nullptr;
	bool game_started = false;


	

	SDL_Texture* Upleft_flipper = nullptr;
	SDL_Texture* Upright_flipper = nullptr;

	SDL_Texture* left_flipper = nullptr;
	SDL_Texture* right_flipper = nullptr;

	uint bonus_fx;
};

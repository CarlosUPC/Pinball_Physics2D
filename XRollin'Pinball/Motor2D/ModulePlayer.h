#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	bool is_dead = false;
	b2PrismaticJoint* dock;

	uint			score = 0;
<<<<<<< HEAD
	uint			lives = 4;
=======
	uint			lives = 0;
	uint flipper_fx;
	bool flipper_pressed = false;
>>>>>>> ca1d537dc5e21b5d3913589488c72a15f243189e

private:
	float dockPosY = 0.0f;
	b2Vec2 displacement;
	
};
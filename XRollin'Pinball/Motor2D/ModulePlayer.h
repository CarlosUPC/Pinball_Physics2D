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

	uint lifes = 0;
	bool is_dead = false;
	b2PrismaticJoint* dock;

private:
	float dockPosY = 0.0f;
	b2Vec2 displacement;
	
};
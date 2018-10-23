#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateCircleSensor(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height,b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float dens, int rest, int filterIndex, b2BodyType type);
	PhysBody* CreateInnerCircle(int x, int y, int radius);
	PhysBody* CreateRotateRectangle(int x, int y, int width, int height, b2BodyType type, float angle);
	b2PrismaticJoint* CreateDock();

	// Create Flippers
	void BuildLeftFlippers(p2List<PhysBody*>* leftKickers);
	void BuildRightFlippers(p2List<PhysBody*>* RightKickers);
	
	// Move Flipper
	void FlippersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	

private:

	bool debug;
	
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2DistanceJoint* distance_joint;

	// Mechanic to rotate fippers
	b2RevoluteJoint* revolute_joint;
	b2RevoluteJoint* revolute_joint2;

public:

	b2World* world;
	b2Body* big_ball;
	PhysBody* map;
	PhysBody* map_1;
	PhysBody* map_2;
	PhysBody* map_3;
	PhysBody* map_4;
	PhysBody* map_5;
	PhysBody* map_6;
	PhysBody* map_7;
	PhysBody* map_8;
	PhysBody* map_9;
	PhysBody* map_10;

	PhysBody* dock;
	PhysBody* dockBase;
};
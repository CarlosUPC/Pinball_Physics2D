#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	revolute_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	App->scene_intro->leftFlippers = new p2List<PhysBody*>;
	App->scene_intro->rightFlippers = new p2List<PhysBody*>;

	

	int map_coords[118] = {
	0, 293,
	14, 276,
	14, 250,
	32, 203,
	3, 104,
	1, 77,
	1, 24,
	4, 15,
	12, 9,
	23, 9,
	33, 16,
	35, 23,
	36, 45,
	37, 63,
	38, 76,
	39, 89,
	46, 112,
	49, 110,
	42, 89,
	37, 26,
	37, 13,
	38, 6,
	46, 2,
	56, 7,
	57, 21,
	59, 49,
	63, 68,
	72, 87,
	74, 86,
	65, 66,
	61, 49,
	62, 27,
	69, 13,
	79, 4,
	89, 0,
	162, 0,
	184, 5,
	203, 16,
	217, 29,
	228, 44,
	235, 60,
	241, 76,
	241, 580,
	224, 580,
	223, 186,
	187, 267,
	207, 293,
	207, 375,
	136, 419,
	136, 498,
	110, 498,
	110, 420,
	107, 415,
	100, 415,
	97, 420,
	97, 498,
	72, 498,
	72, 420,
	1, 375
	};

	// Element Orange 1
	int Map_1[20] = {
		158, 22,
		158, 42,
		214, 81,
		215, 75,
		212, 66,
		204, 53,
		194, 40,
		182, 30,
		169, 23,
		160, 21
	};
	// Element Orange 2
	int Map_2[32] = {
		213, 116,
		192, 158,
		198, 164,
		198, 168,
		182, 189,
		176, 190,
		169, 221,
		159, 249,
		162, 251,
		182, 217,
		198, 190,
		204, 180,
		208, 169,
		212, 157,
		214, 148,
		215, 119
	};

	// Element Orange 3
	int Map_3[66] = {
		152, 195,
		130, 188,
		125, 185,
		124, 175,
		35, 131,
		26, 107,
		22, 87,
		20, 29,
		16, 29,
		18, 87,
		22, 110,
		53, 195,
		63, 197,
		60, 177,
		62, 173,
		69, 173,
		76, 177,
		81, 182,
		84, 201,
		88, 206,
		116, 222,
		122, 221,
		125, 204,
		128, 200,
		136, 200,
		145, 208,
		144, 220,
		138, 236,
		138, 241,
		142, 240,
		148, 221,
		149, 212,
		153, 200
	};

	// Element Red 1
	int Map_4_red[14] = {
		40, 297,
		35, 297,
		34, 339,
		55, 351,
		60, 351,
		61, 346,
		41, 301
	};

	//Element Red 2
	int Map_5_red[14] = {
		172, 297,
		167, 297,
		145, 346,
		146, 351,
		151, 352,
		173, 338,
		173, 301
	};

	// Element stick 1
	int Map_6_stick[10] = {
		103, 21,
		107, 21,
		107, 42,
		103, 42,
		103, 23
	};

	// Element stick 2
	int Map_7_stick[10] = {
		121, 19,
		125, 19,
		125, 40,
		121, 40,
		121, 22
	};

	// Element stick 3
	int Map_8_stick[10] = {
		139, 18,
		143, 18,
		143, 40,
		139, 40,
		139, 21
	};

	// Element stickL 1
	int Map_9_stickL[16] = {
		19, 307,
		15, 307,
		15, 352,
		54, 376,
		58, 376,
		58, 372,
		20, 349,
		20, 311
	};

	// Element stickL 2
	int Map_10_stickL[14] = {
		188, 308,
		192, 308,
		192, 352,
		152, 377,
		148, 373,
		188, 349,
		188, 311
	};
	map = CreateChain(0, 0, map_coords, 118);
	
	map_1 = CreateChain(0, 0, Map_1, 20);
	map_2 = CreateChain(0, 0, Map_2, 32);
	map_3 = CreateChain(0, 0, Map_3, 66);
	map_4 = CreateChain(0, 0, Map_4_red, 14);
	map_5 = CreateChain(0, 0, Map_5_red, 14);
	map_6 = CreateChain(0, 0, Map_6_stick, 10);
	map_7 = CreateChain(0, 0, Map_7_stick, 10);
	map_8 = CreateChain(0, 0, Map_8_stick, 10);
	map_9 = CreateChain(0, 0, Map_9_stickL, 16);
	map_10 = CreateChain(0, 0, Map_10_stickL, 14);
	
	map->body->SetType(b2_staticBody);

	map_1->body->SetType(b2_staticBody);
	map_2->body->SetType(b2_staticBody);
	map_3->body->SetType(b2_staticBody);
	map_4->body->SetType(b2_staticBody);
	map_5->body->SetType(b2_staticBody);
	map_6->body->SetType(b2_staticBody);
	map_7->body->SetType(b2_staticBody);
	map_8->body->SetType(b2_staticBody);
	map_9->body->SetType(b2_staticBody);
	map_10->body->SetType(b2_staticBody);
	

	BuildLeftFlippers(App->scene_intro->leftFlippers);
	BuildRightFlippers(App->scene_intro->rightFlippers);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 5.0f;
	fixture.restitution = 0.01f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateInnerCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 1.5f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height,b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 25.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;


	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, float dens, int rest, int filterIndex, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape shape;

	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = dens;
	fixture.restitution = rest;
	fixture.filter.groupIndex = filterIndex;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = 0;
	pbody->height = 0;

	return pbody;
}

void ModulePhysics::BuildLeftFlippers(p2List<PhysBody*>* leftFlippers)
{
	//Flipper Lower
	PhysBody* flipper = CreateRectangle(111, 382, 30, 7, b2_dynamicBody); 
	PhysBody* gear = CreateRectangle(68, 382, 1, 1, b2_staticBody); 

	//Flipper Upper
	PhysBody* flipper2 = CreateRectangle(120, 180, 25, 7, b2_dynamicBody);
	PhysBody* gear2 = CreateRectangle(130, 180, 1, 1, b2_staticBody);

	//Revolute Joint Flipper Lower
	b2RevoluteJointDef revolute_def;

	revolute_def.bodyA = gear->body;
	revolute_def.bodyB = flipper->body;

	revolute_def.localAnchorB = b2Vec2(-0.25, 0.0);
	revolute_def.enableLimit = true;
	revolute_def.lowerAngle = -(0.1);
	revolute_def.upperAngle = (0.5);
	revolute_def.collideConnected = false;

	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolute_def);
	leftFlippers->add(flipper);

	//Revolute Joint Flipper Upper
	b2RevoluteJointDef revolute_def2;

	revolute_def2.bodyA = gear2->body;
	revolute_def2.bodyB = flipper2->body;

	revolute_def2.localAnchorB = b2Vec2(-0.25, 0.0);
	revolute_def2.enableLimit = true;
	revolute_def2.lowerAngle = -(0.2);
	revolute_def2.upperAngle = (0.3);
	revolute_def2.collideConnected = false;

	revolute_joint2 = (b2RevoluteJoint*)world->CreateJoint(&revolute_def2);
	leftFlippers->add(flipper2);

} 	

void ModulePhysics::BuildRightFlippers(p2List<PhysBody*>* rightFlippers)
{
	//Flipper Lower
	PhysBody* flipper = CreateRectangle(160, 382, 30, 7, b2_dynamicBody);
	PhysBody* gear = CreateRectangle(140, 382, 1, 1, b2_staticBody);

	//Flipper Upper
	PhysBody* flipper2 = CreateRectangle(181, 166, 25, 7, b2_dynamicBody);
	PhysBody* gear2 = CreateRectangle(191, 166, 1, 1, b2_staticBody);

	//Revolute Joint Flipper Lower
	b2RevoluteJointDef revolute_def;
	
	revolute_def.bodyA = gear->body;
	revolute_def.bodyB = flipper->body;

	revolute_def.localAnchorB = b2Vec2(0.25, 0.0);
	revolute_def.enableLimit = true;
	revolute_def.lowerAngle = -(0.5);
	revolute_def.upperAngle = (0.1);
	revolute_def.collideConnected = false;

	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolute_def);
	rightFlippers->add(flipper);

	//Revolute Joint Flipper Upper
	b2RevoluteJointDef revolute_def2;

	revolute_def2.bodyA = gear2->body;
	revolute_def2.bodyB = flipper2->body;

	revolute_def2.localAnchorB = b2Vec2(0.25, 0.0);
	revolute_def2.enableLimit = true;
	revolute_def2.lowerAngle = -(0.95);
	revolute_def2.upperAngle = -(0.25);
	revolute_def2.collideConnected = false;

	revolute_joint2 = (b2RevoluteJoint*)world->CreateJoint(&revolute_def2);
	rightFlippers->add(flipper2);
}

void ModulePhysics::FlippersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl) {


	if (rl == LEFT)
	{
		p2List_item<PhysBody*>* item = App->scene_intro->leftFlippers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
	}
	else if (rl == RIGHT) {
		p2List_item<PhysBody*>* item = App->scene_intro->rightFlippers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
	}
}

b2PrismaticJoint* ModulePhysics::CreateDock() {

	dock = App->physics->CreateRectangle(232, 408, 14, 10, b2_dynamicBody);
	dockBase = App->physics->CreateRectangle(232, 440, 10, 10, b2_staticBody);

	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.Initialize(dock->body, dockBase->body, dockBase->body->GetWorldCenter(), b2Vec2(0, 1));
	prismaticJointDef.collideConnected = true;

	prismaticJointDef.lowerTranslation = -0.25f;
	prismaticJointDef.upperTranslation = 0.25f;
	prismaticJointDef.enableLimit = true;

	prismaticJointDef.maxMotorForce = 250.0f;
	prismaticJointDef.motorSpeed = 150.0f;
	prismaticJointDef.enableMotor = true;

	return (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJointDef);
}


update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	b2Body* body_clicked = nullptr;
	b2Vec2 mouse_position = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 0, 0);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && b != big_ball) {

				if (f->TestPoint(mouse_position))
					body_clicked = b;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property

	if (body_clicked != nullptr) {

		// Mouse joint
		b2MouseJointDef mouse_def;

		mouse_def.bodyA = ground;
		mouse_def.bodyB = body_clicked;
		mouse_def.target = mouse_position;
		mouse_def.dampingRatio = 0.5f;
		mouse_def.frequencyHz = 2.0f;
		mouse_def.maxForce = 100.0f * body_clicked->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&mouse_def);

		// Distance joint
		b2DistanceJointDef distance_def;

		distance_def.Initialize(big_ball, body_clicked, big_ball->GetWorldCenter(), body_clicked->GetWorldCenter());
		distance_def.dampingRatio = 0.5f;
		distance_def.frequencyHz = 2.0f;

		distance_joint = (b2DistanceJoint*)world->CreateJoint(&distance_def);

		
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr) {

		b2Vec2 anchorA = mouse_joint->GetBodyB()->GetPosition();

		mouse_joint->SetTarget(mouse_position);
		b2Vec2 anchorB = mouse_joint->GetTarget();

		App->renderer->DrawLine(METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y), METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y), 255, 0, 0);
	}

	// Draw distance joint

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && distance_joint != nullptr) {

		b2Vec2 anchorA = distance_joint->GetBodyA()->GetPosition();
		b2Vec2 anchorB = distance_joint->GetBodyB()->GetPosition();

		App->renderer->DrawLine(METERS_TO_PIXELS(anchorA.x), METERS_TO_PIXELS(anchorA.y), METERS_TO_PIXELS(anchorB.x), METERS_TO_PIXELS(anchorB.y), 255, 0, 0);
	}

	// TODO 4: If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP) {

		if (mouse_joint != nullptr) {
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}

		// Uncomment if you want to maintain the distance joint
		
		if (distance_joint != nullptr) {
			world->DestroyJoint(distance_joint);
			distance_joint = nullptr;
		}
		

		
	}
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

PhysBody* ModulePhysics::CreateRotateRectangle(int x, int y, int width, int height, b2BodyType type, float angle)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b->SetTransform(b->GetPosition(), angle);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 25.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
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


	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	//Bouncer1 = CreateCircle();
	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;
	
	int PinBall_Board[14] = {
		477, 872,
		507, 874,
		507, 17,
		25, 17,
		27, 871,
		467, 871,
		477, 872
	};

	//CreateChain(-25, -17, PinBall_Board, 13, b2_staticBody, 0.0f);

	int PinBall_Right_Part[64] = {
		477, 872,
		349, 872,
		475, 788,
		475, 618,
		472, 607,
		463, 597,
		453, 587,
		445, 576,
		444, 565,
		446, 557,
		451, 548,
		456, 543,
		461, 536,
		460, 527,
		456, 500,
		445, 466,
		444, 456,
		448, 447,
		454, 440,
		454, 431,
		447, 422,
		439, 414,
		433, 400,
		437, 378,
		441, 357,
		446, 332,
		452, 303,
		471, 217,
		480, 188,
		480, 344,
		480, 643,
		480, 873
	};

	CreateChain(-25, -17, PinBall_Right_Part, 63, b2_staticBody, 0.5f);

	int Inside_1[220] = {
		130, 853,
		37, 790,
		32, 791,
		33, 835,
		24, 836,
		20, 836,
		13, 834,
		4, 830,
		4, 819,
		3, 604,
		3, 595,
		6, 589,
		10, 581,
		14, 574,
		18, 570,
		21, 564,
		25, 560,
		30, 555,
		33, 549,
		36, 542,
		36, 534,
		30, 523,
		23, 516,
		14, 508,
		7, 497,
		12, 487,
		16, 480,
		48, 419,
		50, 410,
		51, 400,
		50, 388,
		39, 334,
		27, 287,
		15, 240,
		11, 228,
		10, 215,
		8, 199,
		6, 180,
		5, 162,
		4, 138,
		7, 120,
		10, 97,
		15, 80,
		27, 62,
		40, 42,
		52, 35,
		69, 20,
		81, 14,
		96, 10,
		110, 7,
		123, 6,
		160, 6,
		180, 11,
		192, 23,
		196, 38,
		198, 53,
		194, 65,
		179, 75,
		166, 80,
		150, 88,
		139, 94,
		129, 101,
		128, 110,
		129, 124,
		130, 133,
		135, 139,
		154, 227,
		158, 232,
		169, 229,
		181, 224,
		188, 222,
		201, 218,
		207, 215,
		206, 189,
		191, 192,
		176, 190,
		166, 183,
		155, 177,
		146, 164,
		141, 149,
		143, 132,
		145, 121,
		157, 108,
		173, 98,
		188, 93,
		194, 95,
		207, 95,
		207, 64,
		206, 49,
		213, 32,
		222, 20,
		233, 13,
		253, 5,
		274, 4,
		290, 5,
		380, 4,
		393, 6,
		406, 10,
		420, 16,
		437, 25,
		450, 40,
		457, 51,
		465, 70,
		467, 87,
		467, 95,
		480, 133,
		478, 0,
		0, 0,
		0, 852,
		129, 854
	};

	CreateChain(0, 0, Inside_1, 219, b2_staticBody, 0.5f);

	int Inside_2[98] = {
		265, 48,
		244, 48,
		239, 49,
		238, 54,
		238, 97,
		235, 113,
		230, 117,
		221, 120,
		204, 123,
		193, 125,
		182, 129,
		175, 137,
		175, 147,
		177, 154,
		181, 158,
		192, 160,
		204, 162,
		231, 167,
		238, 179,
		239, 230,
		237, 235,
		232, 238,
		162, 262,
		174, 325,
		176, 330,
		179, 334,
		185, 336,
		252, 360,
		257, 361,
		260, 361,
		262, 360,
		260, 355,
		253, 346,
		247, 337,
		245, 331,
		243, 323,
		243, 201,
		251, 184,
		244, 180,
		239, 173,
		234, 163,
		233, 151,
		235, 142,
		238, 133,
		244, 129,
		253, 123,
		266, 122,
		267, 55,
		265, 48
	};

	CreateChain(0, 0, Inside_2, 97, b2_staticBody, 0.5f);

	int Canon[12] = {
		480, 855,
		480, 130,
		480, 854,
		450, 854,
		450, 179,
		450, 854
	};

	CreateChain(0, 0, Canon, 12, b2_staticBody, 0.0f);

	int left_slide[18] = {
		33, 628,
		38, 628,
		39, 723,
		139, 791,
		132, 795,
		40, 735,
		34, 728,
		33, 718,
		33, 628
	};

	CreateChain(0, 0, left_slide, 17, b2_staticBody, 0.0f);

	int rigth_sile[28] = {
		317, 791,
		325, 797,
		414, 737,
		419, 733,
		422, 729,
		423, 707,
		423, 680,
		423, 658,
		423, 625,
		420, 625,
		420, 717,
		415, 724,
		403, 733,
		317, 791
	};

	CreateChain(0, 0, rigth_sile, 27, b2_staticBody, 0.0f);

	int canon_box[10] = {
		480, 829,
		451, 829,
		451, 855,
		480, 855,
		480, 829
	};

	CreateChain(0, 0, canon_box, 9, b2_staticBody, 0.0f);

	int left_small_block[32] = {
		299, 228,
		280, 317,
		279, 323,
		278, 326,
		275, 326,
		273, 323,
		272, 319,
		272, 234,
		274, 229,
		276, 227,
		278, 225,
		281, 224,
		288, 224,
		293, 224,
		297, 224,
		299, 228
	};

	CreateChain(0, 0, left_small_block, 31, b2_staticBody, 0.4f);

	int right_small_block[14] = {
		346, 249,
		372, 276,
		375, 281,
		374, 285,
		348, 355,
		335, 351,
		345, 248
	};

	CreateChain(0, 0, right_small_block, 13, b2_staticBody, 0.4f);

	int long_block[46] = {
		108, 357,
		123, 352,
		104, 179,
		94, 82,
		96, 66,
		99, 57,
		101, 54,
		100, 51,
		96, 51,
		84, 59,
		66, 72,
		57, 82,
		51, 91,
		46, 102,
		43, 112,
		42, 128,
		43, 149,
		47, 172,
		57, 210,
		72, 254,
		81, 280,
		97, 328,
		108, 358
	};

	CreateChain(0, 0, long_block, 45, b2_staticBody, 0.4f);

	int right_long_block[76] = {
		375, 195,
		375, 202,
		377, 210,
		380, 215,
		383, 228,
		386, 239,
		388, 242,
		392, 241,
		396, 223,
		411, 186,
		423, 153,
		429, 133,
		433, 119,
		435, 106,
		435, 94,
		433, 79,
		427, 64,
		419, 55,
		407, 46,
		391, 39,
		382, 40,
		376, 43,
		373, 51,
		372, 64,
		374, 75,
		380, 82,
		384, 91,
		384, 101,
		397, 109,
		397, 120,
		397, 131,
		397, 150,
		397, 167,
		395, 175,
		389, 181,
		381, 187,
		377, 190,
		375, 195
	};

	CreateChain(0, 0, right_long_block, 75, b2_staticBody, 0.4f);

	int tiny_block_1[18] = {
		274, 97,
		274, 78,
		272, 74,
		270, 74,
		268, 76,
		268, 98,
		270, 100,
		272, 100,
		274, 97
	};

	CreateChain(0, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	CreateChain(33, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	CreateChain(67, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	CreateChain(100, 0, tiny_block_1, 17, b2_staticBody, 0.2f);


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

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type, float restitution, bool sensor)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b->SetBullet(true);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;
	fixture.isSensor = sensor;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

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

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type, float restitution)
{
	b2BodyDef body;
	body.type = type;
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
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;
	
	b2Body* body_found = nullptr;
	b2Vec2 mouse_position;
	mouse_position.x = PIXEL_TO_METERS(App->input->GetMouseX());
	mouse_position.y = PIXEL_TO_METERS(App->input->GetMouseY());

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
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if(f->TestPoint(mouse_position))
				{ 
					body_found = b;
				}
				
			}
			// test if the current body contains mouse position
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (body_found != nullptr)
	{
		LOG("Creating Join");
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_found;
		def.target = mouse_position;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f*body_found->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT)==KEY_REPEAT && mouse_joint!=nullptr)
	{
		mouse_joint->SetTarget(mouse_position);
		//App->renderer->DrawLine(mouse_position.x, mouse_position.y, body_found->GetPosition().x, body_found->GetPosition().y,255,0,0);
	}
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		body_found = nullptr;
	}

	// TODO 4: If the player releases the mouse button, destroy the joint

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
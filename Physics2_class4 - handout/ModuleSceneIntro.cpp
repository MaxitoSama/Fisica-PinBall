#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	open = false;
	canon_shoot = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	b2RevoluteJointDef def_1;
	b2RevoluteJointDef def_2;
	b2RevoluteJoint* joint_1;
	b2RevoluteJoint* joint_2;

	App->renderer->camera.x = App->renderer->camera.y = 0;


	//--------------------------Image sources------------------------------------------------

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	BackGround= App->textures->Load("pinball/PinBall_Board2.png");
	RightFlipper = App->textures->Load("pinball/right flipper.png");
	LeftFlipper = App->textures->Load("pinball/left flipper.png");
	RightBouncer = App->textures->Load("pinball/right block.png");
	LeftBouncer = App->textures->Load("pinball/left block.png");
	LetterW = App->textures->Load("pinball/w circle.png");
	LetterI = App->textures->Load("pinball/i circle.png");
	LetterN = App->textures->Load("pinball/n circle.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	click = App->audio->LoadFx("pinball/Target2.wav");

	//sensor = App->physics->CreateRectangleSensor(455+10, 834+5, 25, 21);


	//--------------------------Scene Elements------------------------------------------------
	
	switch_1 = App->physics->CreateCircle(13, 476, 10, b2_staticBody, 0.0f, true);
	switch_2 = App->physics->CreateCircle(27, 448, 10, b2_staticBody, 0.0f, true);
	switch_3 = App->physics->CreateCircle(40, 426, 10, b2_staticBody, 0.0f, true);
	switch_4 = App->physics->CreateCircle(116,355, 5, b2_staticBody, 0.0f, true);
	switch_5 = App->physics->CreateCircle(206, 342, 6, b2_staticBody, 0.0f, true);
	switch_6 = App->physics->CreateCircle(235, 352, 6, b2_staticBody, 0.0f, true);
	switch_7 = App->physics->CreateCircle(344, 351, 5, b2_staticBody, 0.0f, true);
	switch_8 = App->physics->CreateCircle(428, 471, 6, b2_staticBody, 0.0f, true);
	switch_9 = App->physics->CreateCircle(436, 501, 6, b2_staticBody, 0.0f, true);

	switch_1->listener = this;
	switch_2->listener = this;
	switch_3->listener = this;
	switch_4->listener = this;
	switch_5->listener = this;
	switch_6->listener = this;
	switch_7->listener = this;
	switch_8->listener = this;
	switch_9->listener = this;

	bouncer_1 = new PhysBody();
	bouncer_1 = App->physics->CreateCircle(267, 155, 22, b2_staticBody, 2.0f);
	B_1sensor = App->physics->CreateCircle(267, 155, 24, b2_staticBody, 2.0f,true);
	B_1sensor->listener = this;
	
	bouncer_2 = new PhysBody();
	bouncer_2 = App->physics->CreateCircle(370, 162, 22, b2_staticBody, 2.0f);
	B_2sensor = App->physics->CreateCircle(370, 162, 24, b2_staticBody, 2.0f, true);
	B_2sensor->listener = this;
	
	bouncer_3 = new PhysBody();
	bouncer_3 = App->physics->CreateCircle(306, 223, 22, b2_staticBody, 2.0f);
	B_3sensor = App->physics->CreateCircle(306, 223, 24, b2_staticBody, 2.0f, true);
	B_3sensor->listener = this;

	slide_block = App->physics->CreateRectangle(253, 26, 5, 31, b2_staticBody);
	cannon_block = App->physics->CreateRectangle(461, 150, 100, 10, b2_staticBody);
	cannon_block->body->SetTransform(cannon_block->body->GetWorldCenter(), -78 * 0.0174532925);

	sensorblocker_w = new PhysBody();
	sensorblocker_w = App->physics->CreateRectangleSensor(290, 110, 20, 1);
	sensorblocker_w->listener = this;

	sensorblocker_i = new PhysBody();
	sensorblocker_i = App->physics->CreateRectangleSensor(323, 110, 20, 1);
	sensorblocker_i->listener = this;

	sensorblocker_n = new PhysBody();
	sensorblocker_n = App->physics->CreateRectangleSensor(356, 110, 20, 1);
	sensorblocker_n->listener = this;
	
	canon_sensor = App->physics->CreateRectangleSensor(17, 845, 30, 30);
	canon_sensor->listener = this;
	
	int left_block[22] = {
		11, 16,
		11, 105,
		53, 134,
		56, 135,
		62, 135,
		69, 129,
		69, 122,
		32, 13,
		28, 10,
		17, 9,
		11, 16,
	};
	
	leftbouncer.add(App->physics->CreateChain(51, 600, left_block, 21, b2_staticBody, 0.3f));

	int left_block_bouncer[10] = {
		372, 610,
		334, 722,
		348, 723,
		380, 616,
		372, 610
	};
	
	App->physics->CreateChain(0, 0, left_block_bouncer, 9, b2_staticBody, 1.6f);


	int right_block[20] = {
		61, 10,
		50, 9,
		44, 15,
		9, 123,
		9, 129,
		16, 135,
		22, 135,
		67, 106,
		67, 16,
		61, 9
	};
	
	rightbouncer.add(App->physics->CreateChain(325, 600, right_block, 19, b2_staticBody, 0.3f));

	int right_block_bouncer[10] = {
		121, 723,
		83, 612,
		74, 616,
		107, 724,
		121, 723
	};
	
	App->physics->CreateChain(0, 0, right_block_bouncer, 9, b2_staticBody, 1.6f);
	
	//----------------------------------------------------------------------------------------
	
	
	rightflipper = App->physics->CreateRectangle(250+30, 790+15, 77,14, b2_dynamicBody);
	leftflipper=App->physics->CreateRectangle(140+41, 790+18, 77,14, b2_dynamicBody);

	l_flipper_joint =App->physics->CreateCircle(144+9, 800+8, 5, b2_staticBody, 0.0f);
	r_flipper_joint =App->physics->CreateCircle(300 + 7, 800 + 3, 5, b2_staticBody, 0.0f);

	def_1.Initialize(leftflipper->body, l_flipper_joint->body, l_flipper_joint->body->GetWorldCenter());
	def_2.Initialize(r_flipper_joint->body, rightflipper->body, r_flipper_joint->body->GetWorldCenter());
	
	def_1.enableLimit = true;
	def_2.enableLimit = true;
	
	def_1.lowerAngle = -30 * DEGTORAD;
	def_1.upperAngle = 30 * DEGTORAD;
	def_2.lowerAngle = -30 * DEGTORAD;
	def_2.upperAngle = 30 * DEGTORAD;

	joint_1 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def_1);
	joint_2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&def_2);

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

	Chain1 = App->physics->CreateChain(0, 0, Inside_1, 219, b2_staticBody, 0.5f);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(BackGround, 0, 0, NULL);
	
	if (w_passed == true)
	{
		App->renderer->Blit(LetterW, 271, 96, NULL);
	}

	if (i_passed == true)
	{
		App->renderer->Blit(LetterI, 304, 94, NULL);
	}

	if (n_passed == true)
	{
		App->renderer->Blit(LetterN, 335, 96, NULL);
	}

	if (w_passed == true && i_passed == true && n_passed == true)
	{
		App->player->score += 100000;
		w_passed = false;
		i_passed == false;
		n_passed == false;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11, b2_dynamicBody, 0.0f));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50,b2_dynamicBody));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64,b2_dynamicBody, 0.0f));
	
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
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

	c = leftbouncer.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(LeftBouncer, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = rightbouncer.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(RightBouncer, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (leftflipper != NULL)
	{
		int x, y;
		leftflipper->GetPosition(x, y);
		App->renderer->Blit(LeftFlipper, x, y, NULL, 1.0f, leftflipper->GetRotation());
	}


	if (rightflipper != NULL)
	{
		int x, y;
		rightflipper->GetPosition(x, y);
		App->renderer->Blit(RightFlipper, x, y, NULL, 1.0f, rightflipper->GetRotation());
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	if (open==true && slide_block->body!=nullptr)
	{
		slide_block->body->GetWorld()->DestroyBody(slide_block->body);
		slide_block->body = nullptr;
		open = false;
		cannon_block->body->SetActive(true);
	}

	if (canon_shoot == true)
	{
		if (SDL_GetTicks() >= ticks)
		{
			App->audio->PlayFx(App->player->drain);
			App->player->Ball->body->ApplyForce({ 0.0f,-200.0f },App->player->Ball->body->GetLocalCenter(),true);
			LOG("force!");
			canon_shoot = false;
		}
	}

	char score[64];
	char lives[4];
	char Title[64] = "PinBall Score: ";
	char Num_lives[32] = "Lives: ";
	sprintf_s(score, "%d   ", App->player->score);
	sprintf_s(lives, "%d", App->player->lives);
	strcat_s(Title, score);
	strcat_s(Title, Num_lives);
	strcat_s(Title, lives);

	App->window->SetTitle(Title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	b2Vec2 stop = { 0.0f,0.0f };

	if(bodyA)
	{
		if (bodyA == sensorblocker_w || bodyA == sensorblocker_i || bodyA == sensorblocker_n && slide_block->body != nullptr)
		{
			open = true;
		}

		if (bodyA == sensorblocker_w)
		{
			w_passed = true;
		}
		if (bodyA == sensorblocker_i)
		{
			i_passed = true;
		}
		if (bodyA == sensorblocker_n)
		{
			n_passed = true;
		}

		if (App->player->getpoints==false && (bodyA == B_1sensor || bodyA == B_2sensor || bodyA == B_3sensor))
		{
			App->audio->PlayFx(bonus_fx);
			LOG("puntos");
			App->player->getpoints = true;
		}

		if (bodyA == canon_sensor && canon_shoot == false && App->player->Ball->body->GetLinearVelocity()==stop)
		{
			canon_shoot = true;
			Timer(1000);
		}
		if (bodyA == switch_1 || bodyA == switch_2 || bodyA == switch_3 || bodyA == switch_4 || bodyA == switch_5
			|| bodyA == switch_6 || bodyA == switch_7 || bodyA == switch_8 || bodyA == switch_9)
		{
			App->audio->PlayFx(click);
		}
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}

void ModuleSceneIntro::Timer(int time)
{
	ticks = SDL_GetTicks() + time;
}
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

	circle = App->textures->Load("assets/wheel.png"); 
	box = App->textures->Load("assets/crate.png");
	rick = App->textures->Load("assets/rick_head.png");
	BackGround= App->textures->Load("assets/PinBall_Board2.png");
	RightFlipper = App->textures->Load("assets/right flipper.png");
	LeftFlipper = App->textures->Load("assets/left flipper.png");
	RightBouncer = App->textures->Load("assets/right block.png");
	LeftBouncer = App->textures->Load("assets/left block.png");
	LetterW = App->textures->Load("assets/w circle.png");
	LetterI = App->textures->Load("assets/i circle.png");
	LetterN = App->textures->Load("assets/n circle.png");
	million_1 = App->textures->Load("assets/1 million.png");
	million_2 = App->textures->Load("assets/2 million.png");
	million_3 = App->textures->Load("assets/3 million.png");
	million_4 = App->textures->Load("assets/4 million.png");
	arrow1 = App->textures->Load("assets/arrow1.png");
	arrow2 = App->textures->Load("assets/arrow2.png");
	arrow3 = App->textures->Load("assets/arrow3.png");
	arrow4 = App->textures->Load("assets/arrow4.png");
	arrow5 = App->textures->Load("assets/arrow5.png");
	arrow6 = App->textures->Load("assets/arrow6.png");
	arrow7 = App->textures->Load("assets/arrow7.png");
	arrow8 = App->textures->Load("assets/arrow8.png");
	arrow9 = App->textures->Load("assets/arrow9.png");

	bonus_fx = App->audio->LoadFx("sounds/bonus.wav");
	click = App->audio->LoadFx("sounds/Target2.wav");
	App->audio->PlayMusic("sounds/music.ogg");

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
	bouncer_1 = App->physics->CreateCircle(267, 155, 22, b2_staticBody, 1.7f);
	B_1sensor = App->physics->CreateCircle(267, 155, 24, b2_staticBody, 2.0f,true);
	B_1sensor->listener = this;
	
	bouncer_2 = new PhysBody();
	bouncer_2 = App->physics->CreateCircle(370, 162, 22, b2_staticBody, 1.7f);
	B_2sensor = App->physics->CreateCircle(370, 162, 24, b2_staticBody, 2.0f, true);
	B_2sensor->listener = this;
	
	bouncer_3 = new PhysBody();
	bouncer_3 = App->physics->CreateCircle(306, 223, 22, b2_staticBody, 1.3f);
	B_3sensor = App->physics->CreateCircle(306, 223, 24, b2_staticBody, 2.0f, true);
	B_3sensor->listener = this;

	slide_block = App->physics->CreateRectangle(253, 26, 5, 31, b2_staticBody);
	cannon_block = App->physics->CreateRectangle(461, 150, 100, 10, b2_staticBody);
	cannon_block->body->SetTransform(cannon_block->body->GetWorldCenter(), -78 * 0.0174532925);

	sensorblocker_w = new PhysBody();
	sensorblocker_w = App->physics->CreateRectangleSensor(290, 90, 20, 1);
	sensorblocker_w->listener = this;

	sensorblocker_i = new PhysBody();
	sensorblocker_i = App->physics->CreateRectangleSensor(323, 90, 20, 1);
	sensorblocker_i->listener = this;

	sensorblocker_n = new PhysBody();
	sensorblocker_n = App->physics->CreateRectangleSensor(356, 90, 20, 1);
	sensorblocker_n->listener = this;
	
	canon_sensor = App->physics->CreateRectangleSensor(17, 845, 30, 30);
	canon_sensor->listener = this;
	
	Millionstart = App->physics->CreateRectangleSensor(170, 120, 35, 1);;
	Millionstart->listener = this;

	Millionend = App->physics->CreateRectangleSensor(220, 220, 35, 1);
	Millionend->listener = this;

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

	App->physics->CreateChain(-25, -17, PinBall_Right_Part, 63, b2_staticBody, 0.5f);

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

	App->physics->CreateChain(0, 0, Inside_2, 97, b2_staticBody, 0.5f);

	int Canon[12] = {
		480, 855,
		480, 130,
		480, 854,
		450, 854,
		450, 179,
		450, 854
	};

	App->physics->CreateChain(0, 0, Canon, 12, b2_staticBody, 0.0f);

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

	App->physics->CreateChain(0, 0, left_slide, 17, b2_staticBody, 0.0f);

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

	App->physics->CreateChain(0, 0, rigth_sile, 27, b2_staticBody, 0.0f);

	int canon_box[10] = {
		480, 829,
		451, 829,
		451, 855,
		480, 855,
		480, 829
	};

	App->physics->CreateChain(0, 0, canon_box, 9, b2_staticBody, 0.0f);

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

	App->physics->CreateChain(0, 0, left_small_block, 31, b2_staticBody, 0.4f);

	int right_small_block[14] = {
		346, 249,
		372, 276,
		375, 281,
		374, 285,
		348, 355,
		335, 351,
		345, 248
	};

	App->physics->CreateChain(0, 0, right_small_block, 13, b2_staticBody, 0.4f);

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

	App->physics->CreateChain(0, 0, long_block, 45, b2_staticBody, 0.4f);

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

	App->physics->CreateChain(0, 0, right_long_block, 75, b2_staticBody, 0.4f);

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

	App->physics->CreateChain(0, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	App->physics->CreateChain(33, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	App->physics->CreateChain(67, 0, tiny_block_1, 17, b2_staticBody, 0.2f);

	App->physics->CreateChain(100, 0, tiny_block_1, 17, b2_staticBody, 0.2f);
	
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
	
	LbouncerSensor= App->physics->CreateChainSensor(-2, 0, left_block_bouncer, 9);
	LbouncerSensor->listener = this;

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
	
	RbouncerSensor = App->physics->CreateChainSensor(2, 0, right_block_bouncer, 9);
	RbouncerSensor->listener = this;
	
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

	millioncount = 0;

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
		i_passed = false;
		n_passed = false;
	}

	if (millioncount>=1)
	{
		App->renderer->Blit(million_1, 165, 248, NULL);
	
	}
	
	if (millioncount >= 2)
	{
		App->renderer->Blit(million_2, 170, 266, NULL);
	}
	
	if (millioncount >= 3)
	{
		App->renderer->Blit(million_3, 172, 285, NULL);
	}
	
	if (millioncount >= 4)
	{
		App->renderer->Blit(million_4, 172, 305, NULL);
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
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

	if (arrow_1 == true)
	{
		App->renderer->Blit(arrow1, 27, 478);
	}

	if (arrow_2 == true)
	{
		App->renderer->Blit(arrow2, 39, 451);
	}

	if (arrow_3 == true)
	{
		App->renderer->Blit(arrow3, 53, 429);
	}

	if (arrow_4 == true)
	{
		App->renderer->Blit(arrow4, 102, 361);
	}

	if (arrow_5 == true)
	{
		App->renderer->Blit(arrow5, 167, 354);
	}

	if (arrow_6 == true)
	{
		App->renderer->Blit(arrow6, 199, 365);
	}
	if (arrow_7 == true)
	{
		App->renderer->Blit(arrow7, 302, 361);
	}
	if (arrow_8 == true)
	{
		App->renderer->Blit(arrow8, 367, 456);
	}
	if (arrow_9 == true)
	{
		App->renderer->Blit(arrow9, 376, 487);
	}

	if (million1 == true && million2 == true)
	{
		million1 = false;
		million2 = false;
		if (millioncount < 4)
		{
			millioncount += 1;
			App->player->score += 1000000 * millioncount;
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

		if (bodyA == Millionstart && million1 != true)
		{
			million1 = true;
		}

		if (bodyA == Millionend && million1 == true && million2 != true)
		{
			million2 = true;
		}
		

		if (App->player->getpoints1==false && (bodyA == B_1sensor || bodyA == B_2sensor || bodyA == B_3sensor))
		{
			App->audio->PlayFx(bonus_fx);
			App->player->getpoints1 = true;
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

			if (bodyA == switch_1)
			{
				arrow_1 = true;
			}

			if (bodyA == switch_2)
			{
				arrow_2 = true;
			}

			if (bodyA == switch_3)
			{
				arrow_3 = true;
			}

			if (bodyA == switch_4)
			{
				arrow_4 = true;
			}

			if (bodyA == switch_5)
			{
				arrow_5 = true;
			}

			if (bodyA == switch_6)
			{
				arrow_6 = true;
			}
			if (bodyA == switch_7)
			{
				arrow_7 = true;
			}
			if (bodyA == switch_8)
			{
				arrow_8 = true;
			}
			if (bodyA == switch_9)
			{
				arrow_9 = true;
			}
		}

		if (App->player->getpoints2==false &&(bodyA == LbouncerSensor || bodyB == RbouncerSensor))
		{
			App->audio->PlayFx(bonus_fx);
			App->player->getpoints2 = true;
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
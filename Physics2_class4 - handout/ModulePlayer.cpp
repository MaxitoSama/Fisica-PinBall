#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//TEXTURES----------------------
	Spring = App->textures->Load("assets/PinBall_Spring.png");
	ball_texture = App->textures->Load("assets/Ball.png");
	blocker_texture = App->textures->Load("assets/slide_blocker.png");
	gameover = App->textures->Load("assets/Game_over.png");
	
	flipperUp =App->audio->LoadFx("sounds/FlipperUp1.wav");
	flipperDown = App->audio->LoadFx("sounds/FlipperDown1.wav");
	drain = App->audio->LoadFx("sounds/Drain1.wav");
	collisionfx = App->audio->LoadFx("sounds/BallCollision2.wav");


	//RECTS-------------------------
	spring_control = { 455,834,17,21 };

	//BOOLS-------------------------
	Shoot = false;
	restart = false;
	getpoints1 = false;
	getpoints2 = false;
	pause = false;

	//BODIES------------------------
	Ball = App->physics->CreateCircle(455, 824, 11, b2_dynamicBody, 0.1f);

	BallSensor= App->physics->CreateRectangleSensor(455 + 10, 834 + 5, 25, 21);
	BallSensor->listener = this;

	Restart = App->physics->CreateRectangleSensor(240, 870, 480, 10);
	Restart->listener = this;

	//COUNTERS---------------------
	force_counter = 0;
	score = 0;
	lives = 3;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//Pause system---------------------------------------
	if (SDL_GetTicks() >= ticks && pause == true)
	{
		pause = false;
		score = 0;
	}
	
	if (pause == true)
	{
		App->renderer->Blit(gameover, 30,300);
	}

	
	//Game Controls--------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->scene_intro->leftflipper->body->ApplyAngularImpulse(-3.0f,true);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperUp);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		App->audio->PlayFx(flipperDown);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->rightflipper->body->ApplyAngularImpulse(3.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperUp);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		App->audio->PlayFx(flipperDown);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && spring_control.y<=852 )
	{
		spring_control.y += 1;
		force_counter += 1;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		spring_control.y = 834;
		App->audio->PlayFx(drain);
		
		if (Shoot == true && pause == false)
		{
			float force = ((float)force_counter * 25) / 3;
			Ball->body->ApplyForce({ 0,-force }, Ball->body->GetLocalCenter(), true);
			Shoot = false;
			App->scene_intro->cannon_block->body->SetActive(false);
		}
		
		force_counter = 0;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		int i, j;
		SDL_GetMouseState(&i, &j);
		Ball->body->SetTransform({ PIXEL_TO_METERS((float)i),  PIXEL_TO_METERS((float)j) }, 0.0f);
	}

	
	//Restart method--------------------------------------------
	if (App->scene_intro->slide_block->body)
	{
		App->renderer->Blit(blocker_texture, 251, 11);
	}

	if (restart == true)
	{
		Ball->body->SetTransform({ PIXEL_TO_METERS(455+0.2f), PIXEL_TO_METERS(824-0.2f) }, 0.0f);
		Ball->body->SetLinearVelocity({ 0,0 });
		App->scene_intro->slide_block = App->physics->CreateRectangle(253, 26, 5, 31, b2_staticBody);
		App->scene_intro->open = false;
		
		if (lives > 0)
		{
			lives -= 1;
		}

		if(lives==0) 
		{
			lives = 3;
			Timer(4000);
			App->scene_intro->w_passed = false;
			App->scene_intro->i_passed = false;
			App->scene_intro->n_passed = false;
			App->scene_intro->millioncount = 0;

			App->scene_intro->arrow_1 = false;
			App->scene_intro->arrow_2 = false;
			App->scene_intro->arrow_3 = false;
			App->scene_intro->arrow_4 = false;
			App->scene_intro->arrow_5 = false;
			App->scene_intro->arrow_6 = false;
			App->scene_intro->arrow_7 = false;
			App->scene_intro->arrow_8 = false;
			App->scene_intro->arrow_9 = false;
		}

		restart = false;
	}


	//Points------------------------------------------------
	if (getpoints1 == true)
	{
		score += 100;
		getpoints1 = false;
	}
	
	if (getpoints2 == true)
	{
		score += 10000;
		getpoints2 = false;
	}

	//Blit everything---------------------------------------
	int x, y;
	Ball->GetPosition(x, y);

	
	App->renderer->Blit(Spring, spring_control.x, spring_control.y);
	App->renderer->Blit(ball_texture, x,y);

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->body->GetFixtureList()->IsSensor())
	{
		if (bodyA == BallSensor && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			Shoot = true;
		}
		if (bodyA == Restart)
		{
			restart = true;
		}
		if (bodyA == App->scene_intro->B_1sensor)
		{
			LOG("puntos");
		}
	}
	
}

void ModulePlayer::Timer(int time)
{
	pause = true;
	ticks = SDL_GetTicks()+time;
}




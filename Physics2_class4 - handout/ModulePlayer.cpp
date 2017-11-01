#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
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
	Spring = App->textures->Load("pinball/PinBall_Spring.png");
	ball_texture = App->textures->Load("pinball/Ball.png");
	blocker_texture = App->textures->Load("pinball/slide_blocker.png");
	//bouncers=App->textures->Load("pinball/")

	//RECTS-------------------------
	spring_control = { 455,834,17,21 };

	//BOOLS-------------------------
	Shoot = false;
	restart = false;
	getpoints = false;
	pause = false;

	//BODIES------------------------
	Ball = App->physics->CreateCircle(455, 824, 11, b2_dynamicBody, 0.4f);
	
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
	if (SDL_GetTicks() >= ticks && pause == true)
	{
		pause = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->scene_intro->leftflipper->body->ApplyAngularImpulse(-3.0f,true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->scene_intro->rightflipper->body->ApplyAngularImpulse(3.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && spring_control.y<=852 && Shoot==true && pause==false)
	{
		spring_control.y += 1;
		force_counter += 1;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && Shoot==true && pause==false)
	{
		float force= ((float)force_counter * 25)/3;
		spring_control.y = 834;
		Ball->body->ApplyForce({ 0,-force}, Ball->body->GetLocalCenter(), true);
		force_counter = 0;
		Shoot = false;
		App->scene_intro->slide_block= App->physics->CreateRectangle(253, 26, 5, 31, b2_staticBody);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		int i, j;
		SDL_GetMouseState(&i, &j);
		Ball->body->SetTransform({ PIXEL_TO_METERS((float)i),  PIXEL_TO_METERS((float)j) }, 0.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Ball->body->SetTransform({ PIXEL_TO_METERS(455+0.2f), PIXEL_TO_METERS(824-0.1f) }, 0.0f);
		Ball->body->SetLinearVelocity({ 0,0 });
		lives = 3;
		score = 0;
		restart = false;
	}

	if (App->scene_intro->slide_block->body)
	{
		App->renderer->Blit(blocker_texture, 251, 11);
	}

	if (restart == true)
	{
		Ball->body->SetTransform({ PIXEL_TO_METERS(455+0.2f), PIXEL_TO_METERS(824-0.2f) }, 0.0f);
		Ball->body->SetLinearVelocity({ 0,0 });
		
		if (lives > 0)
		{
			lives -= 1;
			Timer(1000);
		}

		if(lives==0) //if we want to count from 2 to 0 then change this "if" for an "else"
		{
			lives = 3;
			score = 0;
			Timer(2000);
		}

		restart = false;
	}

	if (getpoints == true)
	{
		score += 100;
		getpoints = false;
	}


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




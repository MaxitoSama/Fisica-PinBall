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

	Spring = App->textures->Load("pinball/PinBall_Spring.png");

	spring_control = { 455,834,17,21 };
	Ball = App->physics->CreateCircle(455, 824, 11, b2_dynamicBody);
	force_counter = 0;

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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->scene_intro->leftflipper->body->ApplyAngularImpulse(-5.0f,true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->scene_intro->rightflipper->body->ApplyAngularImpulse(5.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && spring_control.y<=852)
	{
		spring_control.y += 1;
		force_counter += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP/* && App->scene_intro->sensor->*/)
	{
		float force= ((float)force_counter * 25)/3;
		spring_control.y = 834;
		Ball->body->ApplyForce({ 0,-force}, Ball->body->GetLocalCenter(), true);
		force_counter = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Ball->body->SetTransform({ PIXEL_TO_METERS(455), PIXEL_TO_METERS(824) }, 0.0f);
	}

	App->renderer->Blit(Spring, spring_control.x, spring_control.y);

	return UPDATE_CONTINUE;
}




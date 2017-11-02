#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void Timer(int time);

public:
	p2List<PhysBody*>	circles;
	p2List<PhysBody*>	boxes;
	p2List<PhysBody*>	ricks;
	p2List<PhysBody*>	leftbouncer;
	p2List<PhysBody*>	rightbouncer;
	
	PhysBody*			leftflipper;
	PhysBody*			rightflipper;
	PhysBody*			l_flipper_joint;
	PhysBody*			r_flipper_joint;
	PhysBody*			bouncer_1;
	PhysBody*			bouncer_2;
	PhysBody*			bouncer_3;
	PhysBody*			slide_block;
	PhysBody*			cannon_block;
	PhysBody*			Chain1;

	PhysBody*			canon_sensor;
	PhysBody*			sensorblocker_w;
	PhysBody*			sensorblocker_i;
	PhysBody*			sensorblocker_n;
	PhysBody*			B_1sensor;
	PhysBody*			B_2sensor;
	PhysBody*			B_3sensor;

	PhysBody*			Million;

	PhysBody*			switch_1;
	PhysBody*			switch_2;
	PhysBody*			switch_3;
	PhysBody*			switch_4;
	PhysBody*			switch_5;
	PhysBody*			switch_6;
	PhysBody*			switch_7;
	PhysBody*			switch_8;
	PhysBody*			switch_9;


	bool				open;

	bool				sensed;
	bool				canon_shoot;

	SDL_Texture*		circle;
	SDL_Texture*		box;
	SDL_Texture*		rick;
	SDL_Texture*		BackGround;
	SDL_Texture*		RightFlipper;
	SDL_Texture*		LeftFlipper;
	SDL_Texture*		LeftBouncer;
	SDL_Texture*		RightBouncer;
	SDL_Texture*		LetterW;
	SDL_Texture*		LetterI;
	SDL_Texture*		LetterN;
	SDL_Texture*		million_1;
	SDL_Texture*		million_2;
	SDL_Texture*		million_3;
	SDL_Texture*		million_4;

	uint				bonus_fx;
	uint				ticks;
	uint				click;
	
	p2Point<int>		ray;
	
	bool				ray_on;
	bool				w_passed;
	bool				i_passed;
	bool				n_passed;


	bool				million1;
	bool				million2;
	bool				million3;
	bool				million4;

	bool				give1m;
	bool				give2m;
	bool				give3m;
	bool				give4m;

};

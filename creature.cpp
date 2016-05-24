#include "creature.h"
#include <allegro5/allegro_primitives.h>

Creature::Creature(float x, float y, float velX, int dirX, int ID, ALLEGRO_BITMAP *image)
{
	GameObject::Init(x, y, velX, dirX, 0, 0);
	GameObject::SetID(ID);
	GameObject::SetDY(0);
	GameObject::SetVY(0);
    Creature::damage = 10;
    Creature::hp = 30;
    Creature::attack_cooldown = 0;
    gold_bounty = 5;
	//maxFrame = 0;
	//curFrame = 0;
	//frameCount = 0;
	//frameDelay = 0;
	//frameWidth = 0;
	//frameHeight = 0;
	//animationColumns = 0;
	Creature::image = image;
	maxFrame = 8;//Dla goblina
	curFrame = 0;
	frameCount = 0;
	frameDelay = 5;
	frameWidth = 64;
	frameHeight = 52;
	animationColumns = 8;
}

void Creature::Destroy()
{
	GameObject::Destroy();
}

void Creature::Update()
{
	GameObject::Update();

	if(++frameCount >= frameDelay)
	{
		curFrame++;
		if(curFrame >= maxFrame)
			curFrame = 0;
		frameCount = 0;
	}

	if(x < 0 || x > 800)
		SetAlive(false);
}

void Creature::Render()
{
	GameObject::Render();

	if (GetID() == ENEMY) al_draw_ellipse(x, 165+y*15, 24, 10,al_map_rgb(225,0,0), 1);
	else al_draw_ellipse(x, 165+y*15, 24, 10,al_map_rgb(0,225,0), 1);

    if (GetID() == ENEMY)
    {
	al_draw_bitmap_region(image, curFrame * frameWidth, 0, frameWidth, frameHeight,
		x - frameWidth / 2, 116+y*15, 0);
    }
    else
    al_draw_bitmap(image, x-32,106+y*15, 0 );
}

	int Creature::CheckAttack()
	{
	    if (GetCooldown() > 0)
        {
        SetCooldown(GetCooldown()-1);
        return 0;
        }
        else
        {
        SetCooldown(60+rand()%20);
        return rand()%4 + 9;
        }
	}

    void Creature::GotHit(int dam)
    {
        int hp;
        hp = Creature::GetHp();
        LoseLife(dam);
        if (hp < 1)
        Creature::SetAlive(false);
    }

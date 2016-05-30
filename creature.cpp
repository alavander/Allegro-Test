#include "creature.h"
#include <allegro5/allegro_primitives.h>

Creature::Creature(float x, float y, float stats[], int dirX, int ID, ALLEGRO_BITMAP *image)
{
	GameObject::Init(x, y, stats[2], dirX, 0, 0);
	GameObject::SetID(ID);
    Creature::damage = stats[0];
    Creature::hp = stats[1];
    Creature::attack_cooldown = 0;
	Creature::image = image;
	Creature::maxFrame = 8;//Dla goblina
	Creature::curFrame = 0;
	Creature::frameCount = 0;
	Creature::frameDelay = 5;
	Creature::frameWidth = 64;
	Creature::frameHeight = 52;
	Creature::animationColumns = 8;
	Creature::UNIT_TYPE = stats[4];
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

    if (GetID() == ENEMY)
    {
    al_draw_ellipse(x, 165+y*15, 24, 10,al_map_rgb(225,0,0), 1);//Red Circle for Enemies
	al_draw_bitmap_region(image, curFrame * frameWidth, 0, frameWidth, frameHeight,
		x - frameWidth / 2, 116+y*15, 0);
    }
    else
    {
        al_draw_ellipse(x, 165+y*15, 24, 10,al_map_rgb(0,225,0), 1);//Green Circle for Allies
            switch(GetType())//Drawing different color based on unit type.
            {
            case BERSERKER:
                al_draw_tinted_bitmap(image, al_map_rgb(180, 100, 100),x-32,106+y*15, 0);
                break;
            case LORD:
                al_draw_tinted_bitmap(image, al_map_rgb(100, 200, 100),x-32,106+y*15, 0);
                break;
            case DWARF:
                al_draw_bitmap(image, x-32,106+y*15, 0 );
                break;
            }
    }
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
        return GetAttackValue()/2 + rand() % GetAttackValue()/2;
        }
	}

    void Creature::GotHit(int dam)
    {
        LoseLife(dam);
        int hp = GetHp();
        if (hp < 1)
        SetAlive(false);
    }

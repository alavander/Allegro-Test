#include "creature.h"

Creature::Creature(float x, float y, float velX, int dirX, int ID, ALLEGRO_BITMAP *image)
{
	GameObject::Init(x, y, velX, dirX, 0, 0);
	GameObject::SetID(ID);
	GameObject::SetDY(0);
	GameObject::SetVY(0);
    Creature::damage = 10;
    Creature::hp = 30;
    Creature::attack_cooldown = 0;
	//maxFrame = 0;
	//curFrame = 0;
	//frameCount = 0;
	//frameDelay = 0;
	//frameWidth = 0;
	//frameHeight = 0;
	//animationColumns = 0;
	Creature::image = image;
}

void Creature::Destroy()
{
	GameObject::Destroy();
}

void Creature::Update()
{
	GameObject::Update();

	/*if(++frameCount >= frameDelay)
	{
		curFrame += animationDirection;
		if(curFrame >= maxFrame)
			curFrame = 0;
		else if(curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
	}*/

	if(x < 0 || x > 800)
		SetAlive(false);
}

void Creature::Render()
{
	GameObject::Render();

	//int fx = (curFrame % animationColumns) * frameWidth;
	//int fy = (curFrame / animationColumns) * frameHeight;

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
        SetCooldown(30);
        return 10;
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

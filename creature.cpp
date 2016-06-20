#include "creature.h"



Creature::Creature(float x, float y, Squad *squad_name)
{
    Creature::ptr_to_squad = &*squad_name;
    //Initializing and stats
	GameObject::Init(x, y, squad_name->GetSpeed(), squad_name->GetFraction() == DWARFKIN ? 1 : -1 , 0, 0);
	GameObject::SetID(squad_name->GetFraction() == DWARFKIN ? PLAYER : ENEMY);
	GameObject::SetAnim(0);
    Creature::damage = squad_name->GetDamage();
    Creature::hp = squad_name->GetHp();
	//Animation
	Creature::image = squad_name->image;
	Creature::maxFrame = squad_name->GetMaxFrame();
	Creature::frameDelay = squad_name->GetFrameDelay();
	Creature::frameWidth = squad_name->GetFrameWidth();
	Creature::frameHeight = squad_name->GetFrameHeight();
	Creature::animationColumns = squad_name->GetAnimationColumns();
	Creature::attackDelay = squad_name->GetAttackDelay();
	Creature::curFrame = 0;
	Creature::frameCount = 0;
    Creature::attack_cooldown = attackDelay*animationColumns;
}

void Creature::Destroy()
{
	GameObject::Destroy();
}

void Creature::Update()
{
	GameObject::Update();

    switch(ANIMATION)
    {
    case WALKING:
    if(CanMove())
	if(++frameCount >= frameDelay)
	{
		curFrame++;
		if(curFrame >= maxFrame)
			curFrame = 0;
		frameCount = 0;
	}
	break;
    case ATTACKING:
	if(++frameCount >= attackDelay)
	{
		curFrame++;
		if(curFrame >= maxFrame)
			curFrame = 0;
		frameCount = 0;
	}
	break;
    }

	if(GetID() == ENEMY && x < 0)
        Stage::DecreaseStageLive();

	if(x < 0 || x > 1600)
		SetAlive(false);
}

void Creature::Render()
{
	GameObject::Render();

    if (GetID() == ENEMY)
    {
    //al_draw_ellipse(x-Stage::cameraX, PositionY(), 24, 10,al_map_rgb(225,0,0), 1);//Red Circle for Enemies
	al_draw_bitmap_region(image, curFrame * frameWidth, (ANIMATION*frameHeight), frameWidth, frameHeight,
		x - frameWidth / 2 -Stage::cameraX, PositionY(), ALLEGRO_FLIP_HORIZONTAL);
    }
    else
    {
        //al_draw_ellipse(x-Stage::cameraX, PositionY(), 24, 10,al_map_rgb(0,225,0), 1);//Green Circle for Allies
        al_draw_bitmap_region(image, curFrame * frameWidth, 0, frameWidth, frameHeight,
		x - frameWidth / 2 -Stage::cameraX, PositionY(), 0);
    }
}

	int Creature::CheckAttack()
	{
        SetAnim(ATTACKING);
	    if (GetCooldown() > 0)
        {
        SetCooldown(GetCooldown()-1);
        return 0;
        }
        else
        {
        SetCooldown(attackDelay*animationColumns);
        return GetAttackValue()/2 + rand() % GetAttackValue()/2;
        }
	}

    void Creature::GotHit(int dam)
    {
        LoseLife(dam);
        int hp = GetHp();
        if (hp < 1)
        {
        SetAlive(false);
        if (GetID() == ENEMY) Stage::ObjectivesCount++;
        }
    }

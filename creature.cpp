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
    Creature::UNIT_TYPE = squad_name->GetUnitType();
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
    Creature::isDying = false;
    Creature::isAttacking = false;
    //Increasing the enemy type counts, to prevent spawning too much of strong creatures
    if( GetID() == ENEMY && GetUnitType() == RARE) Stage::IncreaseRareNumber();
    if( GetID() == ENEMY && GetUnitType() == UNCOMMON) Stage::IncreaseUncommonNumber();
}

void Creature::Destroy()
{
	GameObject::Destroy();
}

void Creature::Update()
{
	if (GetUnitType() != LEGENDARY)
    GameObject::Update();

	if (IfIsDying() == true) ChangeState(DYING); //Sprawdzamy czy umiera
	else if (IfIsAttacking() == true) ChangeState(ATTACKING); //Jesli nie umiera, to sprawdzamy czy atakuje
	else if (CanMove() == true) ChangeState(WALKING); //Jesli ani nie umiera, ani nie atakuje, to sprawdzamy czy moze isc
    switch(ANIMATION)
    {
    case WALKING:
    if(CanMove())//Updatujemy animacje tylko jesli moze isc.
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
    case DYING:
	if(++frameCount >= frameDelay)
	{
		curFrame++;
		if(curFrame >= maxFrame)
		{
            if (GetID() == ENEMY)
            {
                if (GetUnitType() == RARE)
                {
                 Stage::DecreaseRareNumber();
                 Stage::AwardHonor(3);
                }
                if (GetUnitType() == UNCOMMON)
                {
                Stage::DecreaseUncommonNumber();
                Stage::AwardHonor(1);
                }
            }
            if(Stage::STAGE_VICTORY_CONDITION == BLOODBATH)
                Stage::ObjectivesCount++;
            SetAlive(false);
		}
        frameCount = 0;
    }
	break;
    }

    switch(Stage::STAGE_VICTORY_CONDITION)
    {
    case SIEGE:
        {
	if(GetID() == ENEMY && x < 0)
        Stage::DecreaseStageLive();
    if(GetID() == PLAYER && x > 1600)
        Stage::AwardGold(ptr_to_squad->GetGoldCost());
	if(x < 0 || x > 1600)
		SetAlive(false);
    break;
        }
    case BLOODBATH:
        {
	if(GetID() == ENEMY && x < 150 && GetY() != 0)
        {
            SetY(0);
            SetX(150);
        }
    if(GetID() == PLAYER && x > 1600)
        Stage::AwardGold(ptr_to_squad->GetGoldCost());
	if( x > 1600)
		SetAlive(false);
    break;
        }
    case HERO_HUNTING:
        {
	if(GetID() == ENEMY && x < 150 && GetY() != 0)
        {
            SetY(0);
            SetX(150);
        }
    if(GetID() == PLAYER && x > 1450 && GetY() != 0)
        {
            SetY(0);
            SetX(1450);
        }
    break;
        }
    }

        SetMove(true); //Defaulotowo przyjmujemy, ze moze chodzic, jesli nie jest herosem
    	SetAttacking(false); //Defaultowo przyjmujemy, ze nie atakuje
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
        al_draw_bitmap_region(image, curFrame * frameWidth, (ANIMATION*frameHeight), frameWidth, frameHeight,
		x - frameWidth / 2 -Stage::cameraX, PositionY(), 0);
        //al_draw_line(x-Stage::cameraX, PositionY(), x-Stage::cameraX, PositionY(), al_map_rgb(240,0,0), 2);
    }
}

	int Creature::CheckAttack()
	{
        if(!IfIsDying())
        {
        SetAttacking(true);
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
	}

    void Creature::GotHit(int dam)
    {
        if(!IfIsDying())
        {
        LoseLife(dam);
        if (GetHp() < 1)
    		SetDying(true);
        }
    }

    void Creature::ChangeState(int newState)
    {
        int state = GetAnimation();
        if (state != newState)
        {
            switch(newState)
            {
                case WALKING:
                {
                SetAnim(WALKING);
                curFrame = 0;
                frameCount = 0;
                break;
                }
                case ATTACKING:
                {
                SetAnim(ATTACKING);
                curFrame = 0;
                frameCount = 0;
                break;
                }
                case DYING:
                {
                SetAnim(DYING);
                curFrame = 0;
                frameCount = 0;
                break;
                }
            }
        }
    }

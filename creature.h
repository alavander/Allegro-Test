#pragma once
#include "objects.h"
#include <iostream>

class Creature : public GameObject
{
private:
    int damage, hp, attack_cooldown, UNIT_TYPE;

protected:


public:
	Creature(float x, float y, float stats[], int dirX, int ID, ALLEGRO_BITMAP *image);

	void Destroy();
	void Update();
	void Render();

    //GameObject inherted functions
    void GotHit(int dam);
    int CheckAttack();

    //Attack functions
    int GetCooldown() {return attack_cooldown;}
    void SetCooldown(int attack_cooldown) {Creature::attack_cooldown = attack_cooldown;}
    int GetAttackValue() {return damage;}
    int GetHp() {return hp;}
    int GetType() {return UNIT_TYPE;}
    void LoseLife(int hp) {Creature::hp -= hp;}

};

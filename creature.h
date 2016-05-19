#pragma once
#include "objects.h"


class Creature : public GameObject
{
private:
    int damage, hp, attack_cooldown;

protected:


public:
	Creature(float x, float y, float velX, int dirX, int ID, ALLEGRO_BITMAP *image);

	void Destroy();
	void Update();
	void Render();

    //GameObject inherted functions
	int CheckAttack();
    void GotHit(int dam);

    //Attack functions
    int GetCooldown() {return attack_cooldown;}
    void SetCooldown(int attack_cooldown) {Creature::attack_cooldown = attack_cooldown;}
    int PerformAttack() {return damage;}
    int GetHp() {return hp;}
    void LoseLife(int hp) {Creature::hp -= hp;}
};

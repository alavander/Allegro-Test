#pragma once
#include "objects.h"
#include <allegro5/allegro_primitives.h>
#include "stage.h"
#include "squad.h"


class Creature : public GameObject
{
private:
    int damage, hp, attack_cooldown, attackDelay, UNIT_TYPE;
    bool isDying;
protected:


public:
	Creature(float x, float y, Squad *squad_name);
    Squad *ptr_to_squad;
	void Destroy();
	void Update();
	void Render();

    //GameObject inherted functions
    void GotHit(int dam);
    int CheckAttack();

    bool IfIsDying() {return isDying;}
    bool SetDying(bool isDying) {Creature::isDying = isDying;}
    int GetCooldown() {return attack_cooldown;}
    void SetCooldown(int attack_cooldown) {Creature::attack_cooldown = attack_cooldown;}
    int GetAttackValue() {return damage;}
    int GetHp() {return hp;}
    int GetType() {return UNIT_TYPE;}
    void LoseLife(int hp) {Creature::hp -= hp;}


};

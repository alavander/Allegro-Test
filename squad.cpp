#include "squad.h"
#include "Const.h"

Squad::Squad(int FRACTION, stats sq_stat, animation sq_anim)
{
    Squad::FRACTION = FRACTION;
    Squad::level = 1;
    Squad::experience = 0;
    Squad::squad_stat = &sq_stat;
    Squad::squad_anim = &sq_anim;
    Squad::gold_cost = sq_stat.gold_cost;
    Squad::damage = sq_stat.damage;
    Squad::hp = sq_stat.hp;
    Squad::speed = sq_stat.speed;
    Squad::maxFrame = sq_anim.maxFrame;
    Squad::frameDelay = sq_anim.frameDelay;
    Squad::frameWidth = sq_anim.frameWidth;
    Squad::frameHeight = sq_anim.frameHeight;
    Squad::animationColumns = sq_anim.animationColumns;
    Squad::image = sq_anim.image;
    Squad::attackDelay =sq_anim.attackDelay;
    Squad::icon_number = sq_anim.icon_number;

};


#include "squad.h"
#include "Const.h"

Squad::Squad(int FRACTION, stats sq_stat, animation sq_anim)
{
    Squad::FRACTION = FRACTION;
    Squad::level = 1;
    Squad::experience = 0;
    Squad::squad_stat = &sq_stat;
    Squad::squad_anim = &sq_anim;
    Squad::UNIT_TYPE = sq_stat.UNIT_TYPE;
    Squad::gold_cost = sq_stat.gold_cost;
    Squad::honor_cost = sq_stat.honor_cost;
    Squad::damage = sq_stat.damage;
    Squad::hp = sq_stat.hp;
    Squad::speed = sq_stat.speed;
    Squad::squad_name = sq_stat.name;
    Squad::maxFrame = sq_anim.maxFrame;
    Squad::frameDelay = sq_anim.frameDelay;
    Squad::frameWidth = sq_anim.frameWidth;
    Squad::frameHeight = sq_anim.frameHeight;
    Squad::animationColumns = sq_anim.animationColumns;
    Squad::image = sq_anim.image;
    Squad::attackDelay =sq_anim.attackDelay;
    Squad::icon_number = sq_anim.icon_number;
    Squad::isDeploying = 0;
};
bool Squad::CheckLevel()
{
    if (level < 11)
    {//levelujemy co 250 xp (lvl 2 = 250, lvl 3 = 500, itd.)
        if (experience > level*250) return true;
        else return false;
    }
}

void Squad::SquadUpdate()
{
        if (CheckLevel() == true)
        {
            SetSquadXP(experience-level*250);//zmniejszamy wolne pkt doswiadczenia
            AddHp(GetUnitType()+3);//+2-4 hp
            AddGoldCost(GetUnitType()+1);//+1-3 gold cost
            AddDamage(GetUnitType()+1);//+1-3 atk
            AwardLevel();//+1 level
        }
}

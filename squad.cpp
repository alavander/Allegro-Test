#include "squad.h"
#include "Const.h"
#include <iostream>

Squad::Squad(int squad_ID, int UNIT_TYPE, int gold_cost, unit_stats sq_stat, unit_animation sq_anim)
{
    Squad::squad_ID = squad_ID;
    Squad::UNIT_TYPE = UNIT_TYPE;
    Squad::level = 1;
    Squad::experience = 0;
    Squad::gold_cost = gold_cost;
    Squad::squad_stat = &sq_stat;
    Squad::squad_anim = &sq_anim;
};


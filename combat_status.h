#pragma once
#include <allegro5/allegro5.h>
#include "creature.h"
/*
buffs - attack up, regeneration
debuffs - attack down, poison
*/
class Combat_Status
{
    private:
        int type;       //type of status effect, can be positive and negative
        int modifier;   //power of status effect
        int duration;   //duration of status effect, if -1 it means its pernament
        int proc_time;  //after how much duration effect will proc, if -1 its constant
        ALLEGRO_BITMAP *image; //Status effect icon or visual effect on creature
    public:
        Combat_Status(Creature *target, int type, int modifier, int duration, int proc_time, ALLEGRO_BITMAP *image); //add new status effect to target creature
        void RemoveStatus(); //removes matching status effect from target creature
        void UpdateStatus(); //updates timer - in case it run out, it expires and restores
};

#include "stage.h"

/* Starting Stage variables */
int Stage::STAGE_VICTORY_CONDITION = HERO_HUNTING;
int Stage::AftermatchStatus = UNRESOLVED;
int Stage::lives = 5;
int Stage::rareNumber = 0;
int Stage::uncommonNumber = 0;
int Stage::ObjectivesCount = 0;
int Stage::cameraX = 0;
int Stage::StageTimeElapsed = 0;
float Stage::gold = 100;
int Stage::honor = 0;

Stage::Stage(int diff)
{
         int random_seed = rand() % 100000;
         StageVictory = random_seed % 3;     //0=hero,1=bloodbath,2=siege
         StageEnemy = random_seed % 2;       //0=greenskins,1=undeads
         StageBackground = random_seed % 3;  //0=forest,1=cave,2=swamp
         StageDiffaulty = diff;   //0=easy,1=medium,2=hard
         StageName = GenerateStageName();
         StageExpReward = ((rand()%31+50)*(10+StageDiffaulty*12))/10+StageEnemy*20+StageVictory*10;
}

std::string Stage::GenerateStageName()
{
    std::string first, second, third, forth;
    switch(StageDiffaulty)
    {
        case 0:
        first = "Beginners ";//easy
        break;
        case 1:
        first = "Intermediate ";//normal
        break;
        case 2:
        first = "Deadly ";//hard
        break;
        default:
        break;
    }
    switch(StageBackground)
    {
        case 0:
        second = "forest ";
        break;
        case 1:
        second = "cave ";
        break;
        case 2:
        second = "swamp ";
        break;
        default:
        break;
    }
    switch(StageVictory)
    {
        case 0:
        third = "hunt ";//hero hunt
        break;
        case 1:
        third = "skirmish ";//bloodbath
        break;
        case 2:
        third = "defence ";//siege
        break;
        default:
        break;
    }
    switch(StageEnemy)
    {
        case 0:
        forth = "of blood";//greenskin
        break;
        case 1:
        forth = "of death";//undead
        break;
        default:
        break;
    }
    return first+second+third+forth;
}

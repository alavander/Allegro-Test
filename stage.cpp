#include "stage.h"

Stage::Stage()
{
         int random_seed = rand() % 100000;
         StageVictory = random_seed % 3;     //0=hero,1=bloodbath,2=siege
         StageEnemy = random_seed % 2;       //0=greenskins,1=undeads
         StageBackground = random_seed % 3;  //0=forest,1=cave,2=swamp
         StageDiffaulty = random_seed % 3;   //0=easy,1=medium,2=hard
         StageName = GenerateStageName();
}

std::string Stage::GenerateStageName()
{
    std::string first, second, third, forth;
    switch(StageDiffaulty)
    {
        case 0:
        first = "Beginners ";
        break;
        case 1:
        first = "Intermediate ";
        break;
        case 2:
        first = "Deadly ";
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
        third = "hunt ";
        break;
        case 1:
        third = "skirmish ";
        break;
        case 2:
        third = "defence ";
        break;
        default:
        break;
    }
    switch(StageEnemy)
    {
        case 0:
        forth = "of blood";
        break;
        case 1:
        forth = "of death";
        break;
        default:
        break;
    }
    return first+second+third+forth;
}

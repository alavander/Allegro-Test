#pragma once
#include "creature.h"
#include "ftext.h"
#include "squad.h"
#include "stage.h"
#include <list>

class Deployment
{
    private:

    public:
        Deployment(Squad *slot1, Squad *slot2, Squad *slot3);
        ~Deployment();
        Squad *OccupiedSlot_1, *OccupiedSlot_2, *OccupiedSlot_3;
        Stage *Pregen_Stage_1, *Pregen_Stage_2, *Pregen_Stage_3, *Selected_Stage;
        GameObject * SpawnUnit(int Selected_Row, int Selected_Slot);
        GameObject * SpawnHero(int fraction);
        Squad GetSelectedSlot(int slot);//Aktualnie wybrany slot unita, do pobierania danych
        void SelectSquadForDeployment(Squad *selectedSquad, int slot);
        void GenerateStages();//tworzy trzy nowe mapy do wyboru
        void PlaySelectedStage();
        Creature *ptr_to_hero;
        Creature *ptr_to_enemy;
};

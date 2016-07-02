#pragma once
#include "squad.h"
#include "creature.h"
#include "Const.h"
#include <list>
#include <iostream>

class Deployment
{
    private:
        //std::list<Stage*> PreGeneratedStageList;
        //Stage *PtrToSelectedStage;
        //int StageMapPosition[6]; //trzyma x,y,x,y,x,y na mapie trzech wygenerowanych stageow by pokazac zaznaczenie.
        //int SquadListPosition[];
        //std::list<Squad*> AvailableSquads; <- przekazujemy do obiektu?
        //std::list<Squad*>::iterator squad_iter; <- j/w

        //utworz druzyne z posrod dostepnych
        //druzyny wybrane na nastepna mape

        //wybor druzyny i bohatera oraz wyswietlenie opisu i statów

        //zapis gry
        //load gry

    public:
        Deployment(Squad *slot1, Squad *slot2, Squad *slot3);
        Squad * OccupiedSlot_1;
        Squad * OccupiedSlot_2;
        Squad * OccupiedSlot_3;
        GameObject * SpawnUnit(int Selected_Row, int Selected_Slot);
        Squad GetSelectedSlot(int slot);//Aktualnie wybrany slot unita, do pobierania danych
        void SelectSquadForDeployment(Squad *selectedSquad, int slot);
};

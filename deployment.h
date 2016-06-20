#pragma once
#include "squad.h"
#include "creature.h"
#include <iostream>

class Deployment
{
    private:
        //ALLEGRO_IMAGE *Mission_Map = NULL;
        //std::list<Stage*> PreGeneratedStageList;
        //Stage *PtrToSelectedStage;
        //int StageMapPosition[6]; //trzyma x,y,x,y,x,y na mapie trzech wygenerowanych stageow by pokazac zaznaczenie.
        //int SquadListPosition[];
        //std::list<Squad*> AvailableSquads;
        //int DeploymentSlot_1, DeploymentSlot_2, DeploymentSlot_3;

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
        //void SelectSquadForDeployment(Squad SelectedSquad, int Slot);

        //int Deployment(int DifficulityMultiplier); //glowna petla deploymentu
        //void StageRequestGeneration(); //generuje 3 stage, ktore beda dostępne podczas deploymentu
        //void DrawSelected(Stage SelectedStage, Squad SelectedSquad); //wyswietla opisy w oparciu o PtrToSelectedStage
        // i PtrToSelectedSquad oraz pozycje X:Y w oparciu o arraye StageMapPosition oraz SquadListPosition.


};

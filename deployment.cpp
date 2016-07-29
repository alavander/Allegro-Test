#include "deployment.h"

Deployment::Deployment(Squad *slot1, Squad *slot2, Squad *slot3)
{
    //Bazowe wypelnienie slotow
    OccupiedSlot_1 = &*slot1;
    OccupiedSlot_1->SetDeploying(1);
    OccupiedSlot_2 = &*slot2;
    OccupiedSlot_2->SetDeploying(2);
    OccupiedSlot_3 = &*slot3;
    OccupiedSlot_3->SetDeploying(3);
    //Generacja losowych misji
    Stage * stage1 = new Stage(0);
    Deployment::Pregen_Stage_1 = stage1;
    Stage * stage2 = new Stage(1);
    Deployment::Pregen_Stage_2 = stage2;
    Stage * stage3 = new Stage(2);
    Deployment::Pregen_Stage_3 = stage3;
    Selected_Stage = Deployment::Pregen_Stage_1;
    //Ustawianie ptr na bohaterów na null
    ptr_to_enemy = nullptr;
    ptr_to_hero = nullptr;
};

Deployment::~Deployment()
{
    delete Pregen_Stage_1;
    delete Pregen_Stage_2;
    delete Pregen_Stage_3;
}

void Deployment::GenerateStages()
{
    //Usuwamy Stare Misje
    delete Pregen_Stage_1;
    delete Pregen_Stage_2;
    delete Pregen_Stage_3;
    //Generacja nowych, losowych misji
    Stage * stage1 = new Stage(0);
    Deployment::Pregen_Stage_1 = stage1;
    Stage * stage2 = new Stage(1);
    Deployment::Pregen_Stage_2 = stage2;
    Stage * stage3 = new Stage(2);
    Deployment::Pregen_Stage_3 = stage3;
    Selected_Stage = Deployment::Pregen_Stage_1;
}

GameObject * Deployment::SpawnUnit(int Selected_Row, int Selected_Slot)
{
    switch(Selected_Slot)
    {
        case 1:
        {
        Creature *unit = new Creature( 20, Selected_Row, OccupiedSlot_1 );
        return unit;
        break;
        }
        case 2:
        {
        Creature *unit = new Creature( 20, Selected_Row, OccupiedSlot_2 );
        return unit;
        break;
        }
        case 3:
        {
        Creature *unit = new Creature( 20, Selected_Row, OccupiedSlot_3 );
        return unit;
        break;
        }
    }
}

Squad Deployment::GetSelectedSlot(int slot)
{
    switch(slot)
    {
    case 1:
    return *OccupiedSlot_1;
    break;
    case 2:
    return *OccupiedSlot_2;
    break;
    case 3:
    return *OccupiedSlot_3;
    break;
    }
};

void Deployment::SelectSquadForDeployment(Squad *selectedSquad, int slot)
{
    switch(slot)
    {
    case 1:
    OccupiedSlot_1 = &*selectedSquad;
    break;
    case 2:
    OccupiedSlot_2 = &*selectedSquad;
    break;
    case 3:
    OccupiedSlot_3 = &*selectedSquad;
    break;
    }
};

void Deployment::PlaySelectedStage()
{
    /*Victory condition*/
    if (Selected_Stage->StageVictory == 0) Stage::STAGE_VICTORY_CONDITION = HERO_HUNTING;
    else if (Selected_Stage->StageVictory == 1) Stage::STAGE_VICTORY_CONDITION = BLOODBATH;
    else Stage::STAGE_VICTORY_CONDITION = SIEGE;
    Stage::gold = 100 - Selected_Stage->StageDiffaulty*10; //-10 gold for normal, -20 gold for hard
    Stage::lives = 5 - Selected_Stage->StageDiffaulty;//-1 lives for normal, -2 lives for hard
    Stage::rareNumber = 0;
    Stage::uncommonNumber = 0;
    Stage::ObjectivesCount = 0;
    Stage::cameraX = 0;
    Stage::honor = 0;
    Stage::StageTimeElapsed = 0;
    Stage::SetAftermatchStatus(UNRESOLVED);
};

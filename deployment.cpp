#include "deployment.h"

Deployment::Deployment(Squad *slot1, Squad *slot2, Squad *slot3)
{
    OccupiedSlot_1 = &*slot1;
    OccupiedSlot_1->SetDeploying(1);
    OccupiedSlot_2 = &*slot2;
    OccupiedSlot_2->SetDeploying(2);
    OccupiedSlot_3 = &*slot3;
    OccupiedSlot_3->SetDeploying(3);
};

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

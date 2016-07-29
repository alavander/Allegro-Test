#include "handler.h"

void Handler::Remove_objects(int remove_flag)
{
    switch(remove_flag)
    {
    case deadObj:
    {
    for(iter = objects.begin(); iter != objects.end(); )
    {
        if(! (*iter)->GetAlive())
        {
            delete (*iter);
            iter = objects.erase(iter);
        }
        else
            iter++;
    }
    break;
    }
    case allObj:
    {
        for(iter = objects.begin(); iter != objects.end(); )
    {
        delete (*iter);
        iter = objects.erase(iter);
    }
    break;
    }
     case miscObj:
    {
    for(iter = objects.begin(); iter != objects.end(); )
    {
        if((*iter)->GetID() == MISC)
        {
            delete (*iter);
            iter = objects.erase(iter);
        }
        else
            iter++;
    }
    break;
    }
    }
}

Squad * Handler::FindSquad(std::string name)
{
for(sqiter = AvailableSquads.begin(); sqiter != AvailableSquads.end(); ++sqiter)
{
    if((*sqiter)->GetSquadName() == name)
    break;
}
    return *sqiter;
}


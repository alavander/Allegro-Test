#pragma once
#include <list>
#include "objects.h"
#include "squad.h"

using std::list;

/*
Singleton:
Klasa ktora ma trzymac wszystkie in_game listy oraz ich iteratory
inicjowana na starcie,
kasowana na koncu
tworzyc nowy obiekt klasy i definiowac go jako extern,
by byl widoczny w innych plikach
 */
class Handler
{
    public:
    list<GameObject *> objects;
    list<GameObject *>::iterator iter;
    list<GameObject *>::iterator iter2;
    list<Squad *> AvailableSquads;
    list<Squad *>::iterator sqiter;
    void Remove_objects(int remove_flag);
};

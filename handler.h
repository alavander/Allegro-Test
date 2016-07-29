#pragma once
#include <list>
#include "objects.h"
#include "squad.h"

using std::list;
/*
Klasa ktora ma trzymac wszystkie in_game listy oraz ich iteratory,
powinna pozwalac zwracac wskaznik na dowolny obiekt o okreslonym typie i/lub parametrach
oraz wskaznik na dowolny squad o okreslonej nazwie
 */
class Handler
{
    public:
    list<GameObject *> objects;
    list<GameObject *>::iterator iter;
    list<GameObject *>::iterator iter2;
    list<Squad *> AvailableSquads;
    list<Squad *>::iterator sqiter;
    void Remove_objects(int remove_flag); //kasuje obiekty o okreslonej fladze: allObj, miscObj, deadObj
    Squad * FindSquad(std::string name); //wyszukuje squad po nazwie i zwraca go
};

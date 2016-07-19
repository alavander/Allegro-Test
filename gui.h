#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "deployment.h"
#include "game_state.h"

/*
Singleton:
Klasa ktora bedzie odpowiedzialna za rysowanie GUI
oraz bedzie trzymac wszystkie gui variables
*/
class GUI
{
    public:
        GUI(Deployment &deployment_, GameState &state_);
        void DrawGUI(Deployment &deployment_, GameState &state_);// -> na to skladaja sie wszystkie inne funkcje DRAW z  renderingu, kazda linia i wszystko ogólnie:
        void DrawMenuScreen(); // if state = menu
        void DrawDeploymentScreen(); // if state = deployment
        void DrawBattleMap(); // if state = playing or aftermatch, Background/Foreground
        void DrawBattleGUI(); // if state = playing or aftermatch, GUI

};

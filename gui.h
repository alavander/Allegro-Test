#pragma once
#include "deployment.h"
#include "game_state.h"
#include "handler.h"
#include "player.h"
#include <allegro5/allegro_primitives.h>


/*
Singleton:
Klasa ktora bedzie odpowiedzialna za rysowanie GUI
oraz bedzie trzymac wszystkie gui variables
*/
class GUI
{
    private:
        GameState * state;
        Data * data;
        Deployment *Deployed;
        Handler *handler;
        Player *player;
    public:
        GUI(GameState *state, Data *data, Deployment *Deployed, Handler *handler, Player *player);
        /* Drawing Function */
        void DrawGUI();// -> na to skladaja sie wszystkie inne funkcje DRAW z  renderingu, kazda linia i wszystko ogólnie:
        void DrawMenuScreen(); // if state = menu
        void DrawDeploymentScreen(); // if state = deployment
        void DrawBattleMap(); // if state = playing or aftermatch, Background/Foreground
        void DrawBattleGUI(); // if state = playing or aftermatch, GUI
        /* Control Functions */
        int unit_selected;
        int row_selected;
        bool cameraLeft;
        bool cameraRight;
        /* Input Functions */
        void Battle_ChangeRow(int input);
        void Deployment_ChangeSelectedSquad(int input);
};



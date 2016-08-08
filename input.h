#pragma once
#include "game_state.h"
#include "deployment.h"
#include "gui.h"
#include "handler.h"
#include "ftext.h"
#include "allegro5/events.h"
#include "allegro5/keyboard.h"

class Input
{
    public:
        Input(GameState * state, Deployment *deployed, GUI *gui, Handler *handler);
        void GetKeyboardInput(ALLEGRO_EVENT *input);//Zmienia input z klawiatury na komende
        void GetMouseClick(int x, int y);           //Jesli kliknelismy na jakis button, zmienia to na komende
        void TurnInputIntoCommand(int command);     //Zmienia komendy na dzialanie w systemie
    private:
        GameState *state;
        Deployment *deployed;
        GUI *gui;
        Handler *handler;
};

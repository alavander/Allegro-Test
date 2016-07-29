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
        void GetInput(ALLEGRO_EVENT *input);
    private:
        GameState *state;
        Deployment *deployed;
        GUI *gui;
        Handler *handler;
};

#include "gui.h"

GUI::GUI(GameState &state_, Data &data_)
{
    *state = state_;
    *data = data_;
}

void GUI::DrawGUI()
{
    int current_state = state->GetState();
    switch(current_state)
    {
        case MENU:
        DrawMenuScreen();
        break;
        case DEPLOYMENT:
        DrawDeploymentScreen();
        break;
        case PLAYING:
        case AFTERMATCH:
        DrawBattleMap();
        DrawBattleGUI();
        break;
    }
}

void GUI::DrawMenuScreen()
{
    //al_draw_bitmap(data->titleScreen, 0, 0, 0);
    //al_draw_text(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, HEIGHT/1.2, ALLEGRO_ALIGN_CENTRE, "Press spacebar to start");
};

void GUI::DrawDeploymentScreen()
{
};

void GUI::DrawBattleMap()
{
};

void GUI::DrawBattleGUI()
{
};

#include "gui.h"

GUI::GUI(Deployment &deployment_, GameState &state_)
{
}

void GUI::DrawGUI(Deployment &deployment_, GameState &state_)
{
    int state = state_.GetState();
    switch(state)
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

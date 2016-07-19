#include "game_state.h"

GameState::GameState()
{
    GSTATE = MENU;
    paused = false;
}

void GameState::ChangeState(int newState)
{
    if (GSTATE != newState)
    GSTATE = newState;
}


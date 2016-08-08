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

void GameState::Pause()
{
    if (GetState() == PLAYING)
    {
    if (GamePaused() == true) SetPause(false);
    else SetPause(true);
    }
}

#pragma once
#include "Const.h"

class GameState
{
    private:
        int GSTATE;
        bool paused; //Pauza nie musi byc oddzielnym statem, bo i po co?

    public:
        GameState();
        void ChangeState            (int newState);
        int  GetState()             {return GSTATE;}
        bool GamePaused()           {return paused;}
        void SetPause(bool paused)  {GameState::paused = paused;}
};

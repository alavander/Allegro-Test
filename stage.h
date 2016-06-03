#pragma once

class Stage
{
    public:
        static int stage_live;
        static int GetStageLives() {return stage_live;}
        static void DecreaseStageLive() {stage_live -= 1;}
};

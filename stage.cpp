#include "stage.h"

static bool CheckVictoryCondition(int obj_count)
        {
            switch(STAGE_VICTORY_CONDITION)
            {
                case HERO_HUNTING:
                if (obj_count >= 0) return true;
                else return false;
                break;
                case BLOODBATH:
                if (obj_count >= 40) return true;
                else return false;
                break;
                case SIEGE:
                if (obj_count >= 20) return true;
                else return false;
                break;
            }
};

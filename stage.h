#pragma once

class Stage
{
    public:
        static int stage_live;
        static int GetStageLives() {return stage_live;}
        static void DecreaseStageLive() {stage_live -= 1;}

        //Victory Condition-Related:
        static int STAGE_VICTORY_CONDITION; //Hero_hunting, bloodbath, siege
        static int ObjectivesCount; // w zaleznosci od victory condition, liczymy ilosc tego co potrzebujemy
        static bool CheckVictoryCondition(int obj_count)
        {
            switch (STAGE_VICTORY_CONDITION)
            {
                case HERO_HUNTING:
                if (obj_count > 0) return true;
                break;
                case BLOODBATH:
                if (obj_count > 40) return true;
                break;
                case SIEGE:
                if (obj_count > 20) return true;
                break;
            }
        };

};

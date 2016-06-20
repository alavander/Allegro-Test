#pragma once

class Stage
{
    public:
        /* Lives related */
        static int lives;
        static int GetStageLives() {return lives;}
        static void DecreaseStageLive() {lives -= 1;}
        /* Gold related */
        static float gold;
        static int GetStageGold() {return gold;}
        static void AwardGold(float amount) {gold += amount;}
        static void SpendGold(float amount) {gold -= amount;}
        static int cameraX;

        //Victory Condition-Related:
        static int STAGE_VICTORY_CONDITION; //Hero_hunting, bloodbath, siege
        static int ObjectivesCount; // w zaleznosci od victory condition, liczymy ilosc tego co potrzebujemy
        static int GetObjectivesCount() {return ObjectivesCount;};
        static bool CheckVictoryCondition(int obj_count)
        {
            switch (STAGE_VICTORY_CONDITION)
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
                default:
                return false;
            }
        };
};

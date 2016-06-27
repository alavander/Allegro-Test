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
        /* Honor related */
        static float honor;
        static int GetStageHonor() {return honor;}
        static void AwardHonor(float amount) {honor += amount;}
        static void SpendHonor(float amount) {honor -= amount;}

        /*Enemies count*/
        static int rareNumber;
        static int uncommonNumber;
        static int GetRareNumber() {return rareNumber;}
        static void DecreaseRareNumber() {rareNumber--;}
        static void IncreaseRareNumber() {rareNumber++;}
        static int GetUncommonNumber() {return uncommonNumber;}
        static void DecreaseUncommonNumber() {uncommonNumber--;}
        static void IncreaseUncommonNumber() {uncommonNumber++;}

        /*Stage Timer*/

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

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
        static int honor;
        static int GetStageHonor() {return honor;}
        static void AwardHonor(int amount) {honor += amount;}
        static void SpendHonor(int amount) {honor -= amount;}

        /*Enemies count*/
        static int rareNumber;
        static int uncommonNumber;
        static int GetRareNumber() {return rareNumber;}
        static void DecreaseRareNumber() {rareNumber--;}
        static void IncreaseRareNumber() {rareNumber++;}
        static int GetUncommonNumber() {return uncommonNumber;}
        static void DecreaseUncommonNumber() {uncommonNumber--;}
        static void IncreaseUncommonNumber() {uncommonNumber++;}

        /*Stage Misc*/
        static int cameraX;
        static int AftermatchStatus;
        static int GetAftermachStatus() {return AftermatchStatus;}
        static void SetAftermatchStatus(int newStatus) {Stage::AftermatchStatus = newStatus;}

        //Victory Condition-Related:
        static int STAGE_VICTORY_CONDITION; //Hero_hunting, bloodbath, siege
        static int ObjectivesCount; // w zaleznosci od victory condition, liczymy ilosc tego co potrzebujemy
        static int GetObjectivesCount() {return ObjectivesCount;};
        static bool CheckVictoryCondition(int obj_count)
        {
            switch (STAGE_VICTORY_CONDITION)
            {
                case HERO_HUNTING:
                if (obj_count >= 1) return true; //1 enemy hero killed
                else return false;
                break;
                case BLOODBATH:
                if (obj_count >= 50) return true; //score 50 kills
                else return false;
                break;
                case SIEGE:
                if (obj_count >= 4) return true; //survive 4 minutes
                else return false;
                break;
                default:
                return false;
            }
        };
};

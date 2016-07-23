#pragma once
#include <allegro5/allegro5.h>
#include <string>
#include "data.h"

class Squad
{
    private:
        int level;                  //Poziom jednostki
        int experience;           //Punkty doswiadczenia ktore ma jednostka
        int gold_cost;              //Ile zlota kosztuje przywolanie jednej jednostki
        int honor_cost;
        int FRACTION;
        //Taken from stats
        int damage;                 //Obrazenia ktore zadaje jednostka
        int hp;                     //Ilosc pkt zycia ktore posiada jednostka
        float speed;                //Szybkosc ruchu jednostki
        //Taken from anim
        int maxFrame;
        int frameDelay;
        int frameWidth;
        int frameHeight;
        int animationColumns;
        int attackDelay;
        int icon_number;
        int UNIT_TYPE;
        std::string squad_name;


        //int unit_number;          //Aktualny stan osobowy squadu
        //int max_unit_number;      //Maksymalny stan osobowy squadu
        //float squad_reinforce;    //Ile unitow przybywa miedzy misjami

    public:
        Squad(int FRACTION, stats sq_stat, animation sq_anim);
        //Public visibility
        stats *squad_stat; //Pointer do bazowych statow jednostki
        animation *squad_anim; //Pointer do IMG oraz informacji o animacji jednostki
        ALLEGRO_BITMAP *image;
        int GetSquadXP() {return experience;}
        float GetSpeed() {return speed;}
        int GetLevel() {return level;}
        int GetGoldCost() {return gold_cost;}
        int GetHonorCost() {return honor_cost;}
        int GetDamage() {return damage;}
        int GetHp() {return hp;}
        int GetMaxFrame() {return maxFrame;}
        int GetFrameDelay() {return frameDelay;}
        int GetFrameWidth() {return frameWidth;}
        int GetFrameHeight() {return frameHeight;}
        int GetAnimationColumns() {return animationColumns;}
        int GetFraction() {return FRACTION;}
        int GetUnitType() {return UNIT_TYPE;}
        int GetAttackDelay() {return attackDelay;}
        int GetIconNumber() {return icon_number;}
        std::string GetSquadName() {return squad_name;}
        int GetDeploying() {return isDeploying;}
        void SetDeploying(int isDeploying) {Squad::isDeploying = isDeploying;}
        void AddHp(int hp) {Squad::hp += hp;}
        void AddGoldCost(int gold_cost) {Squad::gold_cost += gold_cost;}
        void AddDamage(int damage) {Squad::damage += damage;}
        void AwardSquadXP(int var) {experience += var;}
        void SetSquadXP(int var) {experience = var;}
        void AwardLevel() {level += 1;}
        //Other:
        bool CheckLevel();//sprawdzamy czy squad zdobyl odpowiednia ilosc xp by zdobyc level up
        void SquadUpdate(); //Jezeli zdobedziemy level, to updatujemy staty naszego squadu
        void UpdateStats(); //Updatuje staty(zmiany eq lub level upy)
        int isDeploying; //Jesli jest isDeploying to znika z listy do deploy
};


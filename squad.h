#pragma once
#include <allegro5/allegro5.h>

struct unit_stats
{
    int damage;
    int hp;
    float speed;
};

struct unit_animation
{
	int maxFrame;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	ALLEGRO_BITMAP *image;
};

class Squad
{
    private:
        int UNIT_TYPE;              //Typ jednostki -> zmienic potem na (image)
        int level;                  //Poziom jednostki
        float experience;           //Punkty doswiadczenia ktore ma jednostka
        int gold_cost;              //Ile zlota kosztuje przywolanie jednej jednostki
        //int unit_number;          //Aktualny stan osobowy squadu
        //int max_unit_number;      //Maksymalny stan osobowy squadu
        //float squad_reinforce;    //Ile unitow przybywa miedzy misjami

    public:
        Squad(int squad_ID, int UNIT_TYPE, int gold_cost, unit_stats sq_stat, unit_animation sq_anim);
        //Public visibility
        unit_stats *squad_stat; //Pointer do bazowych statow jednostki
        unit_animation *squad_anim; //Pointer do IMG oraz informacji o animacji jednostki
        int squad_ID; //Public ID po ktorym filtrujemy squady
        //Setters:
        void AwardSquadXP(int var) {experience += var;}
        //Getters:
        float GetSquadXP() {return experience;}
        int GetGoldCost() {return gold_cost;}
        //Other:
        bool CheckLevel(); //Sprawdza, czy exp > exp potrzebny do nastepnego levelu i awarduje level.
        void UpdateStats(); //Updatuje staty bonus_stats (zmiany eq lub level upy)
        //bool isDeploying; //Jesli jest isDeploying to znika z listy do deploy
};


#pragma once
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>

using std::list;
using std::string;

struct stats
{
    std::string name;
    int damage;
    int hp;
    float speed;
    int gold_cost;
    int honor_cost;
    int UNIT_TYPE;
};

struct animation
{
    std::string name;
	int maxFrame;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int attackDelay;
	ALLEGRO_BITMAP *image;
	int icon_number;
};

/*
Klasa która będzie pobierać z plików i trzymać listy structów stats i animation */
class Data
{
    private:
        list<stats> stats_;
        list<animation> animation_;
    public:
        Data();
        ~Data();
        void LoadFromFiles();//Flaga Stats = do statsów, flaga anim = do anim.
        stats GetStats(string name); //Wyszukuje na liscie stats statsów o konkretnym name +dodać name
        animation GetAnim(string name);//Wyszukuje na liscie anim animation o konkretnym name +dodać name
        ALLEGRO_FONT *font18, *font12;
        //==============================================
        //PROJECT VARIABLES
        //==============================================
            ALLEGRO_BITMAP *soldierImage = NULL;
            ALLEGRO_BITMAP *barbarianImage = NULL;
            ALLEGRO_BITMAP *squireImage = NULL;
            ALLEGRO_BITMAP *heroImage = NULL;
            ALLEGRO_BITMAP *goblinImage = NULL;
            ALLEGRO_BITMAP *trollImage = NULL;
            ALLEGRO_BITMAP *gnollImage = NULL;
            ALLEGRO_BITMAP *ogreImage = NULL;
            ALLEGRO_BITMAP *skeletonImage = NULL;
            ALLEGRO_BITMAP *ghoulImage = NULL;
            ALLEGRO_BITMAP *wraithImage = NULL;
            ALLEGRO_BITMAP *necroImage = NULL;
            ALLEGRO_BITMAP *bgImage = NULL;
            ALLEGRO_BITMAP *uiImage = NULL;
            ALLEGRO_BITMAP *titleScreen = NULL;
            ALLEGRO_BITMAP *heartIcon = NULL;
            ALLEGRO_BITMAP *icons = NULL;
            ALLEGRO_BITMAP *unit_icon = NULL;
            ALLEGRO_BITMAP *deployment_map = NULL;
};

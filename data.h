#pragma once
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <allegro5/allegro5.h>
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
        void LoadFromFiles();//Flaga Stats = do statsów, flaga anim = do anim.
        stats GetStats(string name); //Wyszukuje na liscie stats statsów o konkretnym name +dodać name
        animation GetAnim(string name);//Wyszukuje na liscie anim animation o konkretnym name +dodać name
};

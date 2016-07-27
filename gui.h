#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include "deployment.h"
#include "game_state.h"


/*
Singleton:
Klasa ktora bedzie odpowiedzialna za rysowanie GUI
oraz bedzie trzymac wszystkie gui variables
*/
class GUI
{
    private:
        GameState * state;
        Data * data;
    public:
        GUI(GameState &state_, Data &data_);
        void DrawGUI();// -> na to skladaja sie wszystkie inne funkcje DRAW z  renderingu, kazda linia i wszystko ogólnie:
        void DrawMenuScreen(); // if state = menu
        void DrawDeploymentScreen(); // if state = deployment
        void DrawBattleMap(); // if state = playing or aftermatch, Background/Foreground
        void DrawBattleGUI(); // if state = playing or aftermatch, GUI
};

class Button
{
    public:
        OnClick(); //cos sie dzieje jak sie kliknie myszka w obrebie koordynatow
        Button(int x1, int y1, int x2, int y2, string text, ALLEGRO_BITMAP *image);
    private:
        std::string text; //text na buttonie
        ALLEGRO_BITMAP *image; //alternatywnie - obrazek
        int x1, y1, x2, y2;//Pole buttona
};

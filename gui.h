#pragma once
#include "deployment.h"
#include "game_state.h"
#include "handler.h"
#include "player.h"
#include <allegro5/allegro_primitives.h>


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
        Deployment *Deployed;
        Handler *handler;
        Player *player;
    public:
        GUI(GameState *state, Data *data, Deployment *Deployed, Handler *handler, Player *player);
        void DrawGUI();// -> na to skladaja sie wszystkie inne funkcje DRAW z  renderingu, kazda linia i wszystko ogólnie:
        void DrawMenuScreen(); // if state = menu
        void DrawDeploymentScreen(); // if state = deployment
        void DrawBattleMap(); // if state = playing or aftermatch, Background/Foreground
        void DrawBattleGUI(); // if state = playing or aftermatch, GUI
        int unit_selected;
        int row_selected;
        bool cameraLeft;
        bool cameraRight;
};

/*W handlerze dorobic liste buttonow.
Iterowac po wszystkich buttonach - te ktore sa w danym screenie dawac jako active, co bedzie powodowalo
ze bedzie dzialac na nie funkcja OnClick i beda widoczne.
Wszystkie inne ustawiac jako inactive, czyli nie beda widoczne oraz nie bedzie dzialac funkcja OnClick.
W przypadku klikniecia myszka, iterowac po wszystkich buttonach ktore: 1) sa w danym screenie oraz sa active,
oraz sprwadzac czy koordynaty klikniecia znajduja sie w koordynatach buttona.
Jesli sa, to wywolujemy funkcje OnClick danego buttona -> przerzucenie do EventHandlera informacji co nalezy zmienic.
*/
class Button
{
    public:
        OnClick(); //cos sie dzieje jak sie kliknie myszka w obrebie koordynatow
        Button(int screen, int x1, int y1, string text, ALLEGRO_BITMAP *image);
    private:
        void DrawButton();//Rysuje przycisk - w zaleznosci od stanu, kliknięty badz nie
        bool active;//Czy przycisk jest aktywny?
        int SCREEN;//enum ktory przypisuje buttona do konkretnego screena
        std::string text; //text na buttonie
        ALLEGRO_BITMAP *image; //obrazek (tlo)
        int x1, y1, x2, y2;//Pole buttona = koordynaty poczatkowe oraz get bitmap width/height
};

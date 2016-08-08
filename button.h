#pragma once
#include "string"
#include "allegro5/allegro5.h"

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
        bool ClickWithinButton(int x, int y);
        Button(int BUTTON_COMMAND, int screen, float scale, bool reversed, int x1, int y1, std::string text, ALLEGRO_BITMAP *image);
        void DrawButton();
        bool IsActive() {return active;}
        int BUTTON_COMMAND; //jakie dzialanie daje klikniecie w dany button
    private:
        bool active;//Czy przycisk jest aktywny?
        int SCREEN;//enum ktory przypisuje buttona do konkretnego screena
        float scale; //wielkosc buttona
        bool reversed; //czy wyswietlanie przycisku ma byc odwrocone czy nie
        std::string text; //text na buttonie
        ALLEGRO_BITMAP *image; //obrazek (tlo)
        int x1, y1, x2, y2;//Pole buttona = koordynaty poczatkowe oraz get bitmap width/height
};

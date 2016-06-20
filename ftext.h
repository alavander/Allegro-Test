#pragma once
#include "objects.h"
#include <allegro5/allegro_font.h>

class Ftext : public GameObject
{
    private:
        int duration, display;

    protected:
        ALLEGRO_FONT *font;
        ALLEGRO_BITMAP *image;

    public:
        Ftext(float x, float y, float velY, int display, int duration, ALLEGRO_FONT *font, ALLEGRO_BITMAP *image);
        void Destroy();
        void Update();
        void Render();
        int GetDuration() {return duration;}
};

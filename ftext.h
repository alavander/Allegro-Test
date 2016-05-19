#pragma once

#include "objects.h"
#include <allegro5/allegro_font.h>

class Ftext : public GameObject
{
    private:
        int duration, display;

    protected:
        ALLEGRO_FONT *font;

    public:
        Ftext(float x, float y, float velX, float velY, int display, ALLEGRO_FONT *font);
        void Destroy();
        void Update();
        void Render();
        int GetDuration() {return duration;}
};

#include "ftext.h"


Ftext::Ftext(float x, float y, float velX, float velY, int display, ALLEGRO_FONT *font)
{
    GameObject::Init(x, y, velX, 0, velY, -1);
	Ftext::display = display;
	Ftext::font = font;
}

void Ftext::Destroy()
{
	GameObject::Destroy();
}

void Ftext::Update()
{
	GameObject::Update();

	if(y < 0 || y > 400)
		SetAlive(false);
}

void Ftext::Render()
{
  al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE , "%i", display);
}

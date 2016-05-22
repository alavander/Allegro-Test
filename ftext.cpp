#include "ftext.h"


Ftext::Ftext(float x, float y, float velY, int display, ALLEGRO_FONT *font)
{
    GameObject::Init(x, y, 0, 0, velY, -1);
	Ftext::display = display;
	Ftext::font = font;
	Ftext::duration = 0;
	GameObject::SetID(MISC);
	GameObject::SetSolid(false);
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

    duration++;
    if (duration > 90) SetAlive(false);
}

void Ftext::Render()
{
  al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE , "%i", display);
}

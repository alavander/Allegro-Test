#include "ftext.h"


Ftext::Ftext(float x, float y, float velY, int display, int duration, ALLEGRO_FONT *font, ALLEGRO_BITMAP *image)
{
    GameObject::Init(x, y, 0, 0, velY, -1);
	Ftext::display = display;
	Ftext::font = font;
	Ftext::duration = duration;
	GameObject::SetID(MISC);
	GameObject::SetSolid(false);
	Ftext::image = image;
}

void Ftext::Destroy()
{
	GameObject::Destroy();
}

void Ftext::Update()
{
	GameObject::Update();

	if(y < 0 || y > 600)
		SetAlive(false);

    duration--;
    if (duration < 0) SetAlive(false);
}

void Ftext::Render()
{
  al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE , "%i", display);
  if (image != NULL)
    al_draw_bitmap(image, x+12, y+10, 0);
}

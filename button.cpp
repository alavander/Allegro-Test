#include "button.h"

Button::Button(int BUTTON_COMMAND, int screen, float scale, bool reversed, int x1, int y1, std::string text, ALLEGRO_BITMAP *image)
{
Button::BUTTON_COMMAND = BUTTON_COMMAND;
Button::reversed = reversed;
Button::SCREEN = screen;
Button::active = true;
Button::text = text;
Button::x1 = x1;
Button::y1 = y1;
Button::x2 = x1+(al_get_bitmap_width(image)*scale);
Button::y2 = y1+(al_get_bitmap_height(image)*scale);
Button::image = image;
Button::scale = scale;
}

bool Button::ClickWithinButton(int x, int y)
{
if ((x>Button::x1 && x<Button::x2) && (y>Button::y1 && y<Button::y2)) return true;
else return false;
}

void Button::DrawButton()
{   int image_width = al_get_bitmap_width(image);
    int image_height = al_get_bitmap_height(image);
    //al_draw_bitmap(image, x1, y1, NULL);
    if (reversed == false)
    al_draw_scaled_bitmap(image, 0, 0, image_width, image_height, x1, y1,  image_width*scale, image_height*scale, 0);
    else
    al_draw_scaled_bitmap(image, 0, 0, image_width, image_height, x1, y1,  image_width*scale, image_height*scale, ALLEGRO_FLIP_HORIZONTAL);
    //if (text != "") al_draw_textf(data->font18, al_map_rgb(255,255,255), x1+image_width/2, y1+image_height/2,0, "%s", text.c_str());
};

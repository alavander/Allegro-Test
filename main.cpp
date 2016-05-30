#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <iostream>
/* Local Includes */
#include "objects.h"
#include "creature.h"
#include "Const.h"
#include "ftext.h"

/*
Prototype-level ToDo:
- Implement Lives System -> Pointer na funkcje?
- Implement Resources(50%)
- Resources generation(50%)
- Animations (moving(50%), standing, attacking, dying)->requires freaking sprite sheets
- Writing/Loading from files
- Sounds and music
- Prevent Spawning Creatures on top of each other
*/
//==============================================
//GLOBALS
//==============================================

std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;
int STATE = MENU;

int main(int argc, char **argv)
{
//==============================================
//SHELL VARIABLES
//==============================================
    bool done = false;
    bool render = false;
    int row_selected = 1;
    int unit_selected = 1;
    float gold = 100;

//Unit Stats
                           //dam hp speed gold cost/spawn chance
//The Bad
float goblin_stat[]         ={8, 18, 4, 0, 3};
float goblin_war_stat[]     ={13,25, 3, 12, 3};
float troll_stat[]          ={25,84, 1, 3, 3};
//The Good
float dwarf_stat[]          ={12,35,1.8,10, 0};
float dwarf_berserker_stat[]={21,20,2.5,25, 1};
float dwarf_lord_stat[]     ={24,72,1.4,40, 2};


//==============================================
//PROJECT VARIABLES
//==============================================

	ALLEGRO_BITMAP *dwarfImage = NULL;
	ALLEGRO_BITMAP *goblinImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *uiImage = NULL;
	ALLEGRO_BITMAP *hand = NULL;
	ALLEGRO_BITMAP *goldIcon = NULL;
	ALLEGRO_BITMAP *titleScreen = NULL;

//==============================================
//ALLEGRO VARIABLES
//==============================================
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font18;

//==============================================
//ALLEGRO INIT FUNCTIONS
//==============================================
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object
    al_set_window_title(display, "Undertales of Drakern");

	if(!display)										//test display object
		return -1;

//==============================================
//ADDON INSTALL
//==============================================
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
    al_init_primitives_addon();

//==============================================
//PROJECT INIT
//==============================================
	font18 = al_load_font("dwarf.ttf", 20, 0);
    /*GUI*/
	bgImage = al_load_bitmap("background.png");
	uiImage = al_load_bitmap("background2.jpg");
	goldIcon = al_load_bitmap("gold.png");
	titleScreen = al_load_bitmap("title_screen.png");

	/*Jednostki*/
	dwarfImage = al_load_bitmap("dwarf_warrior.png");
	al_convert_mask_to_alpha(dwarfImage, al_map_rgb(255,255,255));

	goblinImage = al_load_bitmap("goblin_move.png");
	al_convert_mask_to_alpha(goblinImage, al_map_rgb(0,0,248));
	hand = al_load_bitmap("hand.png");
	al_convert_mask_to_alpha(hand, al_map_rgb(255,0,0));
    /*Timer*/
	srand(time(NULL));
	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//==============================================
		//INPUT
		//==============================================
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_P:
				if (STATE == PAUSED) STATE = PLAYING;
				else STATE = PAUSED;
				break;
			case ALLEGRO_KEY_UP:
                if (row_selected > 1) row_selected -= 1;
				break;
			case ALLEGRO_KEY_DOWN:
                if (row_selected < 3 ) row_selected += 1;
				break;
            case ALLEGRO_KEY_LEFT:
                if (unit_selected > 1) unit_selected -= 1;
                break;
            case ALLEGRO_KEY_RIGHT:
                if (unit_selected < 3) unit_selected += 1;
                break;
            case ALLEGRO_KEY_SPACE:
                    if (STATE == MENU) STATE = PLAYING;
                    else if (STATE == PLAYING)
                    {
                        if (unit_selected == 1 && gold > dwarf_stat[3])
                            {
                                Creature *dwarf = new Creature( 20, row_selected, dwarf_stat, 1, PLAYER, dwarfImage);
                                objects.push_back(dwarf);
                                gold -= dwarf_stat[3];
                                Ftext *text = new Ftext(230, 325, -0.5, -10, 45, font18);
                                objects.push_back(text);
                                break;
                            }
                        if (unit_selected == 2 && gold > dwarf_berserker_stat[3])
                            {
                                Creature *dwarf = new Creature( 20, row_selected, dwarf_berserker_stat, 1, PLAYER, dwarfImage);
                                objects.push_back(dwarf);
                                gold -= dwarf_berserker_stat[3];
                                Ftext *text = new Ftext(230, 325, -0.5, -25, 45, font18);
                                objects.push_back(text);
                                break;
                            }
                        if (unit_selected == 3 && gold > dwarf_lord_stat[3])
                            {
                                Creature *dwarf = new Creature( 20, row_selected, dwarf_lord_stat, 1, PLAYER, dwarfImage);
                                objects.push_back(dwarf);
                                gold -= dwarf_lord_stat[3];
                                Ftext *text = new Ftext(230, 325, -0.5, -40, 45, font18);
                                objects.push_back(text);
                                break;
                            }
                    }
		}
		}
		//==============================================
		//GAME UPDATE
		//==============================================
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			//=====================

            /*Goblin Spawning*/
            if(STATE == PLAYING && rand() % 115 == 0)
            {
                Creature *goblin = new Creature(WIDTH-20, rand() % 3 + 1, goblin_stat, -1, ENEMY, goblinImage);
                objects.push_back(goblin);
            }

            /*Gold Generation*/
            if(STATE == PLAYING)
            gold += 0.04;

            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            objects.sort([](GameObject * first, GameObject * second) {return first->GetY() < second->GetY();});

            /*Collision Checking*/
            if ( STATE == PLAYING)
            for(iter = objects.begin(); iter != objects.end(); ++iter)
            {
                if ( !(*iter)->GetAlive() || !(*iter)->GetSolid()) continue;
                for(iter2 = objects.begin(); iter2 != objects.end(); ++iter2)
                {
                    if( !(*iter2)->GetAlive() || (*iter2) == (*iter) || !(*iter2)->GetSolid())  continue;
                    if((*iter)->CollisionCheck((*iter2)))
                    {
                        ((*iter)->SetMove(false));
                        if((*iter)->IsInRange((*iter2), 60) && (*iter)->IsOppositeTeam((*iter2)))
                            {
                                int damage = (*iter)->CheckAttack();
                                if (damage > 0 )
                                {
                                (*iter2)->GotHit(damage);
                                Ftext *text = new Ftext(((*iter2)->GetX()-5+rand()%10), 100+((*iter2)->GetY())*15, 0.7, damage, 45, font18);
                                objects.push_back(text);
                                }
                            }
                    }
                }
            };
            //Updating (move + ToDo statuses)
                if( STATE == PLAYING)
                for(iter = objects.begin(); iter != objects.end(); ++iter)
                {
                     if ( !(*iter)->GetAlive()) continue;
                     {
                        if((*iter)->CanMove()) (*iter)->Update();
                        else ((*iter)->SetMove(true));
                     }
                }

			//cull the dead
			for(iter = objects.begin(); iter != objects.end(); )
			{
				if(! (*iter)->GetAlive())
				{
					delete (*iter);
					iter = objects.erase(iter);
				}
				else
					iter++;
			}

		//==============================================
		//RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			if (STATE == MENU)
            {
            al_draw_bitmap(titleScreen, 0, 0, 0);
            al_draw_text(font18, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTRE , "Press spacebar to start");
            }
			if (STATE == PLAYING || STATE == PAUSED)
            {
            al_draw_bitmap(bgImage, 0, 0, 0);
            al_draw_bitmap(uiImage, 0, 240, 0);
            al_draw_bitmap(goldIcon, 80, 310, 0);
			//BEGIN PROJECT RENDER===============
            for(iter = objects.begin(); iter != objects.end(); ++iter)
              {
                  (*iter)->Render();
              }
              if (STATE == PAUSED)
              al_draw_text(font18, al_map_rgb(255, 255, 255), 400, 30, ALLEGRO_ALIGN_CENTRE , "Game Paused");
            /* Row Selected Cursor */
              al_draw_tinted_bitmap(hand, al_map_rgba_f(1, 1, 1, 0.1), 5, 146+row_selected*15, 0);
            /* Gold */
              al_draw_textf(font18, al_map_rgb(240, 180, 0), 200, 325, ALLEGRO_ALIGN_CENTRE, "%i", (int)gold);

            al_draw_bitmap(dwarfImage, 490,320, 0 );
            al_draw_tinted_bitmap(dwarfImage, al_map_rgb(180, 100, 100), 580, 320, 0);
            al_draw_tinted_bitmap(dwarfImage, al_map_rgb(100, 200, 100), 660, 320, 0);

            al_draw_rectangle(404+unit_selected*90,325,463+unit_selected*90,383, al_map_rgb(255, 240, 0), 1);
            }

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	}
	//==============================================
	//DESTROY PROJECT OBJECTS
	//==============================================
	for(iter = objects.begin(); iter != objects.end(); )
	{
		(*iter)->Destroy();
		delete (*iter);
		iter = objects.erase(iter);
	}

	al_destroy_bitmap(dwarfImage);
	al_destroy_bitmap(goblinImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(uiImage);
	al_destroy_bitmap(hand);
	al_destroy_bitmap(goldIcon);
	al_destroy_bitmap(titleScreen);

	//SHELL OBJECTS=================================
	al_destroy_font(font18);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
/* Local Includes */
#include "objects.h"
#include "creature.h"
#include "Const.h"
#include "ftext.h"
#include "stage.h"
#include "squad.h"
#include "deployment.h"

/*
Prototype-level ToDo:
- Prevent Spawning Creatures on top of each other
- Map camera control
-Unit types: Trooper, Berserker(0%), Knight, Hero
-Enemies types: Troll(33%), Goblin(66%), Gnoll(33%), Ogre Leader
- UI icons for creatures
- Wrapping loading assess into functions/classes
- Wrapping unit stats into class/struct/file (separate array for stats, and separate array for animations)
- Implement smart attack range (based on creature size)

Classes:
- Implement Squad Class - responsible for holding stats, name, exp, inventory.
    can be put in deployment slot and is base for creature spawning. (80% done)
- Implement Deployment Class - a new deployment UI with options to list all squads, see their stats,
    select units and hero for the next mission.

Alpha-level ToDo:
- Writing/Loading from files (unit stats, save/load(serialization), player progress)
- Sounds and music
- Implement Resources(+gold bounty, +mana, +glory)
- Better combat (traits, special skills and different game mechanics)
- Stages/game modes/introduction campaign
*/
//==============================================
//GLOBALS
//==============================================


std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;

int STATE = MENU;

/* Stage variables */
int Stage::STAGE_VICTORY_CONDITION = 1;
int Stage::lives = 5;
int Stage::ObjectivesCount = 0;
float Stage::gold = 100;

/* GUI variables */
int row_selected = 1;
int unit_selected = 1;

/* Game Loop Functions */
void Remove_dead_objects(); //Usuwa martwe obiekty
void Remove_all_objects(); //Usuwa wszystkie obiekty
void Stage_reset(); //Resetuje wartosci stage'a

int main(int argc, char **argv)
{
//==============================================
//SHELL VARIABLES
//==============================================
    bool done = false;
    bool render = false;

//==============================================
//PROJECT VARIABLES
//==============================================
    ALLEGRO_BITMAP *troopImage = NULL;
	ALLEGRO_BITMAP *dwarfImage = NULL;
	ALLEGRO_BITMAP *goblinImage = NULL;
	ALLEGRO_BITMAP *trollImage = NULL;
	ALLEGRO_BITMAP *gnollImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *uiImage = NULL;
	ALLEGRO_BITMAP *hand = NULL;
	ALLEGRO_BITMAP *titleScreen = NULL;
	ALLEGRO_BITMAP *heartIcon = NULL;

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
	titleScreen = al_load_bitmap("title_screen.png");
	heartIcon = al_load_bitmap("heart_icon.png");
	al_convert_mask_to_alpha(heartIcon, al_map_rgb(80,80,200));
	hand = al_load_bitmap("hand.png");
	al_convert_mask_to_alpha(hand, al_map_rgb(255,0,0));
	/*Jednostki*/
	troopImage = al_load_bitmap("gfx/units/trooper.png");
	dwarfImage = al_load_bitmap("gfx/units/DwarfWarrior.png");
	al_convert_mask_to_alpha(dwarfImage, al_map_rgb(255,255,255));
	goblinImage = al_load_bitmap("gfx/units/GoblinPillager.png");
	trollImage = al_load_bitmap("gfx/units/TrollLeader.png");
	al_convert_mask_to_alpha(trollImage, al_map_rgb(255,0,0));
	gnollImage = al_load_bitmap("gfx/units/GnollAxeman.png");
	al_convert_mask_to_alpha(gnollImage, al_map_rgb(255,0,0));
//==============================================
//SQUAD HANDLING
//==============================================
    stats dwarf_stat = {12, 35, 1.8, 10};//DMG/HP/SPD/COST
    animation dwarf_anim = {0,0,0,0,0,0,dwarfImage}; //maxFrames/FrameDelay/FrameWidth/FrameHeight/AnimationColumns/AttackDelay
    Squad *dwarf_enlist = new Squad(DWARFKIN, dwarf_stat, dwarf_anim);
    stats troop_stat = {10, 16, 2.2, 8};
    animation troop_anim = {6, 8, 135, 105, 6,12, troopImage};
    Squad *trooper = new Squad(DWARFKIN, troop_stat, troop_anim);
    stats goblin_stat = {8, 18, 2.5, 0};
    animation goblin_anim = {4,6,125,100,4,12,goblinImage};
    Squad *goblin_pillager = new Squad(GREENSKINS, goblin_stat, goblin_anim);
    stats troll_stat = {30, 175, 1.5, 0};
    animation troll_anim = {8,6,125,115,8,12,trollImage};
    Squad *war_troll = new Squad(GREENSKINS, troll_stat, troll_anim);
    stats gnoll_stat = {14, 25, 2.2, 0};
    animation gnoll_anim = {8,5,99,79,8,12,gnollImage};
    Squad *gnoll_axeman = new Squad(GREENSKINS, gnoll_stat, gnoll_anim);

    Deployment Deployed(trooper, goblin_pillager, gnoll_axeman);

//==============================================
//TIMER INIT AND STARTUP
//==============================================
	srand(time(NULL));
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
                    if (STATE == MENU )
                    {
                    STATE = PLAYING;
                    break;
                    }
                    else if (STATE == DEFEAT)
                    {
                        Remove_all_objects();
                        Stage_reset();
                        break;
                    }
                    else if (STATE == PLAYING)
                    {
                        int gold_cost = Deployed.GetSelectedSlot(unit_selected).GetGoldCost();
                        if (Stage::GetStageGold() > gold_cost)
                            {
                                objects.push_back(Deployed.SpawnUnit(row_selected, unit_selected));
                                Stage::SpendGold(gold_cost);
                                Ftext *text = new Ftext(230, 325, -0.5, -(gold_cost), 45, font18);
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

            /*Enemy Spawning*/
            if((STATE == PLAYING) && rand() % 115 == 0)
            {
               if (rand() % 10 == 0)
               {
               Creature *troll = new Creature(WIDTH-20, rand() % 3 + 1, war_troll);
               objects.push_back(troll);
               }
               else if (rand() % 3 == 0)
               {
               Creature *gnoll = new Creature(WIDTH-20, rand() % 3 + 1, gnoll_axeman);
               objects.push_back(gnoll);
               }
               else
               {
               Creature *goblin = new Creature(WIDTH-20, rand() % 3 + 1, goblin_pillager);
               objects.push_back(goblin);
               }
            }

            /*Gold Generation*/
            if(STATE == PLAYING)
            Stage::AwardGold(0.04);

            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            //objects.sort([](GameObject * first, GameObject * second) {return first->GetY() < second->GetY();});

            /*Collision Checking*/
            if ( STATE == PLAYING || STATE == CUTSCENE)
            for(iter = objects.begin(); iter != objects.end(); ++iter)
            {
                if ( !(*iter)->GetAlive() || !(*iter)->GetSolid()) continue;
                (*iter)->SetAnim(WALKING);
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
                if( STATE == PLAYING || STATE == CUTSCENE)
                for(iter = objects.begin(); iter != objects.end(); ++iter)
                {
                     if ( !(*iter)->GetAlive()) continue;
                     {
                        (*iter)->Update();
                        (*iter)->SetMove(true);
                     }
                }

			//cull the dead
            Remove_dead_objects();

            //jesli stracimy hp to konczymy gre - observer pattern
			if (Stage::GetStageLives() < 1)
            {
                STATE = DEFEAT;
            }
			//jesli spelnimy objectives misji, to wygrywamy gre - observer pattern
            if (Stage::CheckVictoryCondition(Stage::GetObjectivesCount()) == true)
            {
                STATE = MENU;
                Remove_all_objects();
                Stage_reset();
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
			if (STATE == PLAYING || STATE == PAUSED || STATE == CUTSCENE)
            {
            al_draw_bitmap(bgImage, 0, 0, 0);
            al_draw_bitmap(uiImage, 0, 240, 0);
			//BEGIN PROJECT RENDER===============
            for(iter = objects.begin(); iter != objects.end(); ++iter)
              {
                  (*iter)->Render();
              }

            al_draw_bitmap_region(Deployed.OccupiedSlot_1->image, 0, 0,Deployed.OccupiedSlot_1->GetFrameWidth(),
                                    Deployed.OccupiedSlot_1->GetFrameHeight(), 400,320, 0);
            al_draw_bitmap_region(Deployed.OccupiedSlot_2->image, 0, 0,Deployed.OccupiedSlot_2->GetFrameWidth(),
                                    Deployed.OccupiedSlot_2->GetFrameHeight(), 555,300, 0);
            al_draw_bitmap_region(Deployed.OccupiedSlot_3->image, 0, 0,Deployed.OccupiedSlot_3->GetFrameWidth(),
                                    Deployed.OccupiedSlot_3->GetFrameHeight(), 635,300, 0);
            al_draw_rectangle(404+unit_selected*90,325,463+unit_selected*90,383, al_map_rgb(255, 240, 0), 1);
            /* Row Selected Cursor */
              al_draw_tinted_bitmap(hand, al_map_rgba_f(1, 1, 1, 0.1), 5, 146+row_selected*15, 0);
            /* UI text */
              if (STATE == PAUSED)
                al_draw_text(font18, al_map_rgb(255, 255, 255), 400, 30, ALLEGRO_ALIGN_CENTRE , "Game Paused");
              al_draw_textf(font18, al_map_rgb(240, 180, 0), 200, 325, ALLEGRO_ALIGN_CENTRE, "Gold: %i", (int)Stage::GetStageGold());
              al_draw_text(font18, al_map_rgb(255,255,255), 520, 275, ALLEGRO_ALIGN_CENTRE, "Dwarf");
              al_draw_text(font18, al_map_rgb(255,255,255), 610, 275, ALLEGRO_ALIGN_CENTRE, "Berserker");
              al_draw_text(font18, al_map_rgb(255,255,255), 700, 275, ALLEGRO_ALIGN_CENTRE, "Lord");
              al_draw_text(font18, al_map_rgb(240,180,0), 520, 295, ALLEGRO_ALIGN_CENTRE, "Gold: 10");
              al_draw_text(font18, al_map_rgb(240,180,0), 610, 295, ALLEGRO_ALIGN_CENTRE, "Gold: 25");
              al_draw_text(font18, al_map_rgb(240,180,0), 700, 295, ALLEGRO_ALIGN_CENTRE, "Gold: 40");
              for (int i = 0; i < Stage::GetStageLives(); i++)
              {
                  al_draw_bitmap(heartIcon,5+(i*30), 0, 0);
              }
              al_draw_textf(font18, al_map_rgb(240,30,0), 700, 10, ALLEGRO_ALIGN_CENTER, "Kills to win: %i", 40-(Stage::GetObjectivesCount()));
            }
              if (STATE == DEFEAT)
              {
                al_draw_text(font18, al_map_rgb(255, 30, 30), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "You have been defeated!");
                al_draw_text(font18, al_map_rgb(255, 30, 30), WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to restart!");
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
    al_destroy_bitmap(troopImage);
	al_destroy_bitmap(dwarfImage);
	al_destroy_bitmap(goblinImage);
	al_destroy_bitmap(trollImage);
	al_destroy_bitmap(gnollImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(uiImage);
	al_destroy_bitmap(hand);
	al_destroy_bitmap(titleScreen);
	al_destroy_bitmap(heartIcon);

	//SHELL OBJECTS=================================
	al_destroy_font(font18);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

    delete dwarf_enlist;
    delete trooper;
    delete goblin_pillager;
    delete war_troll;
    delete gnoll_axeman;

	return 0;
}

void Remove_dead_objects()
{
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
};

void Remove_all_objects()
{
			for(iter = objects.begin(); iter != objects.end(); )
			{
					delete (*iter);
					iter = objects.erase(iter);
			}
};

void Stage_reset() // Przeniesc jako metode do stage.h
{
     Stage::lives = 5;
     Stage::ObjectivesCount = 0;
     Stage::gold = 100;
     STATE = PLAYING;
}

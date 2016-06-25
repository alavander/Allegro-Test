#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <string>
#include <iostream>
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
- Attack speed should be a separate variable, and based on it, animations should be faster or slower.
- After attack there should be a short 'cooldown' period.
- Add death animation rendering and game logic handling.
-Unit types: Berserker, Knight, Hero
-Enemies types: Ogre Leader
- UI icons for creatures (50% done - need to show squad name and gold/honor cost)
- Add 'Honor' resource, gained by killing enemies (50%)
- Add 'Honor' icon and floating text upon killing enemy.
- Add troop type (common, uncommon, elite, hero) (50%)
- Basic squad selection at startup.
- Random game type at startup.

Code-tidying:
- Wrapping loading assess into functions/classes (loading at beginning, deleting at end)
- Wrapping unit stats into class/struct/file (separate array for stats, and separate array for animations)
- Make separate State class, for keeping game state.
- Make separate Input class, for dealing with input.
- Make separate Unit State class, for dealing with animation changes.


Classes:
- Implement Deployment Class - a new deployment UI with options to list all squads, see their stats,
    select units and hero for the next mission.

Alpha-level ToDo:
- Remove copy-by-value for stats/anim struct to squad
- Add ranged attack options
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
int Stage::cameraX = 0;
float Stage::gold = 100;

/* GUI variables */
int row_selected = 1;
int unit_selected = 1;
bool cameraLeft = false;
bool cameraRight = false;

/* Game Loop Functions */
void Remove_dead_objects(); //Usuwa martwe obiekty z listy
void Remove_all_objects(); //Usuwa wszystkie obiekty z listy
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
    ALLEGRO_BITMAP *soldierImage = NULL;
	ALLEGRO_BITMAP *barbarianImage = NULL;
	ALLEGRO_BITMAP *goblinImage = NULL;
	ALLEGRO_BITMAP *trollImage = NULL;
	ALLEGRO_BITMAP *gnollImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *uiImage = NULL;
	ALLEGRO_BITMAP *hand = NULL;
	ALLEGRO_BITMAP *titleScreen = NULL;
	ALLEGRO_BITMAP *heartIcon = NULL;
	ALLEGRO_BITMAP *gold_icon = NULL;
	ALLEGRO_BITMAP *attack_icon = NULL;
	ALLEGRO_BITMAP *unit_icon = NULL;
	ALLEGRO_BITMAP *deployment_map = NULL;

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

	display = al_create_display(SCREEN_WIDTH, HEIGHT);			//create our display object
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
	font18 = al_load_font("Data/dwarf.ttf", 20, 0);
    /*GUI*/
	bgImage = al_load_bitmap("Data/BG01.jpg");
	uiImage = al_load_bitmap("Data/background2.jpg");
	titleScreen = al_load_bitmap("Data/title_screen.png");
	heartIcon = al_load_bitmap("Data/heart_icon.png");
	al_convert_mask_to_alpha(heartIcon, al_map_rgb(80,80,200));
	hand = al_load_bitmap("Data/hand.png");
	al_convert_mask_to_alpha(hand, al_map_rgb(255,0,0));
	gold_icon = al_load_bitmap("Data/gold_icon.png");
	attack_icon = al_load_bitmap("Data/attack_icon.png");
	unit_icon = al_load_bitmap("Data/unit_icons_40x40.jpg");
	deployment_map = al_load_bitmap("Data/world_map.jpg");
	/*Jednostki*/
	soldierImage = al_load_bitmap("Data/units/soldier.png");
	barbarianImage = al_load_bitmap("Data/units/barbarian.png");
	goblinImage = al_load_bitmap("Data/units/goblin_pillager.png");
	trollImage = al_load_bitmap("Data/units/troll.png");
	gnollImage = al_load_bitmap("Data/units/gnoll_axeman.png");
//==============================================
//SQUAD HANDLING
//==============================================
    stats barbarian_stat = {19, 35, 1.7, 18};//DMG/HP/SPD/COST
    animation barbarian_anim = {6,6,120,100,9,10,barbarianImage, 2}; //maxFrames/FrameDelay/FrameWidth/FrameHeight/AnimationColumns/AttackDelay/IMG/ICON
    Squad *barbarian = new Squad(DWARFKIN, barbarian_stat, barbarian_anim);
    stats soldier_stat = {11, 21, 2.2, 10};
    animation soldier_anim = {9, 6, 150, 150,9,12, soldierImage,1};
    Squad *soldier = new Squad(DWARFKIN, soldier_stat, soldier_anim);
    stats goblin_stat = {8, 18, 2.5, 0};
    animation goblin_anim = {4,6,125,100,4,15,goblinImage,0};
    Squad *goblin_pillager = new Squad(GREENSKINS, goblin_stat, goblin_anim);
    stats troll_stat = {30, 175, 1.5, 0};
    animation troll_anim = {8,6,250,200,8,18,trollImage,0};
    Squad *war_troll = new Squad(GREENSKINS, troll_stat, troll_anim);
    stats gnoll_stat = {14, 25, 2.2, 0};
    animation gnoll_anim = {8,5,220,175,8,12,gnollImage,0};
    Squad *gnoll_axeman = new Squad(GREENSKINS, gnoll_stat, gnoll_anim);

    Deployment Deployed(soldier, barbarian, gnoll_axeman);

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
                cameraLeft = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                cameraRight = true;
                break;
            case ALLEGRO_KEY_1:
                unit_selected = 1;
                break;
            case ALLEGRO_KEY_2:
                unit_selected = 2;
                break;
            case ALLEGRO_KEY_3:
                unit_selected = 3;
                break;
            case ALLEGRO_KEY_4:
                unit_selected = 4;
                break;
            case ALLEGRO_KEY_SPACE:
                    if (STATE == MENU )
                    {
                    STATE = DEPLOYMENT;
                    break;
                    }
                    if (STATE == DEPLOYMENT )
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
                                Ftext *text = new Ftext(210, 550, -0.5, -(gold_cost), 45, font18, gold_icon);
                                objects.push_back(text);
                                break;
                            }
                    }
		}
		}
		if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
            case ALLEGRO_KEY_LEFT:
                cameraLeft = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                cameraRight = false;
                break;
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
               Creature *troll = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, war_troll);
               objects.push_back(troll);
               }
               else if (rand() % 3 == 0)
               {
               Creature *gnoll = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, gnoll_axeman);
               objects.push_back(gnoll);
               }
               else
               {
               Creature *goblin = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, goblin_pillager);
               objects.push_back(goblin);
               }
            }

            /*Camera*/
            if (cameraLeft == true)
                if (Stage::cameraX < 1 ? Stage::cameraX = 0 : Stage::cameraX -= 5);
            if (cameraRight == true)
                if (Stage::cameraX > 799 ? Stage::cameraX = 800 : Stage::cameraX += 5);


            /*Gold Generation*/
            if(STATE == PLAYING)
            Stage::AwardGold(0.04);

            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            objects.sort([](GameObject * first, GameObject * second) {return first->GetY() < second->GetY();});

            /*Collision Checking*/
            if ( STATE == PLAYING)
            for(iter = objects.begin(); iter != objects.end(); ++iter)
            {
                if ( !(*iter)->GetAlive() || !(*iter)->GetSolid())continue;
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
                                Ftext *text = new Ftext(((*iter2)->GetX()-Stage::cameraX), (*iter2)->PositionY(), 0.7, damage, 45, font18, attack_icon);
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
                        (*iter)->Update();
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
            if (STATE == DEPLOYMENT)
            {
            al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgb(0,0,0));//t³o
            al_draw_text(font18, al_map_rgb(255,255,255), 215, 5, ALLEGRO_ALIGN_CENTER, "Map" );
            al_draw_bitmap(deployment_map,30,30,0);
            al_draw_text(font18, al_map_rgb(255,255,255), (460+750)/2, 5, ALLEGRO_ALIGN_CENTER, "Squad Selection" );
            al_draw_filled_rectangle(460, 30, 750, 330, al_map_rgb(255,255,255));//squad selection
            al_draw_filled_rectangle(600, 500, 750, 530, al_map_rgb(255,255,255));//start button
            al_draw_text(font18, al_map_rgb(0,0,0), (600+750)/2, 505, ALLEGRO_ALIGN_CENTER, "Start Mission" );
            }
			if (STATE == PLAYING || STATE == PAUSED )
            {
            al_draw_bitmap_region(bgImage, 0+Stage::cameraX, 0, SCREEN_WIDTH, HEIGHT, 0, 0, 0);
            al_draw_bitmap(uiImage, 0, 500, 0);
			//BEGIN PROJECT RENDER===============
            for(iter = objects.begin(); iter != objects.end(); ++iter)
              {
                  (*iter)->Render();
              }
            //Icons!
            al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_1->GetIconNumber(), 0,40, 40, 400,560, 0);
            al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_2->GetIconNumber(), 0,40, 40, 500,560, 0);
            al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_3->GetIconNumber(), 0,40, 40, 600,560, 0);
            al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_3->GetIconNumber(), 0,40, 40, 700,560, 0);
            al_draw_rectangle(300+unit_selected*100,560,340+unit_selected*100,600, al_map_rgb(255, 240, 0), 1);
            /* Row Selected Cursor */
              al_draw_tinted_bitmap(hand, al_map_rgba_f(1, 1, 1, 0.1), 5, 410+row_selected*20, 0);
            /* UI text */
              if (STATE == PAUSED)
                al_draw_text(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 30, ALLEGRO_ALIGN_CENTRE , "Game Paused");
                al_draw_textf(font18, al_map_rgb(240, 180, 0), SCREEN_WIDTH/5, 550, ALLEGRO_ALIGN_CENTRE, "Gold: %i", (int)Stage::GetStageGold());

              for (int i = 0; i < Stage::GetStageLives(); i++)
              {
                  al_draw_bitmap(heartIcon,5+(i*30), 0, 0);
              }
              al_draw_textf(font18, al_map_rgb(240,30,0), SCREEN_WIDTH/1.2, 10, ALLEGRO_ALIGN_CENTER, "Kills to win: %i", 40-(Stage::GetObjectivesCount()));
              al_draw_textf(font18, al_map_rgb(240,240,240), 65, 30, ALLEGRO_ALIGN_CENTER, "CameraX: %i", Stage::cameraX);
            }
              if (STATE == DEFEAT)
              {
                al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "You have been defeated!");
                al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to restart!");
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
    al_destroy_bitmap(soldierImage);
	al_destroy_bitmap(barbarianImage);
	al_destroy_bitmap(goblinImage);
	al_destroy_bitmap(trollImage);
	al_destroy_bitmap(gnollImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(uiImage);
	al_destroy_bitmap(hand);
	al_destroy_bitmap(titleScreen);
	al_destroy_bitmap(heartIcon);
	al_destroy_bitmap(attack_icon);
    al_destroy_bitmap(gold_icon);
    al_destroy_bitmap(unit_icon);
    al_destroy_bitmap(deployment_map);

	//SHELL OBJECTS=================================
	al_destroy_font(font18);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

    delete barbarian;
    delete soldier;
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
     Stage::cameraX = 0;
     STATE = PLAYING;
}

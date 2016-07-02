#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>
#include <iterator>
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
- Menu->Deployment: A stage list used to generate three stages for deployment screen
- Deployment: A stage select, used to select one stage for playing
- Deployment: Print if the unit is selected for a slot
- Deployment: When selecting a unit for slot, check if its present in other slots -
                if yes, automatically de-select it from two other slots if its the same.
- Stage: Stage should have: background, objectives, enemy type, diffaulty, description, picture
- Stage: A stage should also have enemy types and enemy hero type
- Stage: A stage should have stage traits
- Consolidated 'remove object' functions into one, with various flags. (done)
- Aftermatch: defeat screens, victory screen (variable in Stage -> showed in the aftermatch) (done)
- Clearing not currently selected stages when going from deployment to stage
- Player class, which holds the squad list variable, number of victories, number of defeats
- Better main menu
- Cleaning currently selected stage when in menu screen
- Better UI for state playing
- Add stage time pass as victory condition of siege (18000 is 5 minutes)
- Add enemy hero killing as base victory condition for hero hunting
- Add losing hero as base defeat condition in bloodbath and hero hunting
- Add hero at the base of level for Bloodbath and Hero Hunting.
- Add 'Honor' icon. (done)
- Show gold or honor actually only if unit needs it.

Code-tidying:
- Wrapping loading assess into functions/classes (loading at beginning, deleting at end)
- Wrapping unit stats into class/struct/file (separate array for stats, and separate array for animations)
- Make separate Input class, for dealing with input.

Classes:
- Implement Deployment Class - a new deployment UI with options to list all squads, see their stats,
    select units and hero for the next mission. (50% DONE)

Alpha-level ToDo:
- Attack speed should be a separate variable, and based on it, animations should be faster or slower.
- After attack there should be a short 'cooldown' period.
- Remove copy-by-value for stats/anim struct to squad
- Add ranged attack options
- Writing/Loading from files (unit stats, save/load(serialization), player progress)
- Sounds and music
- Better combat (traits, special skills and different game mechanics)
- Stages/game modes/introduction campaign
*/
//==============================================
//GLOBALS
//==============================================

std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;

std::list<Squad *> AvailableSquads;
std::list<Squad *>::iterator sqiter;


/* GUI variables */
int row_selected = 1;
int unit_selected = 1;
bool cameraLeft = false;
bool cameraRight = false;
int StageTimeElapsed = 0;
int STATE = MENU;

/* Stage variables */
int Stage::STAGE_VICTORY_CONDITION = 1;
int Stage::AftermatchStatus = UNRESOLVED;
int Stage::lives = 5;
int Stage::rareNumber = 0;
int Stage::uncommonNumber = 0;
int Stage::ObjectivesCount = 0;
int Stage::cameraX = 0;
float Stage::gold = 100;
int Stage::honor = 0;

/* Game Loop Functions */
void Remove_objects(int remove_flag); //Usuwa obiekty w zaleznosci od parametru
void Stage_init(); //Resetuje wartosci stage'a

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
    ALLEGRO_BITMAP *squireImage = NULL;
    ALLEGRO_BITMAP *heroImage = NULL;
    ALLEGRO_BITMAP *goblinImage = NULL;
    ALLEGRO_BITMAP *trollImage = NULL;
    ALLEGRO_BITMAP *gnollImage = NULL;
    ALLEGRO_BITMAP *ogreImage = NULL;
    ALLEGRO_BITMAP *skeletonImage = NULL;
    ALLEGRO_BITMAP *bgImage = NULL;
    ALLEGRO_BITMAP *uiImage = NULL;
    ALLEGRO_BITMAP *titleScreen = NULL;
    ALLEGRO_BITMAP *heartIcon = NULL;
    ALLEGRO_BITMAP *icons = NULL;
    ALLEGRO_BITMAP *unit_icon = NULL;
    ALLEGRO_BITMAP *deployment_map = NULL;
    ALLEGRO_SAMPLE *song = NULL;
//==============================================
//ALLEGRO VARIABLES
//==============================================
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *font18, *font12;

//==============================================
//ALLEGRO INIT FUNCTIONS
//==============================================
    if(!al_init())										//initialize Allegro
        return -1;

    display = al_create_display(SCREEN_WIDTH, HEIGHT);			//create our display object
    al_set_window_title(display, "Undertales of Drakern");
    al_hide_mouse_cursor(display);

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
    al_install_audio();
    al_init_acodec_addon();

//==============================================
//PROJECT INIT
//==============================================
    /*FONT*/
    font18 = al_load_font("Data/dwarf.ttf", 18, 0);
    font12 = al_load_ttf_font("Data/dwarf.ttf", 12, 0);
    /*MUSIC*/
    al_reserve_samples(1);
    /*GUI*/
    bgImage = al_load_bitmap("Data/BG01.jpg");
    uiImage = al_load_bitmap("Data/background2.jpg");
    titleScreen = al_load_bitmap("Data/title_screen.png");
    heartIcon = al_load_bitmap("Data/heart_icon.png");
    al_convert_mask_to_alpha(heartIcon, al_map_rgb(80,80,200));
    icons = al_load_bitmap("Data/icons.png");
    unit_icon = al_load_bitmap("Data/unit_icons_40x40.jpg");
    deployment_map = al_load_bitmap("Data/world_map.jpg");
    /*Jednostki*/
    soldierImage = al_load_bitmap("Data/units/soldier.png");
    barbarianImage = al_load_bitmap("Data/units/barbarian.png");
    squireImage = al_load_bitmap("Data/units/squire.png");
    heroImage = al_load_bitmap("Data/units/hero.png");
    goblinImage = al_load_bitmap("Data/units/goblin_pillager.png");
    trollImage = al_load_bitmap("Data/units/troll.png");
    gnollImage = al_load_bitmap("Data/units/gnoll_axeman.png");
    ogreImage = al_load_bitmap("Data/units/ogre_leader.png");
    skeletonImage = al_load_bitmap("Data/units/skeleton.png");
//==============================================
//SQUAD HANDLING
//==============================================
    stats soldier_stat = {9, 15, 1.95, 10, 0, "Footman", COMMON};
    animation soldier_anim = {9, 6, 150, 150,9,12, soldierImage,1};
    Squad *soldier = new Squad(DWARFKIN, soldier_stat, soldier_anim);
    AvailableSquads.push_back(soldier);
    /*============================================================================*/
    stats barbarian_stat = {18, 28, 1.9, 20, 0, "Barbarian", UNCOMMON};//DMG/HP/SPD/COST/HONOR/SQUAD_NAME
    animation barbarian_anim = {6,6,155,128,6,12,barbarianImage, 2}; //maxFrames/FrameDelay/FrameWidth/FrameHeight/AnimationColumns/AttackDelay/IMG/ICON
    Squad *barbarian = new Squad(DWARFKIN, barbarian_stat, barbarian_anim);
    AvailableSquads.push_back(barbarian);
    /*============================================================================*/
    stats hero_stat = {25, 250, 1, 10, 0, "Hero", LEGENDARY};
    animation hero_anim = {11, 4, 250, 200, 11, 8, heroImage, 4};
    Squad *hero = new Squad(DWARFKIN, hero_stat, hero_anim);
    AvailableSquads.push_back(hero);
    /*============================================================================*/
    stats squire_stat = {16, 70, 1.6, 30, 4, "Squire", RARE};
    animation squire_anim = {7, 7, 200, 100, 7, 10, squireImage, 3};
    Squad *squire = new Squad(DWARFKIN, squire_stat, squire_anim);
    AvailableSquads.push_back(squire);
    /*============================================================================*/
    stats goblin_stat = {5, 12, 3.64, 0, 0, "", COMMON};
    animation goblin_anim = {4,6,125,100,4,15,goblinImage,0};
    Squad *goblin_pillager = new Squad(GREENSKINS, goblin_stat, goblin_anim);
    AvailableSquads.push_back(goblin_pillager);
    /*============================================================================*/
    stats troll_stat = {20, 92, 1.2, 0, 0,"", RARE};
    animation troll_anim = {8,6,250,200,8,15,trollImage,0};
    Squad *war_troll = new Squad(GREENSKINS, troll_stat, troll_anim);
    AvailableSquads.push_back(war_troll);
    /*============================================================================*/
    stats gnoll_stat = {17, 21, 1.9, 0, 0,"", UNCOMMON};
    animation gnoll_anim = {8,5,220,175,8,12,gnollImage,0};
    Squad *gnoll_axeman = new Squad(GREENSKINS, gnoll_stat, gnoll_anim);
    AvailableSquads.push_back(gnoll_axeman);
    /*============================================================================*/
    stats ogre_stat = {25, 250, 1, 10, 0,"", LEGENDARY};
    animation ogre_anim = {8, 4, 250, 200, 8, 12, ogreImage, 0};
    Squad *ogre = new Squad(GREENSKINS, ogre_stat, ogre_anim);
    AvailableSquads.push_back(ogre);
    /*============================================================================*/
    animation skel_anim = {7, 5, 150, 100, 7, 10, skeletonImage, 0};
    /*============================================================================*/
    sqiter = AvailableSquads.begin();
    Deployment Deployed(soldier, barbarian, squire);
//==============================================
//MUSIC
//==============================================
    song = al_load_sample("Data/music/HeroicDemise.ogg");
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
                if (STATE == PLAYING || STATE == PAUSED || STATE == AFTERMATCH) cameraLeft = true;
                if (STATE == DEPLOYMENT)//wybor squada podczas deploymentu
                {
                    if (sqiter == AvailableSquads.begin()) sqiter = std::prev(std::prev(AvailableSquads.end()));
                    else sqiter--;
                    while ((*sqiter)->GetFraction() != DWARFKIN || (*sqiter)->GetUnitType() == LEGENDARY)
                    {
                        if(sqiter == AvailableSquads.begin()) sqiter = std::prev(std::prev(AvailableSquads.end()));
                        else sqiter--;
                    }
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if (STATE == PLAYING || STATE == PAUSED || STATE == AFTERMATCH)cameraRight = true;
                if (STATE == DEPLOYMENT)//Wybor squada podczas deploymentu
                {
                    if (sqiter == std::prev(std::prev(AvailableSquads.end()))) sqiter = AvailableSquads.begin();
                    else sqiter++;
                    while ((*sqiter)->GetFraction() != DWARFKIN || (*sqiter)->GetUnitType() == LEGENDARY)
                    {
                        if(sqiter == std::prev(std::prev(AvailableSquads.end()))) sqiter = AvailableSquads.begin();
                        else sqiter++;
                    }
                }
                break;
            case ALLEGRO_KEY_1:
                unit_selected = 1;
                if (STATE == DEPLOYMENT)
                {
                    //if((*sqiter)->GetSquadName() != Deployed.OccupiedSlot_2->GetSquadName() ||
                    //   (*sqiter)->GetSquadName() != Deployed.OccupiedSlot_3->GetSquadName())
                    //{
                    Deployed.OccupiedSlot_1->SetDeploying(0);
                    Deployed.SelectSquadForDeployment((*sqiter), 1);
                    (*sqiter)->SetDeploying(1);
                    //}
                }
                break;
            case ALLEGRO_KEY_2:
                unit_selected = 2;
                if (STATE == DEPLOYMENT)
                {
                   // if((*sqiter)->GetSquadName() != Deployed.OccupiedSlot_1->GetSquadName() ||
                    //   (*sqiter)->GetSquadName() != Deployed.OccupiedSlot_3->GetSquadName())
                    //{
                    Deployed.OccupiedSlot_2->SetDeploying(0);
                    Deployed.SelectSquadForDeployment((*sqiter), 2);
                    (*sqiter)->SetDeploying(2);
                   // }
                }
                break;
            case ALLEGRO_KEY_3:
                unit_selected = 3;
                if (STATE == DEPLOYMENT)
                {
                   // if((*sqiter)->GetSquadName() != Deployed.OccupiedSlot_1->GetSquadName() ||
                   //    (*sqiter)->GetSquadName() != Deployed.OccupiedSlot_2->GetSquadName())
                   // {
                    Deployed.OccupiedSlot_3->SetDeploying(0);
                    Deployed.SelectSquadForDeployment((*sqiter), 3);
                    (*sqiter)->SetDeploying(3);
                   // }
                }
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
                    Stage_init();
                    al_play_sample(song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    break;
                }
                if (STATE == AFTERMATCH)
                {
                    Remove_objects(allObj);
                    STATE = MENU;
                    break;
                }
                if (STATE == PLAYING)
                {
                    int gold_cost = Deployed.GetSelectedSlot(unit_selected).GetGoldCost();
                    int honor_cost = Deployed.GetSelectedSlot(unit_selected).GetHonorCost();
                    if (Stage::GetStageGold() >= gold_cost && Stage::GetStageHonor() >= honor_cost)
                    {
                        objects.push_back(Deployed.SpawnUnit(row_selected, unit_selected));
                        Stage::SpendGold(gold_cost);
                        Ftext *text = new Ftext(250, 545, -0.5, -(gold_cost), 45, font18, icons, Gold);
                        objects.push_back(text);
                        if (honor_cost > 0)
                        {
                            Stage::SpendHonor(honor_cost);
                            Ftext *text = new Ftext(250, 570, -0.5, -(honor_cost), 45, font18, icons, Honor);
                            objects.push_back(text);
                        }
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
            if((STATE == PLAYING) && rand() % 100 == 0)
            {
                if (rand() % 10 == 0 && Stage::GetRareNumber() < EnemyEliteCount)
                {
                    Creature *troll = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, war_troll);
                    objects.push_back(troll);
                }
                else if (rand() % 3 == 0 && Stage::GetUncommonNumber() < EnemyHeavyCount)
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
                if (Stage::cameraX < 1 ? Stage::cameraX = 0 : Stage::cameraX -= 6);
            if (cameraRight == true)
                if (Stage::cameraX > 799 ? Stage::cameraX = 800 : Stage::cameraX += 6);


            /*Gold Generation*/
            if(STATE == PLAYING)
                Stage::AwardGold(0.04);

            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            objects.sort([](GameObject * first, GameObject * second)
            {
                return first->GetY() < second->GetY();
            });

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
                                    Ftext *text = new Ftext(((*iter2)->GetX()-Stage::cameraX), (*iter2)->PositionY(), 0.7, damage, 45, font18, icons, Damage);
                                    objects.push_back(text);
                                }
                            }
                        }
                    }
                };
            //Updating
            if( STATE == PLAYING)
                for(iter = objects.begin(); iter != objects.end(); ++iter)
                {
                    if ( !(*iter)->GetAlive()) continue;
                    {
                        (*iter)->Update();
                    }
                }

            //cull the dead
            Remove_objects(deadObj);

            //jesli stracimy hp to konczymy gre - observer pattern
            if (Stage::GetStageLives() < 1)
            {
                Stage::SetAftermatchStatus(DEFEAT);
                STATE = AFTERMATCH;
                Remove_objects(miscObj);//Usuwamy m.in floating text
                al_stop_samples();
            }
            //jesli spelnimy objectives misji, to wygrywamy gre - observer pattern?
            if (Stage::CheckVictoryCondition(Stage::GetObjectivesCount()) == true)
            {
                Stage::SetAftermatchStatus(VICTORY);
                STATE = AFTERMATCH;
                Remove_objects(miscObj);
                al_stop_samples();
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
                    al_draw_text(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, HEIGHT/1.2, ALLEGRO_ALIGN_CENTRE, "Press spacebar to start");
                }
                if (STATE == DEPLOYMENT)
                {
                    al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgb(0,0,0));//tlo
                    al_draw_text(font18, al_map_rgb(255,255,255), 215, 5, ALLEGRO_ALIGN_CENTER, "Map" );
                    al_draw_bitmap(deployment_map,30,30,0);
                    al_draw_text(font18, al_map_rgb(255,255,255), (460+750)/2, 5, ALLEGRO_ALIGN_CENTER, "Squad Selection" );
                    al_draw_filled_rectangle(580, 500, 780, 530, al_map_rgb(255,255,255));//start button
                    al_draw_text(font18, al_map_rgb(0,0,0), (600+750)/2, 505, ALLEGRO_ALIGN_CENTER, "Start Mission" );
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 40, 0, "Squad Name: %s",(*sqiter)->GetSquadName().c_str());
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 60, 0, "Level: %i (exp:%i)",(*sqiter)->GetLevel(),(int)(*sqiter)->GetSquadXP());
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 80, 0, "ATK: %i",(*sqiter)->GetDamage());
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 100, 0, "HP: %i",(*sqiter)->GetHp());
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 120, 0, "SPEED: %f",(*sqiter)->GetSpeed());
                    al_draw_textf(font18, al_map_rgb(255,255,255), 460, 140, 0, "Gold Cost: %i",(*sqiter)->GetGoldCost());
                    if ((*sqiter)->GetHonorCost() > 0)
                        al_draw_textf(font18, al_map_rgb(255,255,255), 460, 160, 0, "Honor Cost: %i",(*sqiter)->GetHonorCost());
                    if ((*sqiter)->GetDeploying() > 0)
                        al_draw_textf(font18, al_map_rgb(255,255,255), 460, 200, 0, "Unit is selected for slot: %i",(*sqiter)->GetDeploying());
                    al_draw_text(font18, al_map_rgb(255,255,255), 460, 240, 0, "1.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_1->GetIconNumber(), 0,40, 40, 480,240, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 530, 240, 0, "%s", Deployed.OccupiedSlot_1->GetSquadName().c_str());
                    al_draw_text(font18, al_map_rgb(255,255,255), 460, 300, 0, "2.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_2->GetIconNumber(), 0,40, 40, 480,300, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 530, 300, 0, "%s", Deployed.OccupiedSlot_2->GetSquadName().c_str());
                    al_draw_text(font18, al_map_rgb(255,255,255), 460, 360, 0, "3.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_3->GetIconNumber(), 0,40, 40, 480,360, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 530, 360, 0, "%s", Deployed.OccupiedSlot_3->GetSquadName().c_str());
                }
                if (STATE == PLAYING || STATE == PAUSED || STATE == AFTERMATCH )
                {
                    al_draw_bitmap_region(bgImage, 0+Stage::cameraX, 0, SCREEN_WIDTH, HEIGHT, 0, 0, 0);
                    al_draw_bitmap(uiImage, 0, 500, 0);
                    //BEGIN PROJECT RENDER===============
                    for(iter = objects.begin(); iter != objects.end(); ++iter)
                    {
                        (*iter)->Render();
                    }
                    //Icons!
                    al_draw_text(font18, al_map_rgb(255,255,255), 380, 570, 0, "1.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_1->GetIconNumber(), 0,40, 40, 400,560, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 420, 540, ALLEGRO_ALIGN_CENTER, "%s(%i)", Deployed.OccupiedSlot_1->GetSquadName().c_str(),Deployed.OccupiedSlot_1->GetGoldCost());
                    al_draw_text(font18, al_map_rgb(255,255,255), 480, 570, 0, "2.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_2->GetIconNumber(), 0,40, 40, 500,560, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 520, 540, ALLEGRO_ALIGN_CENTER, "%s(%i)", Deployed.OccupiedSlot_2->GetSquadName().c_str(),Deployed.OccupiedSlot_2->GetGoldCost());
                    al_draw_text(font18, al_map_rgb(255,255,255), 580, 570, 0, "3.");
                    al_draw_bitmap_region(unit_icon, 40*Deployed.OccupiedSlot_3->GetIconNumber(), 0,40, 40, 600,560, 0);
                    al_draw_textf(font12, al_map_rgb(255,255,255), 620, 540, ALLEGRO_ALIGN_CENTER, "%s(%i)", Deployed.OccupiedSlot_3->GetSquadName().c_str(),Deployed.OccupiedSlot_3->GetGoldCost());
                    al_draw_rectangle(300+unit_selected*100,560,340+unit_selected*100,600, al_map_rgb(255, 240, 0), 1);
                    /* Row Selected Cursor */
                    al_draw_textf(font12, al_map_rgb(255,255,255), 5, 410+row_selected*20, 0,"%i. lane>", row_selected);
                    /* UI text */
                    if (STATE == PAUSED)
                    {
                        al_draw_text(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 30, ALLEGRO_ALIGN_CENTRE, "Game Paused");
                        //al_draw_textf(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTRE, "Enemy Rare count: %i", Stage::GetRareNumber());
                        //al_draw_textf(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 70, ALLEGRO_ALIGN_CENTRE, "Enemy Uncommon count: %i", Stage::GetUncommonNumber());
                        //al_draw_textf(font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 90, ALLEGRO_ALIGN_CENTRE, "Stage Time Elapsed: %i", StageTimeElapsed);
                    }
                    al_draw_textf(font18, al_map_rgb(240, 180, 0), SCREEN_WIDTH/5, 545, ALLEGRO_ALIGN_CENTRE, "Gold: %i", (int)Stage::GetStageGold());
                    al_draw_bitmap_region(icons, 20, 0, 10, 10, SCREEN_WIDTH/5+60, 545, 0);
                    al_draw_textf(font18, al_map_rgb(255,255,255), SCREEN_WIDTH/5, 570, ALLEGRO_ALIGN_CENTRE, "Honor: %i", Stage::GetStageHonor());
                    al_draw_bitmap_region(icons, 30, 0, 10, 10, SCREEN_WIDTH/5+60, 570, 0);

                    for (int i = 0; i < Stage::GetStageLives(); i++)
                    {
                        al_draw_bitmap(heartIcon,5+(i*30), 0, 0);
                    }
                    if (Stage::STAGE_VICTORY_CONDITION == BLOODBATH)
                        al_draw_textf(font18, al_map_rgb(240,30,0), SCREEN_WIDTH/1.2, 10, ALLEGRO_ALIGN_CENTER, "Kills to win: %i", 40-(Stage::GetObjectivesCount()));


                    if (STATE == PLAYING ) StageTimeElapsed++;
                    al_draw_textf(font18, al_map_rgb(255,255,255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTER, "%i:%i",(StageTimeElapsed/60/60),(StageTimeElapsed/60)%60);
                }
                if (STATE == AFTERMATCH)
                {
                    if(Stage::GetAftermachStatus() == DEFEAT)
                    {
                    al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "You have been defeated!");
                    al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to restart!");
                    }
                    if(Stage::GetAftermachStatus() == VICTORY)
                    {
                    al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "You have proved victorious!");
                    al_draw_text(font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to start next mission!");
                    }
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

    for(sqiter = AvailableSquads.begin(); sqiter != AvailableSquads.end(); )
    {
        delete (*sqiter);
        sqiter = AvailableSquads.erase(sqiter);
    }
    al_destroy_bitmap(soldierImage);
    al_destroy_bitmap(barbarianImage);
    al_destroy_bitmap(goblinImage);
    al_destroy_bitmap(trollImage);
    al_destroy_bitmap(ogreImage);
    al_destroy_bitmap(squireImage);
    al_destroy_bitmap(heroImage);
    al_destroy_bitmap(gnollImage);
    al_destroy_bitmap(skeletonImage);
    al_destroy_bitmap(bgImage);
    al_destroy_bitmap(uiImage);
    al_destroy_bitmap(titleScreen);
    al_destroy_bitmap(heartIcon);
    al_destroy_bitmap(icons);
    al_destroy_bitmap(unit_icon);
    al_destroy_bitmap(deployment_map);


    //SHELL OBJECTS=================================
    al_destroy_font(font18);
    al_destroy_font(font12);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_sample(song);

    return 0;
}

void Remove_objects(int remove_flag)
{
    switch(remove_flag)
    {
    case deadObj:
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
    break;
    }
    case allObj:
    {
        for(iter = objects.begin(); iter != objects.end(); )
    {
        delete (*iter);
        iter = objects.erase(iter);
    }
    break;
    }
     case miscObj:
    {
    for(iter = objects.begin(); iter != objects.end(); )
    {
        if((*iter)->GetID() == MISC)
        {
            delete (*iter);
            iter = objects.erase(iter);
        }
        else
            iter++;
    }
    break;
    }
    }
};

void Stage_init() // Przeniesc jako metode do stage.h
{
    Stage::STAGE_VICTORY_CONDITION = 1;
    Stage::lives = 5;
    Stage::rareNumber = 0;
    Stage::uncommonNumber = 0;
    Stage::ObjectivesCount = 0;
    Stage::cameraX = 0;
    Stage::gold = 100;
    Stage::honor = 0;
    StageTimeElapsed = 0;
    STATE = PLAYING;
    Stage::SetAftermatchStatus(UNRESOLVED);
}

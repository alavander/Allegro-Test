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
#include <fstream>
/* Local Includes */
#include "game_state.h"
#include "player.h"
#include "handler.h"
#include "objects.h"
#include "creature.h"
#include "Const.h"
#include "ftext.h"
#include "stage.h"
#include "squad.h"
#include "deployment.h"
#include "gui.h"
#include "data.h"
#include "input.h"

/*
- Deployment: When selecting a unit for slot, check if its present in other slots -
                if yes, automatically de-select it from two other slots if its the same.
- Stage: Stage should have: description, picture (75% done)
- Stage: A stage should also have enemy types and enemy hero type
- Stage: A stage should have stage traits
- Menu screen (New Game, Continue Game(if have a valid save in saves directory), Exit)
- Stages should have stage level (on which we will base enemy stats)
- Hero should be able to level up and spend skill points on skills, in separate window
- Add talents(passive specials), which will be available directly from squad - a list, or array of talents, which will modify the way the game plays
- Add skills(active specials), which can be used by creatures, and have a cooldown assiocated with them
- Show the stage location on map (GUI)
- Options: music volume/turn music on/off
- Change lanes name (top,middle,bottom)
- ESC should ask if you wish to quit y/n, as well as pressing 'x'.
- Hp bar if unit is not at 100% hp
- Hero and enemy hero HP bars at top
- Mouse control

Classes:
        Combat Class -> Melee Combat, Ranged Combat, Unit Special Skills Usage, Hp/Dam, Resists, Cooldowns
        *GUI Class, -> Button, Selection Window, Information Window (50% done)
            - Message Class, -> Popup windows containing various informations
            - Ftext Class, -> Move to be part of GUI class, instead of GameObject Class
            - Button Class, -> For clicking
        Input Class, -> Forwarding Pressed Keys/Buttons/Selection Windows to GameState Class (singleton) (done)
        CombatStatus Class,-> Combat Buff, Debuff (stats+/- to targets, damage/healing to targets, stun status)
        Player Class,-> Number of victories, Number of defeats, killed enemies, achivements/unlocks, save games (20% done)
        Hero Class, -> Covers the use of special abilities and hero leveling,
        Audio Class,-> Playing, Stopping sounds and music, audio options (singleton)
        *Data Class,-> For loading assets (base squad stats from txt files, images, sounds and fonts data) (75% done)

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
/* GUI variables */
bool sound_enabled = true;

int main(int argc, char **argv)
{
//==============================================
//SHELL VARIABLES
//==============================================
    bool done = false;
    bool render = false;
//==============================================
//ALLEGRO VARIABLES
//==============================================
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_SAMPLE *song = NULL;
//==============================================
//ALLEGRO INIT FUNCTIONS
//==============================================
    if(!al_init())										//initialize Allegro
        return -1;

    display = al_create_display(SCREEN_WIDTH, HEIGHT);			//create our display object
    al_set_window_title(display, "Tales of Drakern");
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
    /*MUSIC*/
    al_reserve_samples(1);
//==============================================
//INITIALIZATING RANDOM SEED
//==============================================
    srand(time(NULL));
//==============================================
//INITIALIZATING INITIAL SETUP
//==============================================
    GameState State;    //Game State Machine
    Data data;          //Assets
    Handler handler;    //Game Lists
    Player player;      //Player Initialization
//==============================================
//SQUAD HANDLING
//==============================================
    stats soldier_stat = {"Footman", 9, 19, 1.95, 8, 0, COMMON};
    animation soldier_anim = {"Footman",9, 6, 150, 150,9,12, data.soldierImage,1};
    Squad *soldier = new Squad(DWARFKIN, soldier_stat, soldier_anim);
    handler.AvailableSquads.push_back(soldier);
    /*============================================================================*/
    stats barbarian_stat = {"Barbarian",18, 28, 1.9, 20, 0,  UNCOMMON};//DMG/HP/SPD/COST/HONOR/SQUAD_NAME
    animation barbarian_anim = {"Barbarian",6,6,155,128,6,12, data.barbarianImage, 2}; //maxFrames/FrameDelay/FrameWidth/FrameHeight/AnimationColumns/AttackDelay/IMG/ICON
    Squad *barbarian = new Squad(DWARFKIN, barbarian_stat, barbarian_anim);
    handler.AvailableSquads.push_back(barbarian);
    /*============================================================================*/
    stats hero_stat = {"Hero",25, 250, 1, 10, 0,  LEGENDARY};
    animation hero_anim = {"Hero",11, 8, 250, 200, 11, 10, data.heroImage, 4};
    Squad *hero = new Squad(DWARFKIN, hero_stat, hero_anim);
    handler.AvailableSquads.push_back(hero);
    /*============================================================================*/
    stats squire_stat = {"Squire",14, 74, 1.6, 30, 4,  RARE};
    animation squire_anim = {"Squire",7, 7, 200, 100, 7, 10, data.squireImage, 3};
    Squad *squire = new Squad(DWARFKIN, squire_stat, squire_anim);
    handler.AvailableSquads.push_back(squire);
    /*============================================================================*/
    stats goblin_stat = {"Goblin Pillager",5, 12, 3.64, 0, 0,  COMMON};
    animation goblin_anim = {"Goblin Pillager",4,6,125,100,4,15, data.goblinImage,0};
    Squad *goblin_pillager = new Squad(GREENSKINS, goblin_stat, goblin_anim);
    handler.AvailableSquads.push_back(goblin_pillager);
    /*============================================================================*/
    stats troll_stat = {"Troll",20, 92, 1.2, 0, 0, RARE};
    animation troll_anim = {"Troll",8,6,250,200,8,15, data.trollImage,0};
    Squad *war_troll = new Squad(GREENSKINS, troll_stat, troll_anim);
    handler.AvailableSquads.push_back(war_troll);
    /*============================================================================*/
    stats gnoll_stat = {"Gnoll Axeman",17, 21, 1.9, 0, 0, UNCOMMON};
    animation gnoll_anim = {"Gnoll Axeman",8,5,220,175,8,12, data.gnollImage,0};
    Squad *gnoll_axeman = new Squad(GREENSKINS, gnoll_stat, gnoll_anim);
    handler.AvailableSquads.push_back(gnoll_axeman);
    /*============================================================================*/
    stats ogre_stat = {"Ogre",25, 250, 1, 0, 0, LEGENDARY};
    animation ogre_anim = {"Ogre",8, 7, 250, 200, 8, 12, data.ogreImage, 0};
    Squad *ogre = new Squad(GREENSKINS, ogre_stat, ogre_anim);
    handler.AvailableSquads.push_back(ogre);
    /*============================================================================*/
    animation skel_anim = {"Skeleton",7, 5, 150, 100, 7, 10, data.skeletonImage, 0};
    stats skel_stat = {"Skeleton",9, 8, 2.6, 0, 0,  COMMON};
    Squad *skeleton = new Squad(UNDEADS, skel_stat, skel_anim);
    handler.AvailableSquads.push_back(skeleton);
    /*============================================================================*/
    animation ghoul_anim = {"Ghoul",6, 6, 140, 90, 6, 14, data.ghoulImage, 0};
    stats ghoul_stat = {"Ghoul",9, 32, 1.4, 0, 0,  UNCOMMON};
    Squad *ghoul = new Squad(UNDEADS, ghoul_stat, ghoul_anim);
    handler.AvailableSquads.push_back(ghoul);
    /*============================================================================*/
    animation wraith_anim = {"Wraith",6, 7, 200, 130, 6, 12, data.wraithImage, 0};
    stats wraith_stat = {"Wraith",19, 28, 3, 0, 0,  RARE};
    Squad *wraith = new Squad(UNDEADS, wraith_stat, wraith_anim);
    handler.AvailableSquads.push_back(wraith);
    /*============================================================================*/
    animation necro_anim = {"Necromancer",6, 8, 176, 120, 6, 12, data.necroImage, 0};
    stats necro_stats = {"Necromancer",35, 195, 2, 0, 0,  LEGENDARY};
    Squad *necromancer = new Squad(UNDEADS, necro_stats, necro_anim);
    handler.AvailableSquads.push_back(necromancer);
    /*============================================================================*/
    handler.sqiter = handler.AvailableSquads.begin();
    Deployment Deployed(soldier, barbarian, squire);
    GUI gui(&State, &data, &Deployed, &handler, &player);
    Input input(&State, &Deployed, &gui, &handler);
//==============================================
//MUSIC
//==============================================
    song = al_load_sample("Data/music/HeroicDemise.ogg");
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
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;
            else if (ev.keyboard.keycode == ALLEGRO_KEY_M){
                if(sound_enabled == true){
                    sound_enabled = false;
                    al_stop_samples();}
                else {
                    sound_enabled = true;
                    al_play_sample(song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);}}
            else input.GetInput(&ev);
        }
        //==============================================
        //GAME UPDATE
        //==============================================
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;
            //==============================================
            //UPDATE::PLAYING
            //==============================================
            if(State.GetState() == PLAYING )
            {
            /*Camera*/
            if (gui.cameraLeft == true)
                if (Stage::cameraX < 1 ? Stage::cameraX = 0 : Stage::cameraX -= 7);
            if (gui.cameraRight == true)
                if (Stage::cameraX > 799 ? Stage::cameraX = 800 : Stage::cameraX += 7);

            //==============================================
            //UPDATE::PLAYING AND NOT PAUSED
            //==============================================
            if(State.GamePaused() == false)
            {
            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            handler.objects.sort([](GameObject * first, GameObject * second)
            {
                return first->GetY() < second->GetY();
            });
            /*Enemy Spawning*/
            if(rand() % (115 - Deployed.Selected_Stage->StageDiffaulty*10) == 0)
            {
                if (rand() % 10 == 0 && Stage::GetRareNumber() < EnemyEliteCount)
                {
                    if (Deployed.Selected_Stage->StageEnemy == 0)
                    {
                    Creature *troll = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, war_troll);
                    handler.objects.push_back(troll);
                    } else
                    {
                    Creature *undead_wraith = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, wraith);
                    handler.objects.push_back(undead_wraith);
                    }
                }
                else if (rand() % 3 == 0 && Stage::GetUncommonNumber() < EnemyHeavyCount)
                {
                    if (Deployed.Selected_Stage->StageEnemy == 0)
                    {
                    Creature *gnoll = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, gnoll_axeman);
                    handler.objects.push_back(gnoll);
                    } else
                    {
                    Creature *undead_ghoul = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, ghoul);
                    handler.objects.push_back(undead_ghoul);
                    }
                }
                else
                {
                    if (Deployed.Selected_Stage->StageEnemy == 0)
                    {
                    Creature *goblin = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, goblin_pillager);
                    handler.objects.push_back(goblin);
                    } else
                    {
                    Creature *undead_skeleton = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, skeleton);
                    handler.objects.push_back(undead_skeleton);
            }}}
            /*Gold Generation*/
                Stage::AwardGold(0.04);

            /*Collision Checking*/
                for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); ++handler.iter)
                {
                    if ( !(*handler.iter)->GetAlive() || !(*handler.iter)->GetSolid())continue;
                    for(handler.iter2 = handler.objects.begin(); handler.iter2 != handler.objects.end(); ++handler.iter2)
                    {
                        if( !(*handler.iter2)->GetAlive() || (*handler.iter2) == (*handler.iter) || !(*handler.iter2)->GetSolid())  continue;
                        if((*handler.iter)->CollisionCheck((*handler.iter2)))
                        {
                            ((*handler.iter)->SetMove(false));
                            if((*handler.iter)->IsInRange((*handler.iter2), 60) && (*handler.iter)->IsOppositeTeam((*handler.iter2)))
                            {
                                int damage = (*handler.iter)->CheckAttack();
                                if (damage > 0 )
                                {
                                    (*handler.iter2)->GotHit(damage);
                                    Ftext *text = new Ftext(((*handler.iter2)->GetX()-Stage::cameraX), (*handler.iter2)->PositionY(), 0.7, damage, 45, data.font18, data.icons, Damage);
                                    handler.objects.push_back(text);
                                }
                            }
                        }
                    }
                };
            /*Updating*/
                for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); ++handler.iter)
                {
                    if ( !(*handler.iter)->GetAlive()) continue;
                    {
                        (*handler.iter)->Update();
                    }
                }

            /*cull the dead*/
            handler.Remove_objects(deadObj);

            /*jesli stracimy hp/zginie nam hero to konczymy gre*/
            if (Deployed.ptr_to_hero != nullptr)
            {
                if (Stage::GetStageLives() < 1 || (Stage::STAGE_VICTORY_CONDITION != SIEGE && (*Deployed.ptr_to_hero).GetHp() < 1))
                {
                    Stage::SetAftermatchStatus(DEFEAT);
                    State.ChangeState(AFTERMATCH);
                    handler.Remove_objects(miscObj);//Usuwamy m.in floating text
                    al_stop_samples();
                }
                if (Deployed.ptr_to_enemy != nullptr)
                {
                    if (Stage::STAGE_VICTORY_CONDITION == HERO_HUNTING)
                    {
                        if ((*Deployed.ptr_to_enemy).GetHp() < 1)
                        {
                            Stage::ObjectivesCount++;
                        }
                    }
                }
            }
            if (Stage::STAGE_VICTORY_CONDITION == SIEGE)
            {
                if (Stage::StageTimeElapsed > 14400)
                Stage::ObjectivesCount = 4;
                else if (Stage::StageTimeElapsed > 10800)
                Stage::ObjectivesCount = 3;
                else if (Stage::StageTimeElapsed > 7200)
                Stage::ObjectivesCount = 2;
                else if (Stage::StageTimeElapsed > 3600)
                Stage::ObjectivesCount = 1;
            }
            if (Stage::CheckVictoryCondition(Stage::GetObjectivesCount()) == true)
            {
                Stage::SetAftermatchStatus(VICTORY);
                Deployed.OccupiedSlot_1->AwardSquadXP(Deployed.Selected_Stage->StageExpReward);
                Deployed.OccupiedSlot_2->AwardSquadXP(Deployed.Selected_Stage->StageExpReward);
                Deployed.OccupiedSlot_3->AwardSquadXP(Deployed.Selected_Stage->StageExpReward);
                State.ChangeState(AFTERMATCH);
                handler.Remove_objects(miscObj);
                al_stop_samples();
            }
            }
            }
            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;
                gui.DrawGUI();//Drawing all the GUI elements on screen
                if (State.GetState() == PLAYING || State.GetState() == AFTERMATCH )
                {
                    //BEGIN PROJECT RENDER===============
                    for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); ++handler.iter)
                    {
                        (*handler.iter)->Render();
                    }
                }
                //FLIP BUFFERS========================
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
    }
    }
    //==============================================
    //DESTROY PROJECT objects
    //==============================================
    for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); )
    {
        (*handler.iter)->Destroy();
        delete (*handler.iter);
        handler.iter = handler.objects.erase(handler.iter);
    }

    for(handler.sqiter = handler.AvailableSquads.begin(); handler.sqiter != handler.AvailableSquads.end(); )
    {
        delete (*handler.sqiter);
        handler.sqiter = handler.AvailableSquads.erase(handler.sqiter);
    }
    //SHELL objects=================================
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_sample(song);
    return 0;
}

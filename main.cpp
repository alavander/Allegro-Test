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

/*
- Deployment: When selecting a unit for slot, check if its present in other slots -
                if yes, automatically de-select it from two other slots if its the same.
- Stage: Stage should have: background, objectives, enemy type, diffaulty, description, picture (75% done)
- Stage: A stage should also have enemy types and enemy hero type
- Stage: A stage should have stage traits
- Better menu screen

- Stages should have stage level (on which we will base enemy stats)
- Hero should be able to level up and spend skill points on skills, in separate window
- Add talents(passive specials), which will be available directly from squad - a list, or array of talents, which will modify the way the game plays
- Add skills(active specials), which can be used by creatures, and have a cooldown assiocated with them
- Add Exp. reward to stages
- Show the stage location on map (GUI)
- Options: music volume/turn music on/off
- Change lanes name (top,middle,bottom)
- ESC should ask if you wish to quit y/n, as well as pressing 'x'.
- Hp bar if unit is not at 100% hp
- Hero and enemy hero HP bars at top
- Mouse control

Code-tidying:
- Wrapping loading assess into functions/classes (loading at beginning, deleting at end)
- Wrapping unit stats into class/struct/file (separate array for stats, and separate array for animations)
- Make separate Input class, for dealing with input.

Classes:
- Implement Deployment Class - a new deployment UI with options to list all squads, see their stats,
    select units and hero for the next mission. (75% DONE)
        Combat Class -> Melee Combat, Ranged Combat, Unit Special Skills Usage, Hp/Dam, Resists, Cooldowns
        GUI Class, -> Button, Selection Window, Information Window (singleton)
            - Message Class, -> Popup windows containing various informations
            - Ftext Class, -> Move to be part of GUI class, instead of GameObject Class
            - Button Class, -> For clicking
        Input Class, -> Forwarding Pressed Keys/Buttons/Selection Windows to GameState Class (singleton)
        CombatStatus Class,-> Combat Buff, Debuff (stats+/- to targets, damage/healing to targets, stun status)
        *Handler Class, -> Keeping all the game lists together
        *Player Class,-> Number of victories, Number of defeats, killed enemies, achivements/unlocks, save games (50% done)
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
int row_selected = 1;
int unit_selected = 1;
bool cameraLeft = false;
bool cameraRight = false;
int StageTimeElapsed = 0;
bool sound_enabled = true;

/* Stage variables */
int Stage::STAGE_VICTORY_CONDITION = HERO_HUNTING;
int Stage::AftermatchStatus = UNRESOLVED;
int Stage::lives = 5;
int Stage::rareNumber = 0;
int Stage::uncommonNumber = 0;
int Stage::ObjectivesCount = 0;
int Stage::cameraX = 0;
float Stage::gold = 100;
int Stage::honor = 0;
Creature *ptr_to_hero;
Creature *ptr_to_enemy;

/* Game Loop Functions */
void Remove_objects(int remove_flag, Handler &handler); //Usuwa obiekty w zaleznosci od parametru
void Stage_init(); //Resetuje wartosci stage'a
void PlaySelectedStage(Deployment * currently);

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
    GameState State;
    Data data;
    Handler handler;
    Player player;
    //GUI gui(State, data);
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
    stats goblin_stat = {"",5, 12, 3.64, 0, 0,  COMMON};
    animation goblin_anim = {"",4,6,125,100,4,15, data.goblinImage,0};
    Squad *goblin_pillager = new Squad(GREENSKINS, goblin_stat, goblin_anim);
    handler.AvailableSquads.push_back(goblin_pillager);
    /*============================================================================*/
    stats troll_stat = {"",20, 92, 1.2, 0, 0, RARE};
    animation troll_anim = {"",8,6,250,200,8,15, data.trollImage,0};
    Squad *war_troll = new Squad(GREENSKINS, troll_stat, troll_anim);
    handler.AvailableSquads.push_back(war_troll);
    /*============================================================================*/
    stats gnoll_stat = {"",17, 21, 1.9, 0, 0, UNCOMMON};
    animation gnoll_anim = {"",8,5,220,175,8,12, data.gnollImage,0};
    Squad *gnoll_axeman = new Squad(GREENSKINS, gnoll_stat, gnoll_anim);
    handler.AvailableSquads.push_back(gnoll_axeman);
    /*============================================================================*/
    stats ogre_stat = {"",25, 250, 1, 0, 0, LEGENDARY};
    animation ogre_anim = {"",8, 7, 250, 200, 8, 12, data.ogreImage, 0};
    Squad *ogre = new Squad(GREENSKINS, ogre_stat, ogre_anim);
    handler.AvailableSquads.push_back(ogre);
    /*============================================================================*/
    animation skel_anim = {"",7, 5, 150, 100, 7, 10, data.skeletonImage, 0};
    stats skel_stat = {"",9, 8, 2.6, 0, 0,  COMMON};
    Squad *skeleton = new Squad(UNDEADS, skel_stat, skel_anim);
    handler.AvailableSquads.push_back(skeleton);
    /*============================================================================*/
    animation ghoul_anim = {"",6, 6, 140, 90, 6, 14, data.ghoulImage, 0};
    stats ghoul_stat = {"",9, 32, 1.4, 0, 0,  UNCOMMON};
    Squad *ghoul = new Squad(UNDEADS, ghoul_stat, ghoul_anim);
    handler.AvailableSquads.push_back(ghoul);
    /*============================================================================*/
    animation wraith_anim = {"",6, 7, 200, 130, 6, 12, data.wraithImage, 0};
    stats wraith_stat = {"",19, 28, 3, 0, 0,  RARE};
    Squad *wraith = new Squad(UNDEADS, wraith_stat, wraith_anim);
    handler.AvailableSquads.push_back(wraith);
    /*============================================================================*/
    animation necro_anim = {"",6, 8, 176, 120, 6, 12, data.necroImage, 0};
    stats necro_stats = {"",35, 195, 2, 0, 0,  LEGENDARY};
    Squad *necromancer = new Squad(UNDEADS, necro_stats, necro_anim);
    handler.AvailableSquads.push_back(necromancer);
    /*============================================================================*/
    handler.sqiter = handler.AvailableSquads.begin();
    Deployment *Deployed = new Deployment(soldier, barbarian, squire);
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
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_P:
                if (State.GetState() == PLAYING){
                if (State.GamePaused() == true) State.SetPause(false);
                else State.SetPause(true);}
                break;
            case ALLEGRO_KEY_M:
                if(sound_enabled == true) sound_enabled = false;
                else sound_enabled = true;
                break;
            case ALLEGRO_KEY_UP:
                if (State.GetState() == PLAYING)
                    if (row_selected > 1) row_selected -= 1;
                if (State.GetState() == DEPLOYMENT)
                {
                    if (Deployed->Selected_Stage == Deployed->Pregen_Stage_1) Deployed->Selected_Stage = Deployed->Pregen_Stage_2;
                    else if (Deployed->Selected_Stage == Deployed->Pregen_Stage_2) Deployed->Selected_Stage = Deployed->Pregen_Stage_3;
                }
                break;
            case ALLEGRO_KEY_DOWN:
                if (State.GetState() == PLAYING)
                    if (row_selected < 3 ) row_selected += 1;
                if (State.GetState() == DEPLOYMENT)
                {
                    if (Deployed->Selected_Stage == Deployed->Pregen_Stage_3) Deployed->Selected_Stage = Deployed->Pregen_Stage_2;
                    else if (Deployed->Selected_Stage == Deployed->Pregen_Stage_2) Deployed->Selected_Stage = Deployed->Pregen_Stage_1;
                }
                break;
            case ALLEGRO_KEY_LEFT:
                if (State.GetState() == PLAYING || State.GetState() == AFTERMATCH) cameraLeft = true;
                if (State.GetState() == DEPLOYMENT)//wybor squada podczas deploymentu
                {
                    if (handler.sqiter == handler.AvailableSquads.begin()) handler.sqiter = std::prev(std::prev(handler.AvailableSquads.end()));
                    else handler.sqiter--;
                    while ((*handler.sqiter)->GetFraction() != DWARFKIN || (*handler.sqiter)->GetUnitType() == LEGENDARY)
                    {
                        if(handler.sqiter == handler.AvailableSquads.begin()) handler.sqiter = std::prev(std::prev(handler.AvailableSquads.end()));
                        else handler.sqiter--;
                    }
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if (State.GetState() == PLAYING || State.GetState() == AFTERMATCH)cameraRight = true;
                if (State.GetState() == DEPLOYMENT)//Wybor squada podczas deploymentu
                {
                    if (handler.sqiter == std::prev(std::prev(handler.AvailableSquads.end()))) handler.sqiter = handler.AvailableSquads.begin();
                    else handler.sqiter++;
                    while ((*handler.sqiter)->GetFraction() != DWARFKIN || (*handler.sqiter)->GetUnitType() == LEGENDARY)
                    {
                        if(handler.sqiter == std::prev(std::prev(handler.AvailableSquads.end()))) handler.sqiter = handler.AvailableSquads.begin();
                        else handler.sqiter++;
                    }
                }
                break;
            case ALLEGRO_KEY_1:
                unit_selected = 1;
                if (State.GetState() == DEPLOYMENT)
                {
                    //if((*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_2->GetSquadName() ||
                    //   (*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_3->GetSquadName())
                    //{
                    Deployed->OccupiedSlot_1->SetDeploying(0);
                    Deployed->SelectSquadForDeployment((*handler.sqiter), 1);
                    (*handler.sqiter)->SetDeploying(1);
                    //}
                }
                break;
            case ALLEGRO_KEY_2:
                unit_selected = 2;
                if (State.GetState() == DEPLOYMENT)
                {
                   // if((*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_1->GetSquadName() ||
                    //   (*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_3->GetSquadName())
                    //{
                    Deployed->OccupiedSlot_2->SetDeploying(0);
                    Deployed->SelectSquadForDeployment((*handler.sqiter), 2);
                    (*handler.sqiter)->SetDeploying(2);
                   // }
                }
                break;
            case ALLEGRO_KEY_3:
                unit_selected = 3;
                if (State.GetState() == DEPLOYMENT)
                {
                   // if((*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_1->GetSquadName() ||
                   //    (*handler.sqiter)->GetSquadName() != Deployed.OccupiedSlot_2->GetSquadName())
                   // {
                    Deployed->OccupiedSlot_3->SetDeploying(0);
                    Deployed->SelectSquadForDeployment((*handler.sqiter), 3);
                    (*handler.sqiter)->SetDeploying(3);
                   // }
                }
                break;
            case ALLEGRO_KEY_SPACE:
                if (State.GetState() == MENU )
                {
                    State.ChangeState(DEPLOYMENT);
                    break;
                }
                if (State.GetState() == DEPLOYMENT )
                {
                    Stage_init();
                    if (Deployed->Selected_Stage->StageBackground == 0) data.bgImage = al_load_bitmap("Data/BG_forest.jpg"); //Przeniesc do funkcji PlaySelectedStage
                    else if (Deployed->Selected_Stage->StageBackground == 1) data.bgImage = al_load_bitmap("Data/BG_cave.jpg");
                    else data.bgImage = al_load_bitmap("Data/BG_swamp.jpg");
                    PlaySelectedStage(Deployed);
                    State.ChangeState(PLAYING);

                    al_play_sample(song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                        if (Stage::STAGE_VICTORY_CONDITION == BLOODBATH)
                        {
                            Creature *stageHero = new Creature(40, 0, hero);
                            ptr_to_hero = stageHero;
                            handler.objects.push_back(stageHero);
                        }
                        else if (Stage::STAGE_VICTORY_CONDITION == HERO_HUNTING)
                        {
                            Creature *stageHero = new Creature(40, 0, hero);
                            ptr_to_hero = stageHero;
                            handler.objects.push_back(stageHero);
                            if (Deployed->Selected_Stage->StageEnemy == 0)
                            {
                            Creature *enemyHero = new Creature(1560, 0, ogre);
                            ptr_to_enemy = enemyHero;
                            handler.objects.push_back(enemyHero);
                            }
                            else
                            {
                            Creature *enemyHero = new Creature(1560, 0, necromancer);
                            ptr_to_enemy = enemyHero;
                            handler.objects.push_back(enemyHero);
                            }
                        }
                    break;
                }
                if (State.GetState() == AFTERMATCH)
                {
                    Remove_objects(allObj, handler);
                    State.ChangeState(DEPLOYMENT);
                    Deployed->OccupiedSlot_1->SquadUpdate();
                    Deployed->OccupiedSlot_2->SquadUpdate();
                    Deployed->OccupiedSlot_3->SquadUpdate();
                    delete Deployed;
                    Deployment *Deployed = new Deployment(soldier, barbarian, squire);
                    break;
                }
                if (State.GetState() == PLAYING && State.GamePaused() == false)
                {
                    int gold_cost = Deployed->GetSelectedSlot(unit_selected).GetGoldCost();
                    int honor_cost = Deployed->GetSelectedSlot(unit_selected).GetHonorCost();
                    if (Stage::GetStageGold() >= gold_cost && Stage::GetStageHonor() >= honor_cost)
                    {
                        handler.objects.push_back(Deployed->SpawnUnit(row_selected, unit_selected));
                        Stage::SpendGold(gold_cost);
                        Ftext *text = new Ftext(250, 545, -0.5, -(gold_cost), 45, data.font18, data.icons, Gold);
                        handler.objects.push_back(text);
                        if (honor_cost > 0)
                        {
                            Stage::SpendHonor(honor_cost);
                            Ftext *text = new Ftext(250, 570, -0.5, -(honor_cost), 45, data.font18, data.icons, Honor);
                            handler.objects.push_back(text);
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

            /*Camera*/
            if (cameraLeft == true)
                if (Stage::cameraX < 1 ? Stage::cameraX = 0 : Stage::cameraX -= 7);
            if (cameraRight == true)
                if (Stage::cameraX > 799 ? Stage::cameraX = 800 : Stage::cameraX += 7);

            if(State.GetState() == PLAYING && State.GamePaused() == false)
            {
            /*Enemy Spawning*/
            if(rand() % (115 - Deployed->Selected_Stage->StageDiffaulty*10) == 0)
            {
                if (rand() % 10 == 0 && Stage::GetRareNumber() < EnemyEliteCount)
                {
                    if (Deployed->Selected_Stage->StageEnemy == 0)
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
                    if (Deployed->Selected_Stage->StageEnemy == 0)
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
                    if (Deployed->Selected_Stage->StageEnemy == 0)
                    {
                    Creature *goblin = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, goblin_pillager);
                    handler.objects.push_back(goblin);
                    } else
                    {
                    Creature *undead_skeleton = new Creature(WORLD_WIDTH-20, rand() % 3 + 1, skeleton);
                    handler.objects.push_back(undead_skeleton);
                    }
                }
            }

            /*Gold Generation*/
                Stage::AwardGold(0.04);

            /*Sorting by value of Y - Thank you Lambdas and Stackoverflow!*/
            handler.objects.sort([](GameObject * first, GameObject * second)
            {
                return first->GetY() < second->GetY();
            });

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
            //Updating
                for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); ++handler.iter)
                {
                    if ( !(*handler.iter)->GetAlive()) continue;
                    {
                        (*handler.iter)->Update();
                    }
                }

            //cull the dead
            Remove_objects(deadObj, handler);

            //jesli stracimy hp/zginie nam hero to konczymy gre
            if (Stage::GetStageLives() < 1 || Stage::STAGE_VICTORY_CONDITION != SIEGE &&(*ptr_to_hero).GetHp() < 1)
            {
                Stage::SetAftermatchStatus(DEFEAT);
                State.ChangeState(AFTERMATCH);
                Remove_objects(miscObj, handler);//Usuwamy m.in floating text
                al_stop_samples();
            }
            if (Stage::STAGE_VICTORY_CONDITION == HERO_HUNTING)
            {
                if ((*ptr_to_enemy).GetHp() < 1) Stage::ObjectivesCount++;
            }

            if (Stage::STAGE_VICTORY_CONDITION == SIEGE) // Poprawic zeby bralo dobrze
            {
                if (StageTimeElapsed > 14400)
                Stage::ObjectivesCount = 4;
                else if (StageTimeElapsed > 10800)
                Stage::ObjectivesCount = 3;
                else if (StageTimeElapsed > 7200)
                Stage::ObjectivesCount = 2;
                else if (StageTimeElapsed > 3600)
                Stage::ObjectivesCount = 1;
            }
            //jesli spelnimy objectives misji, to wygrywamy gre - observer pattern?
            if (Stage::CheckVictoryCondition(Stage::GetObjectivesCount()) == true)
            {
                Stage::SetAftermatchStatus(VICTORY);
                Deployed->OccupiedSlot_1->AwardSquadXP(Deployed->Selected_Stage->StageExpReward);
                Deployed->OccupiedSlot_2->AwardSquadXP(Deployed->Selected_Stage->StageExpReward);
                Deployed->OccupiedSlot_3->AwardSquadXP(Deployed->Selected_Stage->StageExpReward);
                State.ChangeState(AFTERMATCH);
                Remove_objects(miscObj, handler);
                al_stop_samples();
            }
            }
            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;
                if (State.GetState() == MENU)
                {
                    al_draw_bitmap(data.titleScreen, 0, 0, 0);
                    al_draw_text(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, HEIGHT/1.2, ALLEGRO_ALIGN_CENTRE, "Press spacebar to start");
                }
                if (State.GetState() == DEPLOYMENT)
                {
                    al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgb(0,0,0));//tlo
                    /* STAGE SELECT AND DRAWING */
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 30, 5, 0, "Player name: %s", player.GetPlayerName().c_str() );
                    al_draw_bitmap(data.deployment_map,30,30,0);
                    al_draw_textf(data.font18,al_map_rgb(50,200,250), 220, 360, ALLEGRO_ALIGN_CENTER, "%s", (Deployed->Selected_Stage->StageName).c_str());
                    al_draw_text(data.font12, al_map_rgb(255,255,255), 230, 380, ALLEGRO_ALIGN_CENTER, "(Press up/down arrow to change map)" );
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 20, 420, 0, "Stage mode:" );
                    if(Deployed->Selected_Stage->StageVictory == 0)
                         al_draw_text(data.font18, al_map_rgb(50,180,200), 270, 420, 0, "Hero Hunt" );
                    if(Deployed->Selected_Stage->StageVictory == 1)
                         al_draw_text(data.font18, al_map_rgb(240,0,0), 270, 420, 0, "Bloodbath");
                    if(Deployed->Selected_Stage->StageVictory == 2)
                         al_draw_text(data.font18, al_map_rgb(180,230,30), 270, 420, 0, "Siege" );
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 20, 450, 0, "Stage enemy:" );
                    if(Deployed->Selected_Stage->StageEnemy == 0)
                         al_draw_text(data.font18, al_map_rgb(180,230,30), 270, 450, 0, "Greenskins" );
                    if(Deployed->Selected_Stage->StageEnemy == 1)
                         al_draw_text(data.font18, al_map_rgb(160,160,160), 270, 450, 0, "Undead");
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 20, 480, 0, "Stage map:" );
                    if(Deployed->Selected_Stage->StageBackground == 0)
                         al_draw_text(data.font18, al_map_rgb(180,230,30), 270, 480, 0, "Forest" );
                    if(Deployed->Selected_Stage->StageBackground == 1)
                         al_draw_text(data.font18, al_map_rgb(160,160,160), 270, 480, 0, "Cave");
                    if(Deployed->Selected_Stage->StageBackground == 2)
                         al_draw_text(data.font18, al_map_rgb(35,170,80), 270, 480, 0, "Swamp" );
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 20, 510, 0, "Stage difficulty:" );
                    if(Deployed->Selected_Stage->StageDiffaulty == 0)
                         al_draw_text(data.font18, al_map_rgb(150,200,240), 270, 510, 0, "Easy" );
                    if(Deployed->Selected_Stage->StageDiffaulty == 1)
                         al_draw_text(data.font18, al_map_rgb(250,250,130), 270, 510, 0, "Normal");
                    if(Deployed->Selected_Stage->StageDiffaulty == 2)
                         al_draw_text(data.font18, al_map_rgb(255,0,0), 270, 510, 0, "Hard" );
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 20, 540, 0, "Stage exp:" );
                         al_draw_textf(data.font18, al_map_rgb(255,255,255), 270, 540, 0, "%i", Deployed->Selected_Stage->StageExpReward);
                    /* SQUAD SELECT AND DRAWING */
                    al_draw_text(data.font18, al_map_rgb(255,255,255), (460+750)/2, 5, ALLEGRO_ALIGN_CENTER, "Squad Selection" );
                    al_draw_text(data.font12, al_map_rgb(255,255,255), (460+750)/2, 25, ALLEGRO_ALIGN_CENTER, "(Press left/right to change)" );
                    //al_draw_filled_rectangle(460, 530, 660, 560, al_map_rgb(255,255,255));//start button
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 450, 515, 0, "Options:" );
                    al_draw_text(data.font12, al_map_rgb(255,255,255), 460, 540, 0, "I.   Press spacebar to start" );
                    al_draw_text(data.font12, al_map_rgb(255,255,255), 460, 560, 0, "II.  Press esc to quit to menu" );
                    al_draw_text(data.font12, al_map_rgb(255,255,255), 460, 580, 0, "III. Press s to save game" );
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 50, 0, "Squad Name: %s",(*handler.sqiter)->GetSquadName().c_str());
                    if((*handler.sqiter)->GetLevel() == 10) al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 70, 0, "Level: %i (Max Level)",(*handler.sqiter)->GetLevel());
                        else al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 70, 0, "Level: %i (exp:%i/%i)",(*handler.sqiter)->GetLevel(),(*handler.sqiter)->GetSquadXP(),(*handler.sqiter)->GetLevel()*250);
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 90, 0, "ATK: %i",(*handler.sqiter)->GetDamage());
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 110, 0, "HP: %i",(*handler.sqiter)->GetHp());
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 130, 0, "SPEED: %f",(*handler.sqiter)->GetSpeed());
                    al_draw_textf(data.font18, al_map_rgb(255,255,0), 460, 150, 0, "Gold Cost: %i",(*handler.sqiter)->GetGoldCost());
                    if ((*handler.sqiter)->GetHonorCost() > 0)
                        al_draw_textf(data.font18, al_map_rgb(255,200,0), 460, 170, 0, "Honor Cost: %i",(*handler.sqiter)->GetHonorCost());
                    if ((*handler.sqiter)->GetDeploying() > 0)
                        al_draw_textf(data.font18, al_map_rgb(255,255,255), 460, 190, 0, "Unit is selected for slot: %i",(*handler.sqiter)->GetDeploying());
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 460, 240, 0, "1.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_1->GetIconNumber(), 0,40, 40, 480,240, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 530, 240, 0, "%s", Deployed->OccupiedSlot_1->GetSquadName().c_str());
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 460, 300, 0, "2.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_2->GetIconNumber(), 0,40, 40, 480,300, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 530, 300, 0, "%s", Deployed->OccupiedSlot_2->GetSquadName().c_str());
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 460, 360, 0, "3.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_3->GetIconNumber(), 0,40, 40, 480,360, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 530, 360, 0, "%s", Deployed->OccupiedSlot_3->GetSquadName().c_str());
                    /*Player Stats*/
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 450, 430, 0, "Enemies slain:  %i", player.GetEnemiesKilled());
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 450, 450, 0, "Victories:      %i", player.GetVictories());
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), 450, 470, 0, "Defeats:        %i", player.GetDefeats());

                }
                if (State.GetState() == PLAYING || State.GetState() == AFTERMATCH )
                {
                    al_draw_bitmap_region(data.bgImage, 0+Stage::cameraX, 0, SCREEN_WIDTH, HEIGHT, 0, 0, 0);
                    al_draw_bitmap(data.uiImage, 0, 500, 0);
                    //BEGIN PROJECT RENDER===============
                    for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); ++handler.iter)
                    {
                        (*handler.iter)->Render();
                    }
                    //Icons, position and costs!
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 300, 555, 0, "1.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_1->GetIconNumber(), 0,40, 40, 320,545, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 365, 550, 0, "%i", Deployed->OccupiedSlot_1->GetGoldCost());
                    al_draw_bitmap_region(data.icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_1->GetGoldCost() < 10 ? 375 : 382, 550, 0);
                    if(Deployed->OccupiedSlot_1->GetHonorCost() > 0){
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 365, 570, 0, "%i", Deployed->OccupiedSlot_1->GetHonorCost());
                    al_draw_bitmap_region(data.icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_1->GetHonorCost() < 10 ? 375 : 382, 570, 0);}
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 400, 555, 0, "2.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_2->GetIconNumber(), 0,40, 40, 420,545, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 465, 550, 0, "%i", Deployed->OccupiedSlot_2->GetGoldCost());
                    al_draw_bitmap_region(data.icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_2->GetGoldCost() < 10 ? 475 : 482, 550, 0);
                    if(Deployed->OccupiedSlot_2->GetHonorCost() > 0){
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 465, 570, 0, "%i", Deployed->OccupiedSlot_2->GetHonorCost());
                    al_draw_bitmap_region(data.icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_2->GetHonorCost() < 10 ? 475 : 482, 570, 0);}
                    al_draw_text(data.font18, al_map_rgb(255,255,255), 500, 555, 0, "3.");
                    al_draw_bitmap_region(data.unit_icon, 40*Deployed->OccupiedSlot_3->GetIconNumber(), 0,40, 40, 520,545, 0);
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 565, 550, 0, "%i", Deployed->OccupiedSlot_3->GetGoldCost());
                    al_draw_bitmap_region(data.icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_3->GetGoldCost() < 10 ? 575 : 582, 550, 0);
                    if(Deployed->OccupiedSlot_3->GetHonorCost() > 0){
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 565, 570, 0, "%i", Deployed->OccupiedSlot_3->GetHonorCost());
                    al_draw_bitmap_region(data.icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_3->GetHonorCost() < 10 ? 575 : 582, 570, 0);}
                    /* Basic Selected unit stats*/
                    al_draw_textf(data.font12,al_map_rgb(255,255,255),650, 540, 0,"%s",(Deployed->GetSelectedSlot(unit_selected).GetSquadName()).c_str());
                    al_draw_textf(data.font12,al_map_rgb(255,255,255),640, 555, 0,"Level: %i",Deployed->GetSelectedSlot(unit_selected).GetLevel());
                    al_draw_textf(data.font12,al_map_rgb(255,255,255),640, 570, 0,"Attack: %i",Deployed->GetSelectedSlot(unit_selected).GetDamage());
                    al_draw_textf(data.font12,al_map_rgb(255,255,255),640, 585, 0,"Health: %i",Deployed->GetSelectedSlot(unit_selected).GetHp());
                    /* Unit Selection Box */
                    al_draw_rectangle(220+unit_selected*100,545,260+unit_selected*100,585, al_map_rgb(255, 240, 0), 1);
                    /* Row Selected Cursor */
                    al_draw_textf(data.font12, al_map_rgb(255,255,255), 5, 410+row_selected*20, 0,"%i. lane>", row_selected);
                    /* UI text + small debug*/
                    if (State.GamePaused() == true)
                    {
                        al_draw_text(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 30, ALLEGRO_ALIGN_CENTRE, "Game Paused");
                        //al_draw_textf(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTRE, "Enemy Rare count: %i", Stage::GetRareNumber());
                        //al_draw_textf(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 70, ALLEGRO_ALIGN_CENTRE, "Enemy Uncommon count: %i", Stage::GetUncommonNumber());
                        //al_draw_textf(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 90, ALLEGRO_ALIGN_CENTRE, "Stage Time Elapsed: %i", StageTimeElapsed);
                        //al_draw_textf(data.font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 110, ALLEGRO_ALIGN_CENTRE, "Objectives Count: %i", Stage::GetObjectivesCount());
                    }
                    al_draw_textf(data.font18, al_map_rgb(240, 180, 0), SCREEN_WIDTH/5, 545, ALLEGRO_ALIGN_CENTRE, "Gold: %i", (int)Stage::GetStageGold());
                    al_draw_bitmap_region(data.icons, Gold, 0, 10, 10, SCREEN_WIDTH/5+60, 545, 0);
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), SCREEN_WIDTH/5, 570, ALLEGRO_ALIGN_CENTRE, "Honor: %i", Stage::GetStageHonor());
                    al_draw_bitmap_region(data.icons, Honor, 0, 10, 10, SCREEN_WIDTH/5+60, 570, 0);

                    if (Stage::STAGE_VICTORY_CONDITION == SIEGE)
                    {
                    for (int i = 0; i < Stage::GetStageLives(); i++)
                    {
                        al_draw_textf(data.font18, al_map_rgb(240,30,0), 625, 10, ALLEGRO_ALIGN_CENTER, "Survive for %i more minutes.", 4-(Stage::GetObjectivesCount()));
                        al_draw_bitmap(data.heartIcon,5+(i*30), 0, 0);
                    }
                    }

                    if (Stage::STAGE_VICTORY_CONDITION == BLOODBATH)
                    {
                        al_draw_textf(data.font18, al_map_rgb(240,30,0), SCREEN_WIDTH/1.2, 10, ALLEGRO_ALIGN_CENTER, "Kills to win: %i", 50-(Stage::GetObjectivesCount()));
                        al_draw_textf(data.font18, al_map_rgb(255,255,255), 5, 10, 0, "Hero hp: %i", (*ptr_to_hero).GetHp());
                    }

                    if (Stage::STAGE_VICTORY_CONDITION == HERO_HUNTING)
                    {
                        al_draw_textf(data.font18, al_map_rgb(255,255,255), 560, 10, 0, "Enemy Hero Hp: %i", (*ptr_to_enemy).GetHp());
                        al_draw_textf(data.font18, al_map_rgb(255,255,255), 5, 10, 0, "Hero Hp: %i", (*ptr_to_hero).GetHp());
                    }

                    if (State.GetState() == PLAYING && State.GamePaused() == false ) StageTimeElapsed++;
                    al_draw_textf(data.font18, al_map_rgb(255,255,255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTER, "%i:%i",(StageTimeElapsed/60/60),(StageTimeElapsed/60)%60);
                }
                if (State.GetState() == AFTERMATCH)
                {
                    if(Stage::GetAftermachStatus() == DEFEAT)
                    {
                    al_draw_text(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2-30, ALLEGRO_ALIGN_CENTRE, "You have been defeated!");
                    al_draw_text(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to continue!");
                    }
                    if(Stage::GetAftermachStatus() == VICTORY)
                    {
                    al_draw_text(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2-30, ALLEGRO_ALIGN_CENTRE, "You have proved victorious!");
                    if(Deployed->OccupiedSlot_1->CheckLevel()== true)
                        al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_1->GetSquadName().c_str());
                    else al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_1->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
                    if(Deployed->OccupiedSlot_2->CheckLevel()== true)
                        al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_2->GetSquadName().c_str());
                    else al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_2->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
                    if(Deployed->OccupiedSlot_3->CheckLevel()== true)
                        al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+60, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_3->GetSquadName().c_str());
                    else al_draw_textf(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+60, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_3->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
                    al_draw_text(data.font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+90, ALLEGRO_ALIGN_CENTRE, "Press spacebar to continue!");
                    }
                }
                //FLIP BUFFERS========================
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
        }
    }
    //==============================================
    //DESTROY PROJECT handler.objects
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
    delete Deployed;
    return 0;
}

void Remove_objects(int remove_flag, Handler &handler)
{
    switch(remove_flag)
    {
    case deadObj:
    {
    for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); )
    {
        if(! (*handler.iter)->GetAlive())
        {
            delete (*handler.iter);
            handler.iter = handler.objects.erase(handler.iter);
        }
        else
            handler.iter++;
    }
    break;
    }
    case allObj:
    {
        for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); )
    {
        delete (*handler.iter);
        handler.iter = handler.objects.erase(handler.iter);
    }
    break;
    }
     case miscObj:
    {
    for(handler.iter = handler.objects.begin(); handler.iter != handler.objects.end(); )
    {
        if((*handler.iter)->GetID() == MISC)
        {
            delete (*handler.iter);
            handler.iter = handler.objects.erase(handler.iter);
        }
        else
            handler.iter++;
    }
    break;
    }
    }
};

void Stage_init() // Przeniesc jako metode do stage.h
{
    StageTimeElapsed = 0;
}

void PlaySelectedStage(Deployment * currently)
{
    /*Victory condition*/
    if (currently->Selected_Stage->StageVictory == 0) Stage::STAGE_VICTORY_CONDITION = HERO_HUNTING;
    else if (currently->Selected_Stage->StageVictory == 1) Stage::STAGE_VICTORY_CONDITION = BLOODBATH;
    else Stage::STAGE_VICTORY_CONDITION = SIEGE;
    Stage::gold = 100 - currently->Selected_Stage->StageDiffaulty*10; //-10 gold for normal, -20 gold for hard
    Stage::lives = 5 - currently->Selected_Stage->StageDiffaulty;//-1 lives for normal, -2 lives for hard
    Stage::rareNumber = 0;
    Stage::uncommonNumber = 0;
    Stage::ObjectivesCount = 0;
    Stage::cameraX = 0;
    Stage::honor = 0;
    Stage::SetAftermatchStatus(UNRESOLVED);
};

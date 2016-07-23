#pragma once

const int SCREEN_WIDTH = 800;
const int WORLD_WIDTH = 1600;
const int HEIGHT = 600;
const float LANE[] = {420,450,470,490};

/* Enemies Count - max number of spawned uncommon and rare creatures*/
const int EnemyHeavyCount = 5;
const int EnemyEliteCount = 2;

enum ID{PLAYER, ENEMY, MISC};
enum GSTATE{PLAYING, MENU, AFTERMATCH, DEPLOYMENT};
enum STAGE_VICTORY_CONDITION{HERO_HUNTING, BLOODBATH, SIEGE};
enum FRACTION{DWARFKIN, GREENSKINS, UNDEADS};
enum ANIMATION{WALKING, ATTACKING, DYING};
enum UNIT_TYPE{COMMON, UNCOMMON, RARE, LEGENDARY};
enum AftermatchStatus{UNRESOLVED, VICTORY, DEFEAT};
enum icon_type{NoIcon = 0, Damage = 10, Gold = 20, Honor = 30};//Pozycja/nazwa ikony
enum remove_flag{deadObj, allObj, miscObj};//Flagi uzywane przy kasowaniu obiektow z listy GameObjects


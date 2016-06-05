#pragma once
const int WIDTH = 800;
const int HEIGHT = 400;

enum ID{PLAYER, ENEMY, MISC}; //IDs
enum STATE{PLAYING, PAUSED, MENU, DEFEAT, CUTSCENE}; // States
enum UNIT_TYPE{GREENSKIN, DWARF, BERSERKER, LORD}; // Unit Types
enum STAGE_VICTORY_CONDITION{HERO_HUNTING, BLOODBATH, SIEGE}; //victory conditions

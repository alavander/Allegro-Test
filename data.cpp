#include "data.h"

Data::Data()
{
    /*Font*/
    font18 = al_load_font("Data/dwarf.ttf", 18, 0);
    font12 = al_load_ttf_font("Data/dwarf.ttf", 12, 0);
    /*GUI*/
    bgImage = al_load_bitmap("Data/BG_forest.jpg");
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
    heroImage = al_load_bitmap("Data/units/hero_idle.png");
    goblinImage = al_load_bitmap("Data/units/goblin_pillager.png");
    trollImage = al_load_bitmap("Data/units/troll.png");
    gnollImage = al_load_bitmap("Data/units/gnoll_axeman.png");
    ogreImage = al_load_bitmap("Data/units/ogre_idle.png");
    skeletonImage = al_load_bitmap("Data/units/skeleton.png");
    ghoulImage = al_load_bitmap("Data/units/ghoul.png");
    wraithImage = al_load_bitmap("Data/units/wraith.png");
    necroImage = al_load_bitmap("Data/units/necromancer.png");
}

Data::~Data()
{
    al_destroy_font(font18);
    al_destroy_font(font12);
    al_destroy_bitmap(soldierImage);
    al_destroy_bitmap(barbarianImage);
    al_destroy_bitmap(goblinImage);
    al_destroy_bitmap(trollImage);
    al_destroy_bitmap(ogreImage);
    al_destroy_bitmap(squireImage);
    al_destroy_bitmap(heroImage);
    al_destroy_bitmap(gnollImage);
    al_destroy_bitmap(skeletonImage);
    al_destroy_bitmap(ghoulImage);
    al_destroy_bitmap(wraithImage);
    al_destroy_bitmap(necroImage);
    al_destroy_bitmap(bgImage);
    al_destroy_bitmap(uiImage);
    al_destroy_bitmap(titleScreen);
    al_destroy_bitmap(heartIcon);
    al_destroy_bitmap(icons);
    al_destroy_bitmap(unit_icon);
    al_destroy_bitmap(deployment_map);
}

void Data::LoadFromFiles() // Factory Pattern Challange
{
    std::fstream text_file("Data/unit_data/unit_stats.txt");
    string line;
    while(getline(text_file, line))
    {
        if(line[0] == '*') continue;

        int f_dam, f_hp, f_gold, f_honor, f_type;
        float f_speed;
        std::string f_name;
        std::stringstream(line) >> f_name >> f_dam >> f_hp >> f_speed >> f_gold >> f_honor >> f_type;
        //stats_.push_back(stats (ReturnName())={f_name, f_dam, f_hp, f_speed, f_gold, f_honor, f_type});
    }



    //std::fstream myfile("footman.txt", std::ios_base::in);
    //int f_dam, f_hp, f_gold, f_honor, f_type;
    //float f_speed;
    //std::string f_name;
    //myfile >> f_dam >> f_hp >> f_speed >> f_gold >> f_honor >> f_name >> f_type;

}

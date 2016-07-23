#include "data.h"

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

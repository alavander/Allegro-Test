#include "player.h"


Player::Player()
{
    StartNewGame();
}

void Player::StartNewGame()
{
    number_of_defeats = 0;
    number_of_victories = 0;
    killed_enemies = 0;
    name = "null";
};

void Player::LoadGame()
{

};

void Player::SaveGame()
{
    std::string path = "\\Data\\saves\\";
    std::string filename = GetPlayerName()+".txt";
    std::fstream file(path);
    file.open(filename, std::ios::out | std::ios::trunc);
      if (file.is_open())
      {
        file << name << "\n";
        file << number_of_victories << "\n";
        file << number_of_defeats << "\n";
        file << killed_enemies << "\n";
        file.close();
      }
};

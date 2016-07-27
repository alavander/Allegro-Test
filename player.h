#pragma once
#include "handler.h"
#include <fstream>
#include <string>

class Player
{
    private:
        int number_of_victories;//
        int number_of_defeats;
        int killed_enemies;
        string name;
    public:
        Player();
        void StartNewGame();
        void LoadGame();
        void SaveGame();
        void NewName(string input) {Player::name = input;}
        string GetPlayerName() {return name;}
        int GetVictories() {return number_of_victories;}
        int GetDefeats() {return number_of_defeats;}
        int GetEnemiesKilled() {return killed_enemies;}
        void PlayerVictory() {Player::number_of_victories++;}
        void PlayerDefeat() {Player::number_of_defeats++;}
        void PlayerEnemyKilled() {Player::killed_enemies++;}
};


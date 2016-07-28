#include "gui.h"

GUI::GUI(GameState *state, Data *data, Deployment *Deployed, Handler *handler, Player *player)
{
    GUI::state = state;
    GUI::data = data;
    GUI::Deployed = Deployed;
    GUI::handler = handler;
    GUI::player = player;
    row_selected = 1;
    unit_selected = 1;
}

void GUI::DrawGUI()
{
    int current_state = state->GetState();
    switch(current_state)
    {
        case MENU:
        DrawMenuScreen();
        break;
        case DEPLOYMENT:
        DrawDeploymentScreen();
        break;
        case PLAYING:
        case AFTERMATCH:
        DrawBattleMap();
        DrawBattleGUI();
        break;
    }
}

void GUI::DrawMenuScreen()
{
    al_draw_bitmap(data->titleScreen, 0, 0, 0);
    al_draw_text(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, HEIGHT/1.2, ALLEGRO_ALIGN_CENTRE, "Press spacebar to start");
};

void GUI::DrawDeploymentScreen()
{
al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgb(0,0,0));//tlo
/* STAGE SELECT AND DRAWING */
al_draw_textf(data->font18, al_map_rgb(255,255,255), 30, 5, 0, "Player name: %s", player->GetPlayerName().c_str() );
al_draw_bitmap(data->deployment_map,30,30,0);
al_draw_textf(data->font18,al_map_rgb(50,200,250), 220, 360, ALLEGRO_ALIGN_CENTER, "%s", (Deployed->Selected_Stage->StageName).c_str());
al_draw_text(data->font12, al_map_rgb(255,255,255), 230, 380, ALLEGRO_ALIGN_CENTER, "(Press up/down arrow to change map)" );
al_draw_text(data->font18, al_map_rgb(255,255,255), 20, 420, 0, "Stage mode:" );
if(Deployed->Selected_Stage->StageVictory == 0)
al_draw_text(data->font18, al_map_rgb(50,180,200), 270, 420, 0, "Hero Hunt" );
if(Deployed->Selected_Stage->StageVictory == 1)
al_draw_text(data->font18, al_map_rgb(240,0,0), 270, 420, 0, "Bloodbath");
if(Deployed->Selected_Stage->StageVictory == 2)
al_draw_text(data->font18, al_map_rgb(180,230,30), 270, 420, 0, "Siege" );
al_draw_text(data->font18, al_map_rgb(255,255,255), 20, 450, 0, "Stage enemy:" );
if(Deployed->Selected_Stage->StageEnemy == 0)
al_draw_text(data->font18, al_map_rgb(180,230,30), 270, 450, 0, "Greenskins" );
if(Deployed->Selected_Stage->StageEnemy == 1)
al_draw_text(data->font18, al_map_rgb(160,160,160), 270, 450, 0, "Undead");
al_draw_text(data->font18, al_map_rgb(255,255,255), 20, 480, 0, "Stage map:" );
if(Deployed->Selected_Stage->StageBackground == 0)
al_draw_text(data->font18, al_map_rgb(180,230,30), 270, 480, 0, "Forest" );
if(Deployed->Selected_Stage->StageBackground == 1)
al_draw_text(data->font18, al_map_rgb(160,160,160), 270, 480, 0, "Cave");
if(Deployed->Selected_Stage->StageBackground == 2)
al_draw_text(data->font18, al_map_rgb(35,170,80), 270, 480, 0, "Swamp" );
al_draw_text(data->font18, al_map_rgb(255,255,255), 20, 510, 0, "Stage difficulty:" );
if(Deployed->Selected_Stage->StageDiffaulty == 0)
al_draw_text(data->font18, al_map_rgb(150,200,240), 270, 510, 0, "Easy" );
if(Deployed->Selected_Stage->StageDiffaulty == 1)
al_draw_text(data->font18, al_map_rgb(250,250,130), 270, 510, 0, "Normal");
if(Deployed->Selected_Stage->StageDiffaulty == 2)
al_draw_text(data->font18, al_map_rgb(255,0,0), 270, 510, 0, "Hard" );
al_draw_text(data->font18, al_map_rgb(255,255,255), 20, 540, 0, "Stage exp:" );
al_draw_textf(data->font18, al_map_rgb(255,255,255), 270, 540, 0, "%i", Deployed->Selected_Stage->StageExpReward);
/* SQUAD SELECT AND DRAWING */
al_draw_text(data->font18, al_map_rgb(255,255,255), (460+750)/2, 5, ALLEGRO_ALIGN_CENTER, "Squad Selection" );
al_draw_text(data->font12, al_map_rgb(255,255,255), (460+750)/2, 25, ALLEGRO_ALIGN_CENTER, "(Press left/right to change)" );
//al_draw_filled_rectangle(460, 530, 660, 560, al_map_rgb(255,255,255));//start button
al_draw_text(data->font18, al_map_rgb(255,255,255), 450, 515, 0, "Options:" );
al_draw_text(data->font12, al_map_rgb(255,255,255), 460, 540, 0, "I.   Press spacebar to start" );
al_draw_text(data->font12, al_map_rgb(255,255,255), 460, 560, 0, "II.  Press esc to quit to menu" );
al_draw_text(data->font12, al_map_rgb(255,255,255), 460, 580, 0, "III. Press s to save game" );
al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 50, 0, "Squad Name: %s",(*handler->sqiter)->GetSquadName().c_str());
if((*handler->sqiter)->GetLevel() == 10) al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 70, 0, "Level: %i (Max Level)",(*handler->sqiter)->GetLevel());
else al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 70, 0, "Level: %i (exp:%i/%i)",(*handler->sqiter)->GetLevel(),(*handler->sqiter)->GetSquadXP(),(*handler->sqiter)->GetLevel()*250);
al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 90, 0, "ATK: %i",(*handler->sqiter)->GetDamage());
al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 110, 0, "HP: %i",(*handler->sqiter)->GetHp());
al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 130, 0, "SPEED: %f",(*handler->sqiter)->GetSpeed());
al_draw_textf(data->font18, al_map_rgb(255,255,0), 460, 150, 0, "Gold Cost: %i",(*handler->sqiter)->GetGoldCost());
if ((*handler->sqiter)->GetHonorCost() > 0)
al_draw_textf(data->font18, al_map_rgb(255,200,0), 460, 170, 0, "Honor Cost: %i",(*handler->sqiter)->GetHonorCost());
if ((*handler->sqiter)->GetDeploying() > 0)
al_draw_textf(data->font18, al_map_rgb(255,255,255), 460, 190, 0, "Unit is selected for slot: %i",(*handler->sqiter)->GetDeploying());
al_draw_text(data->font18, al_map_rgb(255,255,255), 460, 240, 0, "1.");
al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_1->GetIconNumber(), 0,40, 40, 480,240, 0);
al_draw_textf(data->font12, al_map_rgb(255,255,255), 530, 240, 0, "%s", Deployed->OccupiedSlot_1->GetSquadName().c_str());
al_draw_text(data->font18, al_map_rgb(255,255,255), 460, 300, 0, "2.");
al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_2->GetIconNumber(), 0,40, 40, 480,300, 0);
al_draw_textf(data->font12, al_map_rgb(255,255,255), 530, 300, 0, "%s", Deployed->OccupiedSlot_2->GetSquadName().c_str());
al_draw_text(data->font18, al_map_rgb(255,255,255), 460, 360, 0, "3.");
al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_3->GetIconNumber(), 0,40, 40, 480,360, 0);
al_draw_textf(data->font12, al_map_rgb(255,255,255), 530, 360, 0, "%s", Deployed->OccupiedSlot_3->GetSquadName().c_str());
/*Player Stats*/
al_draw_textf(data->font18, al_map_rgb(255,255,255), 450, 430, 0, "Enemies slain:  %i", player->GetEnemiesKilled());
al_draw_textf(data->font18, al_map_rgb(255,255,255), 450, 450, 0, "Victories:      %i", player->GetVictories());
al_draw_textf(data->font18, al_map_rgb(255,255,255), 450, 470, 0, "Defeats:        %i", player->GetDefeats());
};

void GUI::DrawBattleMap()
{
al_draw_bitmap_region(data->bgImage, 0+Stage::cameraX, 0, SCREEN_WIDTH, HEIGHT, 0, 0, 0);
};

void GUI::DrawBattleGUI()
{
    al_draw_bitmap(data->uiImage, 0, 500, 0);
    //Icons, position and costs!
    al_draw_text(data->font18, al_map_rgb(255,255,255), 300, 555, 0, "1.");
    al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_1->GetIconNumber(), 0,40, 40, 320,545, 0);
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 365, 550, 0, "%i", Deployed->OccupiedSlot_1->GetGoldCost());
    al_draw_bitmap_region(data->icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_1->GetGoldCost() < 10 ? 375 : 382, 550, 0);
    if(Deployed->OccupiedSlot_1->GetHonorCost() > 0){
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 365, 570, 0, "%i", Deployed->OccupiedSlot_1->GetHonorCost());
    al_draw_bitmap_region(data->icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_1->GetHonorCost() < 10 ? 375 : 382, 570, 0);}
    al_draw_text(data->font18, al_map_rgb(255,255,255), 400, 555, 0, "2.");
    al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_2->GetIconNumber(), 0,40, 40, 420,545, 0);
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 465, 550, 0, "%i", Deployed->OccupiedSlot_2->GetGoldCost());
    al_draw_bitmap_region(data->icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_2->GetGoldCost() < 10 ? 475 : 482, 550, 0);
    if(Deployed->OccupiedSlot_2->GetHonorCost() > 0){
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 465, 570, 0, "%i", Deployed->OccupiedSlot_2->GetHonorCost());
    al_draw_bitmap_region(data->icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_2->GetHonorCost() < 10 ? 475 : 482, 570, 0);}
    al_draw_text(data->font18, al_map_rgb(255,255,255), 500, 555, 0, "3.");
    al_draw_bitmap_region(data->unit_icon, 40*Deployed->OccupiedSlot_3->GetIconNumber(), 0,40, 40, 520,545, 0);
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 565, 550, 0, "%i", Deployed->OccupiedSlot_3->GetGoldCost());
    al_draw_bitmap_region(data->icons, Gold, 0, 10, 10, Deployed->OccupiedSlot_3->GetGoldCost() < 10 ? 575 : 582, 550, 0);
    if(Deployed->OccupiedSlot_3->GetHonorCost() > 0){
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 565, 570, 0, "%i", Deployed->OccupiedSlot_3->GetHonorCost());
    al_draw_bitmap_region(data->icons, Honor, 0, 10, 10, Deployed->OccupiedSlot_3->GetHonorCost() < 10 ? 575 : 582, 570, 0);}
    /* Basic Selected unit stats*/
    al_draw_textf(data->font12,al_map_rgb(255,255,255),650, 540, 0,"%s",(Deployed->GetSelectedSlot(unit_selected).GetSquadName()).c_str());
    al_draw_textf(data->font12,al_map_rgb(255,255,255),640, 555, 0,"Level: %i",Deployed->GetSelectedSlot(unit_selected).GetLevel());
    al_draw_textf(data->font12,al_map_rgb(255,255,255),640, 570, 0,"Attack: %i",Deployed->GetSelectedSlot(unit_selected).GetDamage());
    al_draw_textf(data->font12,al_map_rgb(255,255,255),640, 585, 0,"Health: %i",Deployed->GetSelectedSlot(unit_selected).GetHp());
    /* Unit Selection Box */
    al_draw_rectangle(220+unit_selected*100,545,260+unit_selected*100,585, al_map_rgb(255, 240, 0), 1);
    /* Row Selected Cursor */
    al_draw_textf(data->font12, al_map_rgb(255,255,255), 5, 410+row_selected*20, 0,"%i. lane>", row_selected);
    /* UI text + small debug*/
    if (state->GamePaused() == true)
    {
    al_draw_text(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 30, ALLEGRO_ALIGN_CENTRE, "Game Paused");
    //al_draw_textf(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTRE, "Enemy Rare count: %i", Stage::GetRareNumber());
    //al_draw_textf(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 70, ALLEGRO_ALIGN_CENTRE, "Enemy Uncommon count: %i", Stage::GetUncommonNumber());
    //al_draw_textf(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 90, ALLEGRO_ALIGN_CENTRE, "Stage Time Elapsed: %i", StageTimeElapsed);
    //al_draw_textf(data->font18, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 110, ALLEGRO_ALIGN_CENTRE, "Objectives Count: %i", Stage::GetObjectivesCount());
    }
        al_draw_textf(data->font18, al_map_rgb(240, 180, 0), SCREEN_WIDTH/5, 545, ALLEGRO_ALIGN_CENTRE, "Gold: %i", (int)Stage::GetStageGold());
        al_draw_bitmap_region(data->icons, Gold, 0, 10, 10, SCREEN_WIDTH/5+60, 545, 0);
        al_draw_textf(data->font18, al_map_rgb(255,255,255), SCREEN_WIDTH/5, 570, ALLEGRO_ALIGN_CENTRE, "Honor: %i", Stage::GetStageHonor());
        al_draw_bitmap_region(data->icons, Honor, 0, 10, 10, SCREEN_WIDTH/5+60, 570, 0);
    if (Stage::STAGE_VICTORY_CONDITION == SIEGE)
    {
    for (int i = 0; i < Stage::GetStageLives(); i++)
    {
        al_draw_textf(data->font18, al_map_rgb(240,30,0), 625, 10, ALLEGRO_ALIGN_CENTER, "Survive for %i more minutes.", 4-(Stage::GetObjectivesCount()));
        al_draw_bitmap(data->heartIcon,5+(i*30), 0, 0);
    }
    }
    if (Stage::STAGE_VICTORY_CONDITION == BLOODBATH)
    {
        al_draw_textf(data->font18, al_map_rgb(240,30,0), SCREEN_WIDTH/1.2, 10, ALLEGRO_ALIGN_CENTER, "Kills to win: %i", 50-(Stage::GetObjectivesCount()));
        //al_draw_textf(data->font18, al_map_rgb(255,255,255), 5, 10, 0, "Hero hp: %i", (*ptr_to_hero).GetHp());
    }
    if (Stage::STAGE_VICTORY_CONDITION == HERO_HUNTING)
    {
        //al_draw_textf(data->font18, al_map_rgb(255,255,255), 560, 10, 0, "Enemy Hero Hp: %i", (*ptr_to_enemy).GetHp());
        //al_draw_textf(data->font18, al_map_rgb(255,255,255), 5, 10, 0, "Hero Hp: %i", (*ptr_to_hero).GetHp());
    }
    if (state->GetState() == PLAYING && state->GamePaused() == false ) Stage::StageTimeElapsed++;
        al_draw_textf(data->font18, al_map_rgb(255,255,255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTER, "%i:%i",(Stage::StageTimeElapsed/60/60),(Stage::StageTimeElapsed/60)%60);
    if(state->GetState() == AFTERMATCH)
    {
        if(Stage::GetAftermachStatus() == DEFEAT)
        {
            al_draw_text(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2-30, ALLEGRO_ALIGN_CENTRE, "You have been defeated!");
            al_draw_text(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press spacebar to continue!");
        }
        if(Stage::GetAftermachStatus() == VICTORY)
        {
        al_draw_text(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2-30, ALLEGRO_ALIGN_CENTRE, "You have proved victorious!");
        if(Deployed->OccupiedSlot_1->CheckLevel()== true)
            al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_1->GetSquadName().c_str());
        else al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_1->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
        if(Deployed->OccupiedSlot_2->CheckLevel()== true)
            al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_2->GetSquadName().c_str());
        else al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_2->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
        if(Deployed->OccupiedSlot_3->CheckLevel()== true)
            al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+60, ALLEGRO_ALIGN_CENTRE," Your %s have leveled up!", Deployed->OccupiedSlot_3->GetSquadName().c_str());
        else al_draw_textf(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+60, ALLEGRO_ALIGN_CENTRE," %s have gained %i exp points.", Deployed->OccupiedSlot_3->GetSquadName().c_str(), Deployed->Selected_Stage->StageExpReward);
            al_draw_text(data->font18, al_map_rgb(255, 30, 30), SCREEN_WIDTH/2, HEIGHT/2+90, ALLEGRO_ALIGN_CENTRE, "Press spacebar to continue!");
        }
    }
};

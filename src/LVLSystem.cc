#include "LVLSystem.hh"
#include <iostream>

bool LVLSystem::Update(Player &player, Enemypool &enemypool, Hud &hud){

    if(player.option_wahl != 0){
        if(player.option_wahl == 1){
            Option_Eins(player); 

            player.press = false;
            player.option_wahl = 0;
            g_game.state = EnumGameState::PLAY;
            
            hud._board.detach(hud.LVLS);
        }else if (player.option_wahl == 2){
            Option_Zwei(player);

            player.press = false;
            player.option_wahl = 0;
            g_game.state = EnumGameState::PLAY;
            hud._board.detach(hud.LVLS);
        }else if (player.option_wahl == 3){
            Option_drei(player);
            player.press = false;
            player.option_wahl = 0;
            g_game.state = EnumGameState::PLAY;
            hud._board.detach(hud.LVLS);

        }

        return false;

    }
return true;
}

void LVLSystem::Option_Eins(Player &player){
    player.playerSpeed = player.playerSpeed* 1.2;
    std::cout<< "-----Speed------"<<std::endl;
}

void LVLSystem::Option_Zwei(Player &player){
    player.obstacles = player.obstacles +4 ;
    std::cout<< "-----obstacles +4 ------"<<std::endl;
}

void LVLSystem::Option_drei(Player &player){
    player.xp_bekommt = 100;
    std::cout<< "-----3 Option ------"<<std::endl;
}

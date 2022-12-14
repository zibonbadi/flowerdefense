#pragma once
#include "headers.hh"

class LVLSystem{
private:

public:
	bool Update(Player &player, Enemypool &enemypool, Hud &hud);
    void Option_Eins(Player &player);
    void Option_Zwei(Player &player);
    void Option_drei(Player &player);
};

#pragma once
#include "Game.h"

class Player
{
    private:
        Game::HitType playerType;
    public:
        Player(Game::HitType _playerType);
        Coord InputCoordinates();
        Game::HitType GetPlayerType();
        int ShipsLeft;
};
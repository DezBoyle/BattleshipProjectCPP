#include "Player.h"
#include "Display.h"

Player::Player(Game::HitType _playerType)
{
    playerType = _playerType;
}

Coord Player::InputCoordinates()
{
    Coord returnCoord = Coord(-1,-1);

    if(playerType == Game::HitType::PLAYER)
    {
        //Player logic
        Display::Print("X  :  ");
        std::cin >> returnCoord.X;
        Display::Print("Y  :  ");
        std::cin >> returnCoord.Y;
    }
    else
    {
        //Enemy logic
        returnCoord.X = 1;
        returnCoord.Y = 1;
    }

    return returnCoord;
}

Game::HitType Player::GetPlayerType()   {   return playerType;  }
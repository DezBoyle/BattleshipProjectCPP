#include "Player.h"
#include "Display.h"

Player::Player(Game::HitType _playerType)
{
    playerType = _playerType;
    returnCoord = Coord(-1,-1);
}

Coord Player::InputCoordinates()
{
    returnCoord.X = returnCoord.Y = -1;

    if(playerType == Game::HitType::PLAYER)
    {
        //Player logic
        Display::Print("----------\nYOUR TURN\n----------");
        Display::Print("X  :  ", false);
        std::cin >> returnCoord.X;
        Display::Print("Y  :  ", false);
        std::cin >> returnCoord.Y;
        //Display::Print(std::to_string(returnCoord.X));
        //Display::Print(std::to_string(returnCoord.Y));
    }
    else
    {
        //Enemy logic
        Display::Print("\nCOMPUTER'S TURN\n----------");
        returnCoord.X = 1;
        returnCoord.Y = 1;
    }

    return returnCoord;
}

Game::HitType Player::GetPlayerType()   {   return playerType;  }
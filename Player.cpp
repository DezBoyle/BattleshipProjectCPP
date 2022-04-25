#include "Player.h"
#include "Display.h"
#include "Utility.h"

Player::Player(Game::HitType _playerType, Game* _game)
{
    playerType = _playerType;
    returnCoord = Coord(-1,-1);
    lastHit = Game::HitType::EMPTY;
    guessDirection = Direction::UP;
    nearMissCoord = Coord(-1, -1);
    game = _game;
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
        
        if(nearMissCoord.X == -1 && nearMissCoord.Y == -1)
        {
            Display::Print("AI : Im guessing a random coordinate");
            //pick at random, making sure not to pick anything that has been chosen before
            RandomGuess();

            Display::PrintCoord(returnCoord);
            guessDirection = Direction::UP;
        }
        else
        {
            Display::Print("AI : Close.. I'm guessing adjacent to coordinate");
            Display::PrintCoord(nearMissCoord);
            Display::Print("AI : Direction : " + std::to_string((int)guessDirection));

            //pick an adjacent one
            do
            {
                Display::Print("AI : guessing... ", true);
                returnCoord.X = nearMissCoord.X;
                returnCoord.Y = nearMissCoord.Y;
                switch(guessDirection)
                {
                    case Direction::UP:     returnCoord.Y -= 1;    break;
                    case Direction::DOWN:     returnCoord.Y += 1;    break;
                    case Direction::LEFT:     returnCoord.X -= 1;    break;
                    case Direction::RIGHT:     returnCoord.X += 1;    break;
                }
                Display::PrintCoord(returnCoord);

                if(!Game::InBounds(returnCoord))
                {
                    Display::Print("AI : I picked out of bounds.. trying again");
                    returnCoord.X = returnCoord.Y = -1; //out of bounds, try again
                }
                guessDirection = (Direction)((int)guessDirection + 1); //increment
                if((int)guessDirection > 4)
                    RandomGuess();
            }while(returnCoord.X == -1 && returnCoord.Y == -1);
        }
    }

    return returnCoord;
}

void Player::SetHitResult(Game::HitType hitResult)
{
    if(lastHit != Game::HitType::NEARMISS && hitResult == Game::HitType::NEARMISS)
        nearMissCoord = returnCoord; //found a new near miss coord
    else if(hitResult == (playerType == Game::HitType::PLAYER) ? Game::HitType::ENEMY : Game::HitType::PLAYER)
        nearMissCoord.X = nearMissCoord.Y = -1;

    lastHit = hitResult;
}

void Player::RandomGuess()
{
    do{
        returnCoord.X = Utility::RandomRange(0, Game::GridSize);
        returnCoord.Y = Utility::RandomRange(0, Game::GridSize);
    }while(!game->ValidGuess(returnCoord));
}

Game::HitType Player::GetPlayerType()   {   return playerType;  }
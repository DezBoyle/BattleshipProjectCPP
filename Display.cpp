#include "Display.h"

void Display::Print(std::string text)
{
    std::cout << text << std::endl;
}

std::string Display::boardOutput = std::string("");
//I guess we have to initilize it??  Strange syntax

void Display::Clear()
{
    Print("\n\n\n\n\n\n");
}

void Display::DisplayBoard(Game* game)
{
    boardOutput = std::string("\n\n\n    0  1  2  3  4  5  6  7 \n");
    boardOutput += std::string("   ------------------------\n");
    for(int y = 0; y < Game::GridSize; y++)
    {
        boardOutput += std::string(std::to_string(y) + " |");
        for(int x = 0; x < Game::GridSize; x++)
        {
            switch(game->ReadGrid(Coord(x,y), Game::HitType::EMPTY))
            {
                case Game::HitType::EMPTY:
                    boardOutput += std::string(" . ");
                break;
                case Game::HitType::PLAYER:
                    boardOutput += std::string(" O ");
                break;
                case Game::HitType::ENEMY:
                    boardOutput += (Game::DEBUGXRAYHACKS ? std::string(" X ") : std::string(" . "));
                break;
                case Game::MISS: break;
                case Game::NEARMISS: break;
                
            }
        }
        boardOutput += std::string("| " + std::to_string(y) + "\n");
    }
    boardOutput += std::string("   ------------------------\n");
    boardOutput += std::string("    0  1  2  3  4  5  6  7 \n");
    Print(boardOutput);
}
#include "Game.h"
#include "Display.h"
#include "Player.h"
#include "Utility.h"
#include <ctime>
#include <thread>
#include <chrono>

Game::Game()
{
    /*std::cout << "GAME START" << std::endl;
    
    Typing this out so I can try to understand it
    
    gameGrid - pointer to a pointer of ints
        basically a reference to an array of pointers (more arrays)
            ARRAYS ARE JUST POINTERS
    gameGrid = new int*[8]
        Creating a array of 8 int pointers (which are going to be arrays)
    gameGrid[i] = new int[i]
        Creating a new array of ints at the pointer
            gameGrid = pointer.  Index this to get another pointer.
            index that pointer to get a value

    Im assuming when you index something with the [], its just skipping
    through the memory at the size of the data type * index
    */

   srand(time(NULL));

   gameGrid = new int*[GridSize];
    for(int i = 0; i < 8; i++)
        gameGrid[i] = new int[GridSize];
}

Game::~Game()
{
}


void Game::Restart()
{
    player = new Player(HitType::PLAYER);
    enemy = new Player(HitType::ENEMY);

    std::cout << "RE/STARTING GAME" << std::endl;
    playerTurn = true;
    player->ShipsLeft = ShipsPerPlayer;
    enemy->ShipsLeft = ShipsPerPlayer;

    for(int x = 0; x < GridSize; x++)
    {
        for(int y = 0; y < GridSize; y++)
        {
            //gameGrid[x][y] = ((x + y) % 3) ? HitType::ENEMY : HitType::PLAYER;
            gameGrid[x][y] = HitType::EMPTY;
        }
    }

    PlaceShips(player->GetPlayerType());
    PlaceShips(enemy->GetPlayerType());
    
    //go between player and enemy here

    while(true)
    {
        Display::Clear();
        Display::DisplayBoard(this);

        if(DEBUGXRAYHACKS)
            playerTurn = true;

        Coord coord = playerTurn ? player->InputCoordinates() : enemy->InputCoordinates();
        std::string output = std::string("");

        Game::HitType result = ReadGrid(coord, playerTurn ? player->GetPlayerType() : enemy->GetPlayerType());
        if(result == HitType::MISS)
            output = std::string("\n------\nMiss.\n------");
        if(result == HitType::NEARMISS)
            output = std::string("\n------\nNear Miss.\n------");
        if(result == HitType::PLAYER || result == HitType::ENEMY)
            output = std::string("\n------\nHit!\n------");
        Display::Print(output);

        playerTurn = !playerTurn;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void Game::PlaceShips(Game::HitType playerType)
{
    int shipsToPlace = ShipsPerPlayer;
    while(shipsToPlace > 0)
    {
        int randX, randY;
        randX = Utility::RandomRange(0, ShipsPerPlayer);
        randY = Utility::RandomRange(0, ShipsPerPlayer);
    
        if(ReadGrid(Coord(randX, randY), Game::HitType::EMPTY))
        {
            shipsToPlace--;
            gameGrid[randX][randY] = playerType;
        }
    }
}

Game::HitType Game::ReadGrid(Coord coord, Game::HitType playerType)
{
    if(playerType == Game::HitType::EMPTY) //empty means its neither the player or the enemy
        return (Game::HitType)gameGrid[coord.X][coord.Y];

    Game::HitType typeToLookFor = playerType == Game::HitType::PLAYER ? Game::HitType::ENEMY : Game::HitType::PLAYER;
    Game::HitType hit = (Game::HitType)gameGrid[coord.X][coord.Y];
    if(hit == typeToLookFor) //hit the type we're looking for
        return hit;
    
    if(coord.X > 0) //check left
    {
        hit = (Game::HitType)gameGrid[coord.X - 1][coord.Y];
        if(hit == typeToLookFor)
            return Game::HitType::NEARMISS;
    }
    if(coord.X < GridSize - 1) //check right
    {
        hit = (Game::HitType)gameGrid[coord.X + 1][coord.Y];
        if(hit == typeToLookFor)
            return Game::HitType::NEARMISS;
    }
    if(coord.Y > 0) //check up
    {
        hit = (Game::HitType)gameGrid[coord.X][coord.Y - 1];
        if(hit == typeToLookFor)
            return Game::HitType::NEARMISS;
    }
    if(coord.Y < GridSize - 1) //check down
    {
        hit = (Game::HitType)gameGrid[coord.X][coord.Y + 1];
        if(hit == typeToLookFor)
            return Game::HitType::NEARMISS;
    }
    
    return Game::HitType::MISS;
}
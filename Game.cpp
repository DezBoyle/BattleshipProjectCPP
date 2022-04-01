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
    coord = Coord(-1,-1); //a temporary coordinate I wanted to keep in the heap to reuse
}

Game::~Game()
{
}


void Game::Restart()
{
    player = new Player(HitType::PLAYER, this);
    enemy = new Player(HitType::ENEMY, this);

    std::cout << "RE/STARTING GAME" << std::endl;
    playerTurn = true;
    player->ShipsLeft = ShipsPerPlayer;
    enemy->ShipsLeft = ShipsPerPlayer;

    for(int x = 0; x < GridSize; x++)
    {
        for(int y = 0; y < GridSize; y++)
        {
            //clear the grid
            coord.X = x;
            coord.Y = y;
            SetGrid(coord, HitType::EMPTY);
        }
    }

    PlaceShips(player->GetPlayerType());
    PlaceShips(enemy->GetPlayerType());
    
    Display::Clear();
    Display::DisplayBoard(this);

    //Game loop : go between player and enemy here
    while(true)
    {
        if(!playerTurn)
            ClearHitMarkers();
        
        //debug testing bit
        if(DEBUGXRAYHACKS)
            playerTurn = true;

        //get input from the player / AI
        Coord coord = playerTurn ? player->InputCoordinates() : enemy->InputCoordinates();
        Game::HitType result = ReadGrid(coord, playerTurn ? player->GetPlayerType() : enemy->GetPlayerType());

        //we guessed but found an empty space
        if(result == Game::HitType::MISS || result == Game::HitType::EMTPYGUESSED || result == Game::HitType::NEARMISS)
            SetGrid(coord, Game::HitType::EMTPYGUESSED); //clear the slot so it makes it easier to see which ones you already guessed
        else //HIT SOMETHING
            SetGrid(coord, Game::HitType::HITMARKER); //draw a # symbol where a ship was just hit
        Display::DisplayBoard(this);
        
        if(playerTurn)
            Display::PrintHit(result); //show the player what they hit
        else
            enemy->SetHitResult(result);  //tell the AI what it hit

        //count how many ships are left
        player->ShipsLeft = CountShips(player->GetPlayerType());
        enemy->ShipsLeft = CountShips(enemy->GetPlayerType());

        //did we win or lose?
        if(enemy->ShipsLeft <= 0)
        {
            Display::Clear();
            Display::Print("~ YOU WIN ~");
            break;
        }
        if(player->ShipsLeft <= 0)
        {
            Display::Clear();
            Display::Print("~ YOU LOSE ~");
            break;
        }

        playerTurn = !playerTurn;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void Game::PlaceShips(Game::HitType playerType)
{
    int shipsToPlace = ShipsPerPlayer;
    while(shipsToPlace > 0)
    {
        coord.X = Utility::RandomRange(0, GridSize);
        coord.Y = Utility::RandomRange(0, GridSize);
    
        if(ReadGrid(coord, Game::HitType::EMPTY))
        {
            shipsToPlace--;
            SetGrid(coord, playerType);
        }
    }
}

int Game::CountShips(Game::HitType playerType)
{
    int count = 0;
    for(int x = 0; x < GridSize; x++)
    {
        for(int y = 0; y < GridSize; y++)
        {
            coord.X = x;
            coord.Y = y;

            if(ReadGrid(coord, HitType::EMPTY) == playerType)
                count++;
        }
    }
    return count;
}

bool Game::InBounds(Coord coord)
{
    return (coord.X < GridSize && coord.X >= 0 && coord.Y < GridSize && coord.Y >= 0);
}

bool Game::ValidGuess(Coord coord)
{
    return InBounds(coord) && (ReadGrid(coord, Game::HitType::EMPTY) != Game::HitType::EMTPYGUESSED);
}

void Game::ClearHitMarkers()
{
    for(int x = 0; x < GridSize; x++)
    {
        for(int y = 0; y < GridSize; y++)
        {
            coord.X = x;
            coord.Y = y;
            if(ReadGrid(coord, Game::HitType::EMPTY) == Game::HitType::HITMARKER)
                SetGrid(coord, HitType::EMTPYGUESSED);
        }
    }
}

void Game::SetGrid(Coord coord, Game::HitType typeToSet)
{
    gameGrid[coord.X][coord.Y] = typeToSet;
}

Game::HitType Game::ReadGrid(Coord coord, Game::HitType playerType)
{
    if(playerType == Game::HitType::EMPTY) //empty means it will simply return the value on the grid, not gameplay related
        return (Game::HitType)gameGrid[coord.X][coord.Y];

    Game::HitType typeToLookFor = (playerType == Game::HitType::PLAYER) ? Game::HitType::ENEMY : Game::HitType::PLAYER;
    Game::HitType hit = (Game::HitType)gameGrid[coord.X][coord.Y];
    if(hit == typeToLookFor) //hit the type we're looking for
        return typeToLookFor;
    
    if(coord.X > 0)            //check left
        if((Game::HitType)gameGrid[coord.X - 1][coord.Y] == typeToLookFor)
            return Game::HitType::NEARMISS;
    if(coord.X < GridSize - 1) //check right
        if((Game::HitType)gameGrid[coord.X + 1][coord.Y] == typeToLookFor)
            return Game::HitType::NEARMISS;
    if(coord.Y > 0)            //check up
        if((Game::HitType)gameGrid[coord.X][coord.Y - 1] == typeToLookFor)
            return Game::HitType::NEARMISS;
    if(coord.Y < GridSize - 1) //check down
        if((Game::HitType)gameGrid[coord.X][coord.Y + 1] == typeToLookFor)
            return Game::HitType::NEARMISS;

    
    return Game::HitType::MISS;
}
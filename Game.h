#pragma once
#include <iostream>

class Player; //scuffed solution

struct Coord
{
    int X, Y;
    Coord(int x, int y) {   X = x; Y = y;    }
};

class Game
{
private:
    int** gameGrid;
    bool playerTurn = true;
    Player* player;
    Player* enemy;
public:
    static const bool DEBUGXRAYHACKS = true;
    static const int GridSize = 8;
    static const int ShipsPerPlayer = 8;
    enum HitType {PLAYER, ENEMY, MISS, NEARMISS, EMPTY};
    Game();
    ~Game();
    void Restart();
    enum HitType ReadGrid(Coord coord, Game::HitType playerType);
private:
    void PlaceShips(Game::HitType playerType);
};
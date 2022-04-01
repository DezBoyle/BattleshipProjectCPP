#pragma once
#include <iostream>

class Player; //scuffed solution

struct Coord
{
    int X, Y;
    Coord(int x, int y) {   X = x; Y = y;    }
    Coord() {   X = Y = 0;  }
};

class Game
{
public:
    enum HitType {PLAYER, ENEMY, MISS, NEARMISS, EMPTY, EMTPYGUESSED, HITMARKER};

private:
    Coord coord;
    int** gameGrid;
    bool playerTurn = true;
    Player* player;
    Player* enemy;
    void ClearHitMarkers();
    void SetGrid(Coord coord, Game::HitType typeToSet);
public:
    static const bool DEBUGXRAYHACKS = false;
    static const int GridSize = 8;
    static const int ShipsPerPlayer = 8;
    Game();
    ~Game();
    void Restart();
    enum HitType ReadGrid(Coord coord, Game::HitType playerType);
    static bool InBounds(Coord coord);
    bool ValidGuess(Coord coord);
private:
    void PlaceShips(Game::HitType playerType);
};
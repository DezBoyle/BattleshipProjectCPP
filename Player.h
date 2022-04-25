#pragma once
#include "Game.h"

class Player
{
    private:
        Game::HitType playerType;
        Coord returnCoord; //the coord the player/enemy return when InputCoordinates is called.  Also stores the last coord
        Game::HitType lastHit; //the last hitresult (HitType) the AI recieved
        Coord nearMissCoord; //the AI starts guessing around this point
        enum Direction {UP, DOWN, LEFT, RIGHT};
        Player::Direction guessDirection;
        Game* game;
    public:
        Player(Game::HitType _playerType, Game* _game);
        Coord InputCoordinates();
        Game::HitType GetPlayerType();
        int ShipsLeft;
        void SetHitResult(Game::HitType hitResult);
        void RandomGuess();
};
#pragma once
#include <iostream>
#include <string>
#include "Game.h"

class Display
{
    private:
        static std::string boardOutput;
    public:
        static void Print(std::string text);
        static void DisplayBoard(Game* game);
        static void Clear();
};
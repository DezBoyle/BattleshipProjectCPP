#include "Utility.h"


int Utility::RandomRange(int min, int max)
{
    //return (min + (rand() % (max - min)));
    static std::mt19937 gen(std::time(nullptr)); //Better to find another proper source of entropy
    std::uniform_int_distribution<> dis(min, max-1);
    return dis(gen);
}
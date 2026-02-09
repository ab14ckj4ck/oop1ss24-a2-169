//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Dice class. The Dice Class is used to generate random numbers
// to simulate a dice roll
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#include "Dice.hpp"

int Dice::roll(int count, int max)
{
  int sum = 0;
  for (int i = 0; i < count; ++i)
  {
    sum += Oop::Random::getInstance().getRandomNumber(max);
  }
  return sum;
}

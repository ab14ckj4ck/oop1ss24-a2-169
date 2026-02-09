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
#ifndef DICE_HPP
#define DICE_HPP

#include "Random.hpp"

class Dice
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // rolls a random number depending on the upper bound
  //
  // @param count - how often you want to roll
  // @param max - upper bound
  //
  // @return the rolled value as int
  //
  static int roll(int count, int max);
};
#endif

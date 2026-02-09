//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Potion class, it's constructor and the methods. The Potion
// class is used to represent a potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef POTION_HPP
#define POTION_HPP

#include "Consumable.hpp"
#include <string>

class Potion : public Consumable
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for a potion
  //
  // @param abbreviation - abbreviation of the potion type
  // @param amount - amount of similar items
  //
  Potion(const std::string& abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for potion
  //
  //
  Potion(const Potion&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor of potion
  //
  //
  ~Potion() override = default;
};

#endif

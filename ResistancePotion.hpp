//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Potion class, it's constructor and the methods. The Potion
// class is used to represent a potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef RESISTANCEPOTION_HPP
#define RESISTANCEPOTION_HPP

#include "DamageType.hpp"
#include "Potion.hpp"
#include <map>
#include <string>

class ResistancePotion : public Potion
{
private:
  DamageType type_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for resistance potions
  //
  // @param abbreviation - abbreviation of the resistance potion type
  // @param amount - amount of similar items
  //
  ResistancePotion(const std::string& abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for resistance potions
  //
  //
  ResistancePotion(const ResistancePotion&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for potion
  //
  //
  ~ResistancePotion() override = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for type of res potion
  //
  // @return the damage type
  //
  DamageType getType() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // map of abbreviation to DamageType
  //
  //
  static std::map<std::string, DamageType> resistance_potion_items_;
};

#endif

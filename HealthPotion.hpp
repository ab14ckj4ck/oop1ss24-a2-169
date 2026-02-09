//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the HealthPotion class, it's constructor and the methods. The
// HealthPotion class is used to represent a health potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef HEALTHPOTION_HPP
#define HEALTHPOTION_HPP

#include "Potion.hpp"
#include <map>
#include <string>
enum class HealthPotionType
{
  NORMAL,
  GREATER,
  SUPRERIOR
};

class HealthPotion : public Potion
{
private:
  HealthPotionType type_;
  int base_regenerated_health_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for a health potion
  //
  // @param abbreviation - abbreviation of the potion type
  // @param amount - amount of similar items
  //
  HealthPotion(const std::string& abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for health potions
  //
  //
  HealthPotion(const HealthPotion&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for health potions
  //
  //
  ~HealthPotion() override = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  //  map of abbreviation to HealthPotionType
  //
  //
  static std::map<std::string, HealthPotionType> health_potion_items_;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // can be called on a HealthPotion object and sets the base_regenrated_health depending on the type_
  //
  //
  void fillOutHealthPotionObject();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for health potion type
  //
  // @return the type of the health potion
  //
  HealthPotionType getType() { return type_; };
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for type value
  //
  // @return the base regenerated health
  //
  int getBaseRegeneratedHealth() const { return base_regenerated_health_; };
};

#endif

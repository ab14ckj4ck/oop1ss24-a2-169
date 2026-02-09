//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Potion class, it's constructor and the methods. The Potion
// class is used to represent a potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "ResistancePotion.hpp"

ResistancePotion::ResistancePotion(const std::string& abbreviation, int amount) : Potion(abbreviation, amount)
{
  type_ = resistance_potion_items_.at(abbreviation);
  setItemType(ItemType::CONSUMABLE);
};

std::map<std::string, DamageType> ResistancePotion::resistance_potion_items_ = {
    {"FIRS", DamageType::FIRE}, {"CORS", DamageType::COLD}, {"FORS", DamageType::FORCE}, {"ACRS", DamageType::ACID}};

DamageType ResistancePotion::getType() const
{
  return type_;
}

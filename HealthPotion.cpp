//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the HealthPotion class, it's constructor and the methods. The
// HealthPotion class is used to represent a health potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "HealthPotion.hpp"

HealthPotion::HealthPotion(const std::string& abbreviation, int amount) : Potion(abbreviation, amount)
{
  base_regenerated_health_ = 0;
  type_ = health_potion_items_.at(abbreviation);
  setItemType(ItemType::CONSUMABLE);
};

std::map<std::string, HealthPotionType> HealthPotion::health_potion_items_ = {
    {"NHEP", HealthPotionType::NORMAL},
    {"GHEP", HealthPotionType::GREATER},
    {"SHEP", HealthPotionType::SUPRERIOR},
};

void HealthPotion::fillOutHealthPotionObject()
{
  switch (type_)
  {
    case HealthPotionType::NORMAL:
      base_regenerated_health_ = 2;
      break;
    case HealthPotionType::GREATER:
      base_regenerated_health_ = 4;
      break;
    case HealthPotionType::SUPRERIOR:
      base_regenerated_health_ = 8;
      break;
  }
}
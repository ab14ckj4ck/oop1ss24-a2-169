//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Ammunition class, it's constructor and the methods. The
// Ammunition class is used to store the Ammunition attributes
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//

#include "Ammunition.hpp"

std::map<std::string, AmmunitionType> Ammunition::ammunition_items_ = {{"ARRW", AmmunitionType::ARROW},
                                                                       {"BOLT", AmmunitionType::BOLT}};
Ammunition::Ammunition(const std::string abbreviation, int amount) : Consumable(abbreviation, amount)
{
  ammunition_type_ = ammunition_items_.at(abbreviation);
  setItemType(ItemType::CONSUMABLE);
}
AmmunitionType Ammunition::getAmmunitionType() const
{
  return ammunition_type_;
};

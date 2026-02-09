//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Armor class, it's constructor and the methods. The Armor
// class is used to store the type and the baseArmorValue
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#include "Armor.hpp"

Armor::Armor(const std::string abbreviation, int amount) : Item(abbreviation, amount)
{
  armor_type_ = armor_items_.at(abbreviation);
  setItemType(ItemType::ARMOR);
}

std::map<std::string, ArmorType> Armor::armor_items_ = {{"LARM", ArmorType::LEATHER_ARMOR},
                                                        {"BPLT", ArmorType::BREASTPLATE},
                                                        {"CHML", ArmorType::CHAIN_MAIL},
                                                        {"PARM", ArmorType::PLATE_ARMOR}};

void Armor::fillOutArmorObject()
{
  switch (armor_type_)
  {
    case ArmorType::LEATHER_ARMOR:
      base_armor_value_ = 1;
      break;
    case ArmorType::BREASTPLATE:
      base_armor_value_ = 4;
      break;
    case ArmorType::CHAIN_MAIL:
      base_armor_value_ = 6;
      break;
    case ArmorType::PLATE_ARMOR:
      base_armor_value_ = 8;
      break;
  }
}
ArmorType Armor::getArmorType()
{
  return armor_type_;
}

int Armor::getValue() const
{
  return base_armor_value_;
}

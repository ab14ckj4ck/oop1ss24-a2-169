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
#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "Item.hpp"
#include <map>
#include <string>

enum class ArmorType
{
  LEATHER_ARMOR = 1,
  BREASTPLATE = 4,
  CHAIN_MAIL = 6,
  PLATE_ARMOR = 8
};

class Armor : public Item
{
private:
  int base_armor_value_;
  ArmorType armor_type_;

public:
  static std::map<std::string, ArmorType> armor_items_;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for Armor
  //
  //
  Armor(const std::string abbreviation, int amount);

  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for Armor
  //
  //
  Armor(const Armor&) = delete;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // can be called on a Armor object and sets the base_armor_value_ depending on the armor_type_
  //
  // @return the correct ArmorType
  //
  void fillOutArmorObject();
  ArmorType getArmorType();
  int getValue() const;
};

#endif

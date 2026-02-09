//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Item class, it's constructor and the methods. The Item
// class is used as an abstract class for Weapon, Armor and Consumable
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Item.hpp"

std::map<std::string, std::string> Item::abbr_to_name = {
    {"NHEP", "Normal Health Potion"},
    {"GHEP", "Greater Health Potion"},
    {"SHEP", "Superior Health Potion"},
    {"FIRS", "Potion of Fire Resistance"},
    {"CORS", "Potion of Cold Resistance"},
    {"FORS", "Potion of Force Resistance"},
    {"ACRS", "Potion of Acid Resistance"},
    {"ARRW", "Arrow"},
    {"BOLT", "Bolt"},
    {"LARM", "Leather Armor"},
    {"BPLT", "Breastplate"},
    {"CHML", "Chain Mail"},
    {"PARM", "Plate Armor"},
    {"DAGG", "Dagger"},
    {"MACE", "Mace"},
    {"WARH", "Warhammer"},
    {"HAXE", "Handaxe"},
    {"GAXE", "Greataxe"},
    {"RAPI", "Rapier"},
    {"SSWD", "Shortsword"},
    {"LSWD", "Longsword"},
    {"GSWD", "Greatsword"},
    {"SBOW", "Shortbow"},
    {"LBOW", "Longbow"},
    {"LCRS", "Light Crossbow"},
    {"HCRS", "Heavy Crossbow"},
    {"QFIR", "Quarterstaff of Fire"},
    {"QCLD", "Quarterstaff of Cold"},
    {"QACD", "Quarterstaff of Acid"},
    {"QFRC", "Quarterstaff of Force"},
};

Item::Item(const std::string& abbreviation, int amount) : amount_(amount), abbreviation_(abbreviation)
{
  name_ = abbr_to_name.at(abbreviation);
}

void Item::setItemType(ItemType type)
{
  type_ = type;
}

ItemType Item::getItemType()
{
  return type_;
}

int Item::getAmount() const
{
  return amount_;
}

void Item::setAmount(int amount)
{
  amount_ = amount;
}

void Item::setName(std::string* name)
{
  name_ = *name;
}

std::string Item::getName()
{
  return name_;
}

std::string Item::getAbbreviation() const
{
  return abbreviation_;
}

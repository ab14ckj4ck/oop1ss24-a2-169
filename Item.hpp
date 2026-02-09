//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Item class, it's constructor and the methods. The Item
// class is used as an abstract class for Weapon, Armor and Consumable
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef ITEM_HPP
#define ITEM_HPP

#include <map>
#include <string>

enum class ItemType
{
  WEAPON,
  ARMOR,
  CONSUMABLE
};

class Item
{
private:
  int amount_;
  std::string name_;
  std::string abbreviation_;
  ItemType type_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for an item
  //
  // @param abbreviation - abbreviation of the item type
  // @param amount - amount of similar items
  //
  Item(const std::string& abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for item
  //
  //
  Item(const Item&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for item
  //
  //
  virtual ~Item() = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for item type
  //
  // @param type - new type
  //
  void setItemType(ItemType type);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for item type
  //
  // @return a items type
  //
  ItemType getItemType();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for amount
  //
  // @return the amount of an item
  //
  int getAmount() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for amount
  //
  // @param amount - new amount
  //
  void setAmount(int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for the name of an item
  //
  // @param name - new name
  //
  void setName(std::string* name);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for the item's name
  //
  // @return the name of an item
  //
  std::string getName();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for the items abbreviation
  //
  // @return the abbreviation of an item
  //
  std::string getAbbreviation() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // map of abbreviations and full Item names
  //
  //
  //
  static std::map<std::string, std::string> abbr_to_name;
};

#endif

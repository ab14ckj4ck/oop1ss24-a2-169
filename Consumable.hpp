//---------------------------------------------------------------------------------------------------------------------
//
// Description: Virtual class for all consumable items
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef CONSUMABLE_HPP
#define CONSUMABLE_HPP

#include "Item.hpp"
#include <algorithm>
#include <vector>

class Consumable : public Item
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for consumables
  //
  //
  Consumable(const std::string& abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for consumables
  //
  //
  Consumable(const Consumable&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for consumables
  //
  //
  ~Consumable() override = default;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // uses and discards the item
  //
  // @param consumable: represents the item that we are looking for in the inventory
  // @param inventory: inventory that we loop through
  //
  static void discardUsed(Consumable* consumable, std::vector<Item*>* inventory);
};

#endif

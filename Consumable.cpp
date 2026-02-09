//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Command class, it's constructor and the methods. The Command
// class is used to parse the input string into an command object.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#include "Consumable.hpp"

Consumable::Consumable(const std::string& abbreviation, int amount) : Item(abbreviation, amount) {}

void Consumable::discardUsed(Consumable* consumable, std::vector<Item*>* inventory)
{
  auto it = std::find(inventory->begin(), inventory->end(), consumable);

  if (it != inventory->end())
  {
    if ((*it)->getAmount() > 1)
    {
      (*it)->setAmount((*it)->getAmount() - 1);
    }
    else
    {
      inventory->erase(it);
      delete consumable;
    }
  }
}

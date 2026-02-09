//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Potion class, it's constructor and the methods. The Potion
// class is used to represent a potion that can be used by the player to regenerate health
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Potion.hpp"

Potion::Potion(const std::string& abbreviation, int amount) : Consumable(abbreviation, amount) {}

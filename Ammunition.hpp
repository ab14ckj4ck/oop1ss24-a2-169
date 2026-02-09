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

#ifndef AMMUNITION_HPP
#define AMMUNITION_HPP

#include "Consumable.hpp"
#include <map>
#include <string>

enum class AmmunitionType
{
  ARROW,
  BOLT
};

class Ammunition : public Consumable
{
private:
  AmmunitionType ammunition_type_;

public:
  static std::map<std::string, AmmunitionType> ammunition_items_;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for Ammunition
  //
  //
  Ammunition(const std::string abbreviation, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for Ammunition
  //
  //
  Ammunition(const Ammunition&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for Ammunition
  //
  //
  ~Ammunition() override = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for AmmunitionType
  //
  // @return the correct AmmunitionType
  //
  AmmunitionType getAmmunitionType() const;
};

#endif

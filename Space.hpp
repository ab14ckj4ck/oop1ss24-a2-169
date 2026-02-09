//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Space class, it's constructor and the methods. The
// Space class is used the represent a Field that can hold a Character
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef SPACE_HPP
#define SPACE_HPP

#include "Character.hpp"
#include "Field.hpp"
#include "Item.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

class Space : public Field
{
private:
  Character* character_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor of space
  //
  // @param sign - displayed sign
  //
  Space(char sign);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for space
  //
  //
  Space(const Space&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor of space
  //
  //
  ~Space() override;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for character_
  //
  Character* getCharacter() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for character_
  //
  // @param character: character that the member variable is going to point to
  //
  void setCharacter(Character* character);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // tries to loot the potentially dead characters inventory
  //
  // @param player: player that is being looted
  //
  void tryToLoot(Character* player) const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // override Field::draw
  //
  void draw() override;
};

#endif

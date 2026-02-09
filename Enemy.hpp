//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Enemy class. The Enemy Class is used to create enemies for the
// game.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include "Item.hpp"
#include "Random.hpp"
#include <algorithm>
#include <map>
#include <vector>
class Enemy : public Character
{
private:
  std::vector<Item*> lootable_inventory_;
  std::string abbreviation_;

private:
public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  //  constructor for  Enemy
  //
  //
  Enemy(int base_health, int vitality, int strength, int base_armor, EntityType type, int id, DamageType resistant_to);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for enemy
  //
  //
  Enemy(const Enemy&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for enemy
  //
  //
  ~Enemy() override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the EnemyType of the enemy
  //
  // @return a pointer to the current weapon
  //
  Weapon* getCurrentWeapon() override { return Character::getCurrentWeapon(); }
  //--------------------------------------------------------------------------------------------------------------------
  //
  // chooses a vector of fitting weapons for the attack range and chooses a random out of them to attack.
  //
  // @param enemy_pos - position of the attacking enemy
  // @param target_position - position of the player to be attacked
  //
  void chooseWeapon(std::vector<int>* enemy_pos, std::vector<int>* target_position);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the inventory of the enemy
  //
  // @return a pointer to the inventory
  //
  std::vector<Item*>* getInventory() override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // gets the id of the enemy
  //
  // @return the id as int
  //
  int getId() const override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns abbreviation of the enemy type
  //
  // @return a poniter to the abbreviation
  //
  std::string* getAbbreviation();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for type abbreviation
  //
  // @param abbreviation - string that determines the abbreviation
  //
  void setAbbreviation(std::string* abbreviation);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for the lootable inventory
  //
  // @param lootable_inventory - vector of items
  //
  void setLootableInventory(std::vector<Item*>* lootable_inventory);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for lootable inventory
  //
  // @return a pointer to the lootable inventory
  //
  std::vector<Item*>* getLootableInventory() override;
};

#endif

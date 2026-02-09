//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Player class, it's constructor and the methods. The
// Player class is used to represent the player in the game
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Armor.hpp"
#include "Character.hpp"
#include "Potion.hpp"
#include "Weapon.hpp"
#include <cstring>
#include <vector>

class Player : public Character
{
private:
  std::string name_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for player
  //
  // @param base_health : base health of the player
  // @param vitality : vitality of the player
  // @param strength : strength of the player
  // @param base_armor : base armor of the player
  // @param player_type : type of the player
  // @param name : name of the player
  // @param id : id of the player
  // @param resistant_to : damage type the player is resistant to
  //
  Player(int base_health,
         int vitality,
         int strength,
         int base_armor,
         EntityType player_type,
         std::string* name,
         int id,
         DamageType resistant_to);

  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for player
  //
  //
  Player(const Player&) = delete;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for player
  //
  //
  ~Player() override;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints damage type as string
  //
  // @return the full name of a damage type
  //
  std::string damageTypeToString() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // uses a potion from inventory
  //
  // @param potion : Potion to use
  //
  void usePotion(Potion* potion);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the current health of the player
  //
  // @return the current health as int
  //
  int getCurrentHealth();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // sets the current health of the player
  //
  // @param new_health : new health value
  //
  void setCurrentHealth(int new_health) override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the Name of the Player
  //
  // @return a players name
  //
  std::string getName() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // adds an amount of an item to the inventory
  //
  // @param item : item to add
  // @param amount : amount to add
  //
  void addItemAmount(Item* item, int amount);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // adds a looted inventory to own inventory
  //
  // @param loot_inventory - inventory to be looted
  //
  void addLootedInventory(std::vector<Item*>* loot_inventory);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // adds items to the inventory
  //
  // @param inventory - inventory to be added to
  //
  void addToInventory(std::vector<Item*>& inventory);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // finds an item in the inventory
  //
  // @param item - item to be found
  //
  // @return a bool indicating if an item is in the inventory
  //
  bool findInInventory(Item* item);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // player type as string
  //
  // @param type - type to get the string from
  //
  // @return a player type as full text
  //
  static std::string getPlayerTypeText(EntityType type);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for current armor
  //
  // @return a pointer to the current armor
  //
  Armor* getCurrentArmor() const override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for currennt armor
  //
  // @param armor - armor to be set
  //
  void setCurrentArmor(Armor* armor) override;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for current weapon
  //
  // @param weapon - weapon to be set
  //
  void setCurrentWeapon(Weapon* weapon);
};

#endif
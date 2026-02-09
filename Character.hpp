//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Character class, it's constructor and the methods. The
// Character class is used to store the attributes of the characters and the methods to interact with them.
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Ammunition.hpp"
#include "Armor.hpp"
#include "Dice.hpp"
#include "Item.hpp"
#include "Weapon.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
enum class EntityType
{
  BARBARIAN,
  WIZARD,
  ROGUE,
  GOBLIN,
  LICH,
  ZOMBIE,
};
class Character
{

protected:
  const int base_health_;
  int current_health_;
  int previous_health_;
  int max_health_;
  int vitality_;
  int strength_;
  int armor_value_;
  int id_;
  std::string abbreviation_;
  const int base_armor_;
  bool is_alive_;
  bool was_resistant_;
  EntityType entity_type_;
  Weapon* current_weapon_;
  Armor* current_armor_;
  DamageType is_resistant_to_;
  std::vector<Item*> inventory_;

  void addToInventory(std::vector<Item*>* items);

  //--------------------------------------------------------------------------------------------------------------------
  //
  // Calculates correct armor value
  //
  int calculateArmorValue();

public:
  Character(int base_health,
            int vitality,
            int strength,
            int base_armor,
            EntityType entity_type,
            int id,
            DamageType resistant_to);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for Caracter
  //
  //
  Character(const Character&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for Character
  //
  //
  virtual ~Character(); //--------------------------------------------------------------------------------------------------------------------
  //
  // Returns the inventory of the character
  //
  // @return a pointer pointing to the inventory
  //
  virtual std::vector<Item*>* getInventory();

  //--------------------------------------------------------------------------------------------------------------------
  //
  // sets the current health of the character
  //
  // @param new_healt : new health of the character
  //
  virtual void setCurrentHealth(int new_health);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for current weapon
  //
  // @param weapon to be set
  //
  void setCurrentWeapon(Weapon* weapon);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // virtual function to return the current equipped armor
  //
  // @return a pointer to the current worn armor
  //
  virtual Armor* getCurrentArmor() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for current armor
  //
  // @param armor to be set
  //
  virtual void setCurrentArmor(Armor* current_armor);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the Abbreviation of the character type
  //
  // @return a pointer to the abbriviation
  //
  std::string* getAbbreviation();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns id of character
  //
  // @return the id as int
  //
  virtual int getId() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns strength of character
  //
  // @return the strength as int
  //
  int getStrength() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns strength of character
  //
  // @return the previousHealth as int
  //
  int getPreviousHealth() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns vitality of character
  //
  // @return the vitality as int
  //
  int getVitality() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns armor value of character
  //
  // @return the armorvalue as int
  //
  int getArmorValue() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns current health of character
  //
  // @return the current health as int
  //
  int getCurrentHealth() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns max health of character
  //
  // @return the max health as int
  //
  int getMaxHealth() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns if character is alive
  //
  // @return a bool that indicates if the player is alive
  //
  bool isAlive() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns if character was resistant
  //
  // @return was resistant as a bool
  //
  bool getWasResistant() const;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns current weapon.
  //
  // @return a pointer to the current weapon
  //
  virtual Weapon* getCurrentWeapon();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Calc's the amount of damage dealt by an weapon
  //
  // @param curr_weapon : weapon to use
  // @param entity : target of the attack
  //
  // @return the calculated damage as an int
  //
  int calculateDamage(Weapon* curr_weapon, int& rolled) const;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns entity type
  //
  // @return the corresponding Entitytype
  //
  EntityType getEntityType() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the Abbreviation of the entity type
  //
  //
  virtual std::string getEntityTypeAbbreviation();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the string name of a Zombie
  //
  // @return the name of a Character
  //
  std::string getTypeName();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // deletes a used Item out of the inventory
  //
  // @param ammo_type : type of ammo to be deleted
  //
  void discardUsed(AmmunitionType ammo_type);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for resistance
  //
  // @param new resistance
  //
  void setResistantTo(DamageType resistant_to);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // return the current resistance
  //
  // @return the corresponding damage type
  //
  DamageType getResistantTo() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for Alive status
  //
  // @param boolean
  //
  void setIsAlive(bool boolean);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for previous health - used for damage calc / print
  //
  // @param previous_health - set's last health
  //
  void setPreviousHealth(int previous_health);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for previous resistant - used for damage calc / print
  //
  // @param was_resistant - last resistance
  //
  void setWasResistant(bool was_resistant);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns how much damage the currently equipped armor blocks
  //
  // @return the current armor value as int
  //
  int getCurrentArmorValue() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the lootable_inv of enemies, because enemies don't drop their play inventory
  //
  // @return a pointer to the lootable inventory
  //
  virtual std::vector<Item*>* getLootableInventory();
};

#endif

//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Player class, it's constructor and the methods. The
// Player class is used to represent the player in the game
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Player.hpp"

#include "HealthPotion.hpp"
#include "ResistancePotion.hpp"

Player::Player(int base_health,
               int vitality,
               int strength,
               int base_armor,
               EntityType entity_type,
               std::string* name,
               int id,
               DamageType resistant_to)
    : Character(base_health, vitality, strength, base_armor, entity_type, id, resistant_to), name_(*name)
{
  max_health_ = base_health + vitality;
  current_health_ = max_health_;
  previous_health_ = max_health_;
  abbreviation_ = getEntityTypeAbbreviation();
}

Player::~Player()
{
  bool armor_in_inv = false, weapon_in_inv = false;
  for (auto item : inventory_)
  {
    if (item == current_armor_)
    {
      armor_in_inv = true;
    }
    else if (item == current_weapon_)
    {
      weapon_in_inv = true;
    }
  }

  if (!weapon_in_inv)
  {
    delete current_weapon_;
  }
  if (!armor_in_inv)
  {
    delete current_armor_;
  }
}
std::string Player::damageTypeToString() const
{
  switch (is_resistant_to_)
  {
    case DamageType::FIRE:
      return "Fire";
    case DamageType::COLD:
      return "Cold";
    case DamageType::FORCE:
      return "Force";
    case DamageType::ACID:
      return "Acid";
    case DamageType::PHYSICAL:
      return "Physical";
    case DamageType::NONE:
      return "None";
  }
  return "None";
}

void Player::usePotion(Potion* potion)
{
  HealthPotion* health_potion = dynamic_cast<HealthPotion*>(potion);
  std::cout << getTypeName() << " [" << abbreviation_ << "] \"" << name_ << "\" consumed \"" << potion->getName()
            << "\"." << std::endl;
  if (health_potion != nullptr)
  {
    int health_add = 0;
    switch (health_potion->getType())
    {
      case HealthPotionType::NORMAL:
        health_add += Dice::roll(2, 4);
        std::cout << "[Dice Roll] 2 d4 resulting in a total value of " << health_add << "." << std::endl;
        std::cout << "\n"
                  << getTypeName() << " [" << abbreviation_ << "] \"" << name_ << "\" regenerates "
                  << abs(current_health_ - max_health_) << " health." << std::endl;
        current_health_ += health_add;
        if (current_health_ > max_health_)
        {
          current_health_ = max_health_;
        }
        break;
      case HealthPotionType::GREATER:
        health_add += Dice::roll(4, 4);
        std::cout << "[Dice Roll] 4 d4 resulting in a total value of " << health_add << "." << std::endl;
        std::cout << "\n"
                  << getTypeName() << " [" << abbreviation_ << "] \"" << name_ << "\" regenerates "
                  << abs(current_health_ - max_health_) << " health." << std::endl;
        ;
        current_health_ += health_add;
        if (current_health_ > max_health_)
        {
          current_health_ = max_health_;
        }
        break;
      case HealthPotionType::SUPRERIOR:
        health_add += Dice::roll(8, 4);
        std::cout << "[Dice Roll] 8 d4 resulting in a total value of " << health_add << "." << std::endl;
        std::cout << "\n"
                  << getTypeName() << " [" << abbreviation_ << "] \"" << name_ << "\" regenerates "
                  << abs(current_health_ - max_health_) << " health." << std::endl;
        ;
        current_health_ += health_add;
        if (current_health_ > max_health_)
        {
          current_health_ = max_health_;
        }
        break;
    }
  }

  ResistancePotion* res_potion = dynamic_cast<ResistancePotion*>(potion);
  if (res_potion != nullptr)
  {
    switch (res_potion->getType())
    {
      case DamageType::FIRE:
        is_resistant_to_ = DamageType::FIRE;
        break;
      case DamageType::COLD:
        is_resistant_to_ = DamageType::COLD;
        break;
      case DamageType::FORCE:
        is_resistant_to_ = DamageType::FORCE;
        break;
      case DamageType::ACID:
        is_resistant_to_ = DamageType::ACID;
        break;
      case DamageType::PHYSICAL:
        is_resistant_to_ = DamageType::PHYSICAL;
        break;
      case DamageType::NONE:
        is_resistant_to_ = DamageType::NONE;
        break;
    }
    std::cout << getTypeName() << " [" << abbreviation_ << "] \"" << name_ << "\" is now resistant to \""
              << damageTypeToString() << "\" until leaving the room." << std::endl;
  }
  Consumable::discardUsed(potion, &inventory_);
}

std::string Player::getName() const
{
  return name_;
}

int Player::getCurrentHealth()
{
  return current_health_;
}

void Player::setCurrentHealth(int new_health)
{
  current_health_ = new_health;
}

void Player::addItemAmount(Item* looted_item, int amount)
{
  for (auto& item : inventory_)
  {
    if (item->getAbbreviation() == looted_item->getAbbreviation())
    {
      item->setAmount(item->getAmount() + amount);
      delete looted_item;
      break;
    }
  }
}

void Player::addLootedInventory(std::vector<Item*>* loot_inventory)
{
  for (auto item : *loot_inventory)
  {
    if (findInInventory(item))
    {
      addItemAmount(item, item->getAmount());
    }
    else if (current_weapon_ != nullptr && current_weapon_->getAbbreviation() == item->getAbbreviation())
    {
      current_weapon_->setAmount(current_weapon_->getAmount() + item->getAmount() - 1);
      inventory_.push_back(current_weapon_);
      delete item;
    }
    else if (current_armor_ != nullptr && current_armor_->getAbbreviation() == item->getAbbreviation())
    {
      current_armor_->setAmount(current_armor_->getAmount() + item->getAmount() - 1);
      inventory_.push_back(current_armor_);
      delete item;
    }
    else
    {
      inventory_.push_back(item);
    }
  }
  loot_inventory->clear();
}

void Player::addToInventory(std::vector<Item*>& inventory)
{
  for (auto item : inventory)
  {
    if (findInInventory(item))
    {
      addItemAmount(item, item->getAmount());
    }
    else
    {
      inventory_.push_back(item);
    }
  }
}

bool Player::findInInventory(Item* item)
{
  for (size_t i = 0; i < inventory_.size(); i++)
  {
    if (inventory_.at(i)->getAbbreviation() == item->getAbbreviation())
    {
      return true;
    }
  }
  return false;
}
std::string Player::getPlayerTypeText(EntityType type)
{
  std::string player_type_text;
  switch (type)
  {
    case EntityType::BARBARIAN:
      player_type_text = "Barbarian";
      break;
    case EntityType::WIZARD:
      player_type_text = "Wizard";
      break;
    case EntityType::ROGUE:
      player_type_text = "Rogue";
      break;
    default:
      player_type_text = "x";
  }
  return player_type_text;
}

Armor* Player::getCurrentArmor() const
{
  return Character::getCurrentArmor();
}

void Player::setCurrentArmor(Armor* armor)
{
  Character::setCurrentArmor(armor);
}

void Player::setCurrentWeapon(Weapon* weapon)
{
  Character::setCurrentWeapon(weapon);
}

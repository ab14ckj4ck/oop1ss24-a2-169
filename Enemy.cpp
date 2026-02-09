//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Enemy class. The Enemy Class is used to create enemies for the
// game.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Enemy.hpp"

Enemy::Enemy(int base_health,
             int vitality,
             int strength,
             int base_armor,
             EntityType entity_type,
             int id,
             DamageType resistant_to)
    : Character(base_health, vitality, strength, base_armor, entity_type, id, resistant_to)
{
  abbreviation_ = Character::getEntityTypeAbbreviation();
  max_health_ = base_health;
  current_health_ = max_health_;
  previous_health_ = max_health_;
}

Enemy::~Enemy()
{
  if (!lootable_inventory_.empty())
  {
    for (auto& item : lootable_inventory_)
    {
      delete item;
    }
  }
  lootable_inventory_.clear();
}

std::vector<Item*>* Enemy::getInventory()
{
  return &inventory_;
}

int Enemy::getId() const
{
  return id_;
}

void Enemy::chooseWeapon(std::vector<int>* enemy_pos, std::vector<int>* target_position)
{
  bool is_x_adjacent =
      enemy_pos->at(0) - target_position->at(0) <= 1 && enemy_pos->at(0) - target_position->at(0) >= -1;
  bool is_y_adjacent =
      enemy_pos->at(1) - target_position->at(1) <= 1 && enemy_pos->at(1) - target_position->at(1) >= -1;

  std::vector<Weapon*> usable_weapons;
  for (auto item : inventory_)
  {
    Weapon* weapon = dynamic_cast<Weapon*>(item);
    if (weapon != nullptr)
    {
      if (weapon->getAttackType() == AttackType::MELEE && is_x_adjacent && is_y_adjacent)
      {
        usable_weapons.push_back(weapon);
      }
      else if (weapon->getAttackType() == AttackType::RANGED)
      {
        usable_weapons.push_back(weapon);
      }
    }
  }
  if (!usable_weapons.empty())
  {
    int random_int = Oop::Random::getInstance().getRandomNumber(usable_weapons.size());
    current_weapon_ = usable_weapons.at(random_int - 1);
  }
}

std::string* Enemy::getAbbreviation()
{
  return &abbreviation_;
}

void Enemy::setAbbreviation(std::string* abbreviation)
{
  abbreviation_ = *abbreviation;
}

void Enemy::setLootableInventory(std::vector<Item*>* lootable_inventory)
{
  lootable_inventory_ = *lootable_inventory;
}

std::vector<Item*>* Enemy::getLootableInventory()
{
  return &lootable_inventory_;
}

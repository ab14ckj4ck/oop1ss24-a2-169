//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Character class, it's constructor and the methods. The
// Character class is used to store the attributes of the characters and the methods to interact with them.
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#include "Character.hpp"

#include "Ammunition.hpp"

Character::Character(int base_health,
                     int vitality,
                     int strength,
                     int base_armor,
                     EntityType entity_type,
                     int id,
                     DamageType resistant_to)
    : base_health_(base_health), vitality_(vitality), strength_(strength), base_armor_(base_armor),
      entity_type_(entity_type), is_resistant_to_(resistant_to)
{
  max_health_ = base_health_;
  previous_health_ = base_health_;
  current_health_ = base_health_;
  armor_value_ = base_armor;
  is_alive_ = true;
  id_ = id;
  was_resistant_ = false;
  current_armor_ = nullptr;
  current_weapon_ = nullptr;
}

Character::~Character()
{
  for (auto& item : inventory_)
  {
    delete item;
  }
  inventory_.clear();
}

std::string* Character::getAbbreviation()
{
  return &abbreviation_;
}

Weapon* Character::getCurrentWeapon()
{
  return current_weapon_;
}

std::vector<Item*>* Character::getInventory()
{
  return &inventory_;
}

int Character::getStrength() const
{
  return strength_;
}

int Character::getVitality() const
{
  return vitality_;
}

int Character::getArmorValue() const
{
  return armor_value_;
}

int Character::getCurrentHealth() const
{
  return current_health_;
}

bool Character::isAlive() const
{
  return is_alive_;
}

void Character::setIsAlive(bool boolean)
{
  is_alive_ = boolean;
}

int Character::getMaxHealth() const
{
  return max_health_;
}

EntityType Character::getEntityType() const
{
  return entity_type_;
}

int Character::getId() const
{
  return id_;
}

int Character::getPreviousHealth() const
{
  return previous_health_;
}

void Character::setPreviousHealth(int previous_health)
{
  previous_health_ = previous_health;
}

bool Character::getWasResistant() const
{
  return was_resistant_;
}

void Character::setWasResistant(bool was_resistant)
{
  was_resistant_ = was_resistant;
}

void Character::setCurrentHealth(int current_health)
{
  current_health_ = current_health;
  if (current_health_ <= 0)
  {
    is_alive_ = false;
  }
}

void Character::setCurrentWeapon(Weapon* weapon)
{
  current_weapon_ = weapon;
}

void Character::addToInventory(std::vector<Item*>* items)
{
  inventory_.insert(inventory_.end(), items->begin(), items->end());
}

int Character::calculateDamage(Weapon* curr_weapon, int& rolled) const
{
  rolled = 0;
  WeaponType weapon_type = curr_weapon->getWeaponType();
  int damage = 0;
  int str = getStrength();
  int vit = getVitality();
  switch (weapon_type)
  {
    /* Strength */
    case WeaponType::MACE:
      rolled = Dice::roll(1, 6);
      damage = rolled + str;
      break;
    case WeaponType::WARHAMMER:
      rolled = Dice::roll(1, 10);
      damage = rolled + str;
      break;
    case WeaponType::HANDAXE:
      rolled = Dice::roll(1, 6);
      damage = rolled + str;
      break;
    case WeaponType::GREATAXE:
      rolled = Dice::roll(1, 12);
      damage = rolled + str;
      break;
    case WeaponType::LONGSWORD:
      rolled = Dice::roll(1, 10);
      damage = rolled + str;
      break;
    case WeaponType::GREATSWORD:
      rolled = Dice::roll(2, 6);
      damage = rolled + str;
      break;

    /* Vitality */
    case WeaponType::DAGGER:
      rolled = Dice::roll(1, 4);
      damage = rolled + vit;
      break;
    case WeaponType::RAPIER:
      rolled = Dice::roll(1, 8);
      damage = rolled + vit;
      break;
    case WeaponType::SHORTSWORD:
      rolled = Dice::roll(1, 6);
      damage = rolled + vit;
      break;
    case WeaponType::SHORTBOW:
      rolled = Dice::roll(1, 6);
      damage = rolled + vit;
      break;
    case WeaponType::LONGBOW:
      rolled = Dice::roll(1, 8);
      damage = rolled + vit;
      break;
    case WeaponType::LIGHT_CROSSBOW:
      rolled = Dice::roll(1, 8);
      damage = rolled + vit;
      break;
    case WeaponType::HEAVY_CROSSBOW:
      rolled = Dice::roll(1, 10);
      damage = rolled + vit;
      break;

    /* Staff's */
    case WeaponType::QUARTERSTAFF_OF_FIRE:
      if (getEntityType() == EntityType::WIZARD || getEntityType() == EntityType::LICH)
      {
        rolled = Dice::roll(3, 6);
        damage = rolled;
        break;
      }
      else
      {
        rolled = Dice::roll(1, 6);
        damage = rolled + str;
        break;
      }
    case WeaponType::QUARTERSTAFF_OF_COLD:
      if (getEntityType() == EntityType::WIZARD || getEntityType() == EntityType::LICH)
      {
        rolled = Dice::roll(2, 10);
        damage = rolled;
        break;
      }
      else
      {
        rolled = Dice::roll(1, 6);
        damage = rolled + str;
        break;
      }
    case WeaponType::QUARTERSTAFF_OF_ACID:
    case WeaponType::QUARTERSTAFF_OF_FORCE:
      if (getEntityType() == EntityType::WIZARD || getEntityType() == EntityType::LICH)
      {
        rolled = Dice::roll(1, 10);
        damage = rolled;
        break;
      }
      else
      {
        rolled = Dice::roll(1, 6);
        damage = rolled + str;
        break;
      }
  }
  return damage;
}

std::string Character::getEntityTypeAbbreviation()
{
  switch (entity_type_)
  {
    case EntityType::LICH:
      return "L";
    case EntityType::ZOMBIE:
      return "Z";
    case EntityType::GOBLIN:
      return "G";
    case EntityType::WIZARD:
      return "W";
    case EntityType::ROGUE:
      return "R";
    case EntityType::BARBARIAN:
      return "B";
  }
}
std::string Character::getTypeName()
{
  switch (entity_type_)
  {
    case EntityType::LICH:
      return "Lich";
    case EntityType::ZOMBIE:
      return "Zombie";
    case EntityType::GOBLIN:
      return "Goblin";
    case EntityType::WIZARD:
      return "Wizard";
    case EntityType::ROGUE:
      return "Rogue";
    case EntityType::BARBARIAN:
      return "Barbarian";
  }
}

Armor* Character::getCurrentArmor() const
{
  return current_armor_;
}

void Character::setCurrentArmor(Armor* current_armor)
{
  current_armor_ = current_armor;
}

void Character::discardUsed(AmmunitionType ammo_type)
{
  auto it = std::find_if(inventory_.begin(), inventory_.end(), [ammo_type](Item* item) {
    return item->getItemType() == ItemType::CONSUMABLE &&
           static_cast<Ammunition*>(item)->getAmmunitionType() == ammo_type;
  });
  if (it != inventory_.end())
  {
    Ammunition* ammo = static_cast<Ammunition*>(*it);
    if (ammo->getAmount() > 1)
    {
      ammo->setAmount(ammo->getAmount() - 1);
    }
    else
    {
      delete ammo;
      inventory_.erase(it);
    }
  }
}

void Character::setResistantTo(DamageType resistant_to)
{
  is_resistant_to_ = resistant_to;
}

DamageType Character::getResistantTo() const
{
  return is_resistant_to_;
}

int Character::getCurrentArmorValue() const
{
  if (current_armor_ == nullptr)
  {
    return base_armor_;
  }
  int base_armor = current_armor_->getValue();
  switch (current_armor_->getArmorType())
  {
    case ArmorType::LEATHER_ARMOR:
      return base_armor + vitality_;
    case ArmorType::BREASTPLATE:
      return base_armor + std::min(vitality_, 2);
    case ArmorType::CHAIN_MAIL:
      return 6;
    case ArmorType::PLATE_ARMOR:
      return 8;
  }
}

std::vector<Item*>* Character::getLootableInventory()
{
  return nullptr;
}
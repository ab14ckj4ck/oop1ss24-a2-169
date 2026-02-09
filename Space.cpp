//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Space class, it's constructor and the methods. The
// Space class is used the represent a Field that can hold a Character
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Space.hpp"

#include "Enemy.hpp"
#include "Player.hpp"

Space::Space(char sign) : Field(sign)
{
  character_ = nullptr;
}

Space::~Space()
{
  if (dynamic_cast<Enemy*>(character_) != nullptr)
  {
    delete character_;
  }
}

void Space::draw()
{
  std::string sign = " ";
  std::string id = " ";
  if (character_ != nullptr)
  {
    if (character_->getEntityType() == EntityType::ZOMBIE || character_->getEntityType() == EntityType::LICH ||
        character_->getEntityType() == EntityType::GOBLIN)
    {
      id = std::to_string(character_->getId());
    }
    if (character_->isAlive())
    {
      sign = character_->getEntityTypeAbbreviation();
    }
    else
    {
      sign = "X";
      id = " ";
    }
  }
  std::cout << " " << sign << id;
}
Character* Space::getCharacter() const
{
  return character_;
}
void Space::setCharacter(Character* character)
{
  character_ = character;
}
void Space::tryToLoot(Character* player) const
{
  if (getCharacter() != nullptr && !getCharacter()->isAlive())
  {
    std::vector<Item*>* player_inv = player->getInventory();
    std::vector<Item*>* space_inv = getCharacter()->getInventory();
    std::vector<Item*> items_to_remove;
    for (auto item : *space_inv)
    {
      player_inv->push_back(item);
      items_to_remove.push_back(item);
    }
    for (auto item : items_to_remove)
    {
      auto new_end = std::remove(space_inv->begin(), space_inv->end(), item);
      space_inv->erase(new_end, space_inv->end());
    }
  }
}
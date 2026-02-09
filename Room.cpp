//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Room class, it's constructor and the methods. The Room
// class is used to represent a room in the game
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Room.hpp"

const int Room::ADJACENT_ROOMS_SIZE = 9;

Room::Room(int room_id, int width, int height, bool is_visited)
    : adjacent_rooms_(ADJACENT_ROOMS_SIZE), height_(height), width_(width), is_visited_(is_visited), room_id_(room_id)
{
  is_completed_ = false;
  for (int width_iterator = 0; width_iterator < height; ++width_iterator)
  {
    std::vector<Field*> new_row;

    for (int height_iterator = 0; height_iterator < width; ++height_iterator)
    {
      new_row.push_back(new Space(' '));
    }
    fields_.push_back(new_row);
  }
}

Room::~Room()
{
  if (!fields_.empty())
  {
    for (auto& field_row : fields_)
    {
      for (auto& field : field_row)
      {
        delete field;
      }
      field_row.clear();
    }
  }
  fields_.clear();
  adjacent_rooms_.clear();
  enemies_in_room_.clear();
}

int Room::getRoomId() const
{
  return room_id_;
}

std::vector<std::vector<Field*>>* Room::getFields()
{
  return &fields_;
}

std::vector<Room*>* Room::getAdjacentRooms()
{
  return &adjacent_rooms_;
}

void Room::draw()
{
  std::cout << "   ";
  for (int i = 0; i < width_; ++i)
  {
    std::cout << "  " << i + 1 << " ";
  }
  std::cout << " " << std::endl;
  drawBorderRow();
  for (int row = 0; row < height_; ++row)
  {
    std::cout << " " << row + 1 << " |";
    for (int col = 0; col < width_; ++col)
    {
      fields_.at(row).at(col)->draw();
      std::cout << "|";
    }
    std::cout << std::endl;
    drawBorderRow();
  }
  cleanAttackFields();
}

void Room::drawBorderRow() const
{
  std::cout << "   +";
  for (int col = 0; col < width_; ++col)
  {
    std::cout << "---+";
  }
  std::cout << std::endl;
}

void Room::printEnemies()
{
  bool first = true;
  for (auto enemy : enemies_in_room_)
  {
    if (enemy->isAlive())
    {
      if (first)
      {
        first = false;
        std::cout << "   ";
      }
      else
      {
        std::cout << ", ";
      }
      std::cout << *enemy->getAbbreviation() << enemy->getId() << ": " << enemy->getCurrentHealth() << "/"
                << enemy->getMaxHealth();
    }
  }
  std::cout << std::endl;
}

int Room::findColumn(Character* entity)
{
  for (int row = 0; row < height_; row++)
  {
    for (int col = 0; col < width_; col++)
    {
      Space* space = dynamic_cast<Space*>(fields_.at(row).at(col));
      if (space != nullptr)
      {
        if (space->getCharacter() == entity)
        {
          return col;
        }
      }
    }
  }
  return -1;
}

int Room::findRow(Character* entity)
{
  for (int row = 0; row < height_; row++)
  {
    for (int col = 0; col < width_; col++)
    {
      Space* space = dynamic_cast<Space*>(fields_.at(row).at(col));
      if (space != nullptr)
      {
        if (space->getCharacter() == entity)
        {
          return row;
        }
      }
    }
  }
  return -1;
}

Field* Room::findEntityField(Character* entity)
{
  for (int row = 0; row < height_; ++row)
  {
    for (int col = 0; col < width_; ++col)
    {
      if (typeid(fields_.at(row).at(col)) == typeid(Space))
      {
        Space* space = dynamic_cast<Space*>(fields_.at(row).at(col));
        if (space->getCharacter() == entity)
        {
          return fields_.at(row).at(col);
        }
      }
    }
  }
  return nullptr;
}

std::vector<Enemy*>* Room::getEnemiesInRoom()
{
  return &enemies_in_room_;
}

Field* Room::findField(int col, int row)
{
  return fields_.at(row).at(col);
}

void Room::cleanAttackFields()
{
  for (int row = 0; row < height_; row++)
  {
    for (int col = 0; col < width_; col++)
    {
      fields_.at(row).at(col)->setIsAttacksTargetField(false);
      fields_.at(row).at(col)->setIsAttacksAffectedField(false);
    }
  }
}

bool Room::getCompleted() const
{
  return is_completed_;
}
void Room::setCompleted(bool is_completed)
{
  is_completed_ = is_completed;
}
void Room::setIsVisited(bool is_visited)
{
  is_visited_ = is_visited;
}
bool Room::getIsVisited() const
{
  return is_visited_;
}

void Room::sortEnemies()
{
  std::sort(enemies_in_room_.begin(), enemies_in_room_.end(), [](const Enemy* a, const Enemy* b) {
    int priority_a = getPriority(a->getEntityType());
    int priority_b = getPriority(b->getEntityType());
    if (priority_a == priority_b)
    {
      return a->getId() < b->getId();
    }
    return priority_a < priority_b;
  });
}

int Room::getPriority(EntityType type)
{
  switch (type)
  {
    case EntityType::GOBLIN:
      return 1;
    case EntityType::LICH:
      return 2;
    case EntityType::ZOMBIE:
      return 3;
    default:
      return 4;
  }
}

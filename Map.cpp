//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Map class, it's constructor and the methods. The
// Map class is used to store the rooms
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Map.hpp"

#include <iostream>

Map::~Map()
{
  if (!rooms_.empty())
  {
    for (auto& room : rooms_)
    {
      delete room;
    }
  }
}

std::vector<Room*>* Map::getRooms()
{
  return &rooms_;
}

void Map::drawCurrentRoom()
{
  current_room_->draw();
}

Room* Map::getCurrentRoom() const
{
  return current_room_;
}

void Map::setCurrentRoom(Room* room)
{
  current_room_ = room;
}

Room* Map::getRoomBasedOnID(int room_id) const
{
  return rooms_.at(room_id - 1);
}
void Map::setPreviousRoom(Room* room)
{
  previous_room_ = room;
}
Room* Map::getPreviousRoom() const
{
  return previous_room_;
}

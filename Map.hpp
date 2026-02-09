//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Map class, it's constructor and the methods. The
// Map class is used to store the rooms
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef MAP_HPP
#define MAP_HPP

#include "Room.hpp"
#include <vector>
class Map
{
private:
  std::vector<Room*> rooms_;
  Room* current_room_;
  Room* previous_room_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for map
  //
  //
  Map() = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for map
  //
  //
  Map(const Map&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for map
  //
  //
  ~Map();

  //--------------------------------------------------------------------------------------------------------------------
  //
  // draws the current room
  //
  //
  void drawCurrentRoom();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for rooms_
  //
  // @return a pointer to the rooms in the map
  //
  std::vector<Room*>* getRooms();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for current_room_
  //
  // @return a pointer to the current room
  //
  Room* getCurrentRoom() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for current_room_
  //
  // @param room: pointer to a room
  //
  void setCurrentRoom(Room* room);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Gets the room in rooms_ based on id
  //
  // @param room_id: specifies the room to look for in rooms_
  //
  // @return a pointer to a room
  //
  Room* getRoomBasedOnID(int room_id) const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for previous room
  //
  // @param room - last room
  //
  void setPreviousRoom(Room* room);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for previous room
  //
  // @return a pointer to the previous room
  //
  Room* getPreviousRoom() const;
};

#endif

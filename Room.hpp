//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Room class, it's constructor and the methods. The Room
// class is used to represent a room in the game
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef ROOM_HPP
#define ROOM_HPP

#include "Enemy.hpp"
#include "Field.hpp"
#include "Space.hpp"
#include <iostream>
#include <memory>
#include <vector>

class Room
{

private:
  std::vector<std::vector<Field*>> fields_;
  std::vector<Room*> adjacent_rooms_;
  std::vector<Enemy*> enemies_in_room_;
  int height_;
  int width_;
  bool is_visited_;
  int room_id_;
  bool is_completed_;

private:
  static const int ADJACENT_ROOMS_SIZE;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // draws the border rows
  //
  //
  void drawBorderRow() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Sets the bools for fields targeted and affected by an attack to false after printing the map
  //
  void cleanAttackFields();
  //---------------------------------------------------------------------------------------------------------------------
  //
  // returns priority of given entity
  //
  // @param type - entity type
  //
  // @returns: the priority of the EntityType when sorting
  //
  //
  static int getPriority(EntityType type);

public:
  //---------------------------------------------------------------------------------------------------------------------
  //
  // constructor for room
  //
  // @param room_id - id of the room
  // @param width - width of the room
  // @param height - height of the room
  // @para is_visited - if the room was visited
  //
  Room(int room_id, int width, int height, bool is_visited);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for room
  //
  //
  Room(const Room&) = delete;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // destructor for room
  //
  // @param
  //
  ~Room();
  //---------------------------------------------------------------------------------------------------------------------
  //
  // getter for height_
  //
  // @returns: height of the room
  //
  //
  int getHeight() const { return height_; }
  //---------------------------------------------------------------------------------------------------------------------
  //
  // getter for width_
  //
  // @returns: width of the room
  //
  //
  int getWidth() const { return width_; }
  //---------------------------------------------------------------------------------------------------------------------
  //
  // getter for is_visited_
  //
  //
  // @returns: whether the room has been visited
  //
  bool isVisited() const { return is_visited_; }
  //---------------------------------------------------------------------------------------------------------------------
  //
  // draws the current room
  //
  //
  void draw();
  //---------------------------------------------------------------------------------------------------------------------
  //
  // finds the field where the given entity is on in the current room
  //
  // @param entity
  //
  // @returns: the found Field
  //
  Field* findEntityField(Character* entity);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // finds a field by x and y in the current room
  //
  // @param x
  // @param y
  //
  // @returns: the found Field
  //
  Field* findField(int x, int y);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // finds the row of an entity
  //
  // @param entity
  //
  // returns: the index of the row
  //
  int findRow(Character* entity);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // finds the column of an entity
  //
  // @param entity
  //
  // returns: the index of the collumn
  //
  int findColumn(Character* entity);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // getter for completed_
  //
  //
  bool getCompleted() const;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // setter for completed_
  //
  // @param is_completed - status if room is completed
  //
  void setCompleted(bool is_completed);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for enemies_in_room_
  //
  // @returns: vector of enemies in room
  //
  std::vector<Enemy*>* getEnemiesInRoom();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for fields_
  //
  // @returns: Fields in Room
  //
  std::vector<std::vector<Field*>>* getFields();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for adjacent_rooms_
  //
  // @returns: vector of adjacent Rooms
  //
  std::vector<Room*>* getAdjacentRooms();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for is_visited_
  //
  // @param value to be set
  //
  void setIsVisited(bool is_visited);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for is_visited_
  //
  //
  // @returns: whether the Room has been visited
  //
  bool getIsVisited() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns current room's id
  //
  //
  // @returns: The Rooms id
  //
  //
  int getRoomId() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // sorts enemies in room
  //
  //
  void sortEnemies();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // outputs the enemies in the room
  //
  void printEnemies();
};

#endif

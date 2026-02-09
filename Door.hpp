//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Door class, it's constructor and the methods. The
// Door class is used the represent a Field that can access another Room
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef DOOR_HPP
#define DOOR_HPP
#include "Field.hpp"

class Door : public Field
{
private:
  bool is_locked_;
  int id_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for door
  //
  //
  Door(int id, char sign);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for door
  //
  //
  Door(const Door&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for door
  //
  //
  ~Door();

  //---------------------------------------------------------------------------------------------------------------------
  //
  // tries to open the door
  //
  void unlock();
  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for is_locked_
  //
  // @return bool wheter door is locked or not
  //
  bool getIsLocked() const;
  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for is_locked_
  //
  // @param is_locked: new value for is_locked_
  //
  void setIsLocked(bool is_locked);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for id_
  //
  // @return the id as int
  //
  int getId() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // draw's the current room.
  //
  void draw() override;
};
#endif

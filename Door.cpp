//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Door class, it's constructor and the methods. The
// Door class is used the represent a Field that can access another Room
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Door.hpp"

#include <iostream>

Door::Door(int id, char sign) : Field(sign), is_locked_(true), id_(id) {}

Door::~Door() {}

bool Door::getIsLocked() const
{
  return is_locked_;
}

void Door::setIsLocked(bool is_locked)
{
  is_locked_ = is_locked;
}

void Door::draw()
{
  std::string locked = " ";
  if (is_locked_)
  {
    locked = "#";
  }
  std::cout << locked << sign_ << id_;
}

int Door::getId() const
{
  return id_;
}

void Door::unlock()
{
  is_locked_ = false;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Field class, it's constructor and the methods. The
// Field class is used as an abstract class to represent the tiles in a room
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Field.hpp"

Field::Field(char sign) : sign_(sign)
{
  is_attacks_affected_field_ = false;
  is_attacks_target_field_ = false;
}

char Field::getSign() const
{
  return sign_;
}

bool Field::getIsAttacksAffectedField() const
{
  return is_attacks_affected_field_;
}

void Field::setIsAttacksAffectedField(bool is_attacks_affected_field)
{
  is_attacks_affected_field_ = is_attacks_affected_field;
}

bool Field::getIsAttacksTargetField() const
{
  return is_attacks_target_field_;
}

void Field::setIsAttacksTargetField(bool is_attacks_target_field)
{
  is_attacks_target_field_ = is_attacks_target_field;
}

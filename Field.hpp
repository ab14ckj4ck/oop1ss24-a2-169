//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Field class, it's constructor and the methods. The
// Field class is used as an abstract class to represent the tiles in a room
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------

#ifndef FIELD_HPP
#define FIELD_HPP

class Field
{
protected:
  char sign_;
  bool is_attacks_target_field_;
  bool is_attacks_affected_field_;

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // constructor for field
  //
  //
  Field(char sign);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for field
  //
  //
  Field(const Field&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // virtual destructor for Field
  //
  //
  virtual ~Field() = default;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for sign_
  //
  // @return the sign_ as char
  //
  char getSign() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // draws the Field
  //
  virtual void draw() = 0;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for is_attacks_target_field_
  //
  // @return a bool that is the target field
  //
  bool getIsAttacksTargetField() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for is_attacks_target_field_
  //
  // @param is_attacks_target_field: set value
  //
  void setIsAttacksTargetField(bool is_attacks_target_field);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Getter for is_attacks_affected_field
  //
  // @return a bool that is an affected field
  //
  bool getIsAttacksAffectedField() const;

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Setter for is_attacks_affected_field
  //
  // @param is_attacks_affected_field: set value
  //
  void setIsAttacksAffectedField(bool is_attacks_affected_field);
};

#endif

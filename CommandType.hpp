//---------------------------------------------------------------------------------------------------------------------
//
// Class for commands. Handling command preparation
//
// Author: Florian Scherer
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef COMMANDTYPE_HPP
#define COMMANDTYPE_HPP

enum class CommandType
{
  /*Display Commands*/
  HELP,
  STORY,
  MAP,
  POSITIONS,
  PLAYER,
  INVENTORY,

  /*Action Commands*/
  MOVE,
  LOOT,
  USE,
  ATTACK,

  /* Other Commands */
  QUIT,
  UNKNOWN
};

#endif
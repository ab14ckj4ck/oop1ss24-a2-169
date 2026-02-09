//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Command class, it's constructor and the methods. The Command
// class is used to parse the input string into an command object.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "CommandType.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

class Player;
class Game;

enum InvalidPosition
{
  INVALID = -224225
};

class Command
{
private:
  std::string input_;
  CommandType type_;
  std::vector<std::string> parameters_;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // finds the type of the command and assigns fitting enum
  //
  // @param command: command string
  //
  void findType(const std::string& command);

public:
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Command constructor
  //
  // @param input: input string from the user
  //
  explicit Command(std::string& input);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for Command
  //
  //
  Command(const Command&) = delete;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor for command
  //
  //
  ~Command() = default;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // validates if the command got the right amount of parameters or is unknown
  //
  // @return a bool indicating if the command is valid
  //
  bool validate();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // parses the input string and determines the command
  //
  //
  void parseInput();

  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns type of command
  //
  // @return the corresponding type
  //
  CommandType getType() const;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns a vector filled with streams that contain the parameters
  //
  // @return the paramaters of the command
  //
  std::vector<std::string> getParameters() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the player in the command
  //
  // @param game: in order to access players smart pointer
  //
  // @return a smart pointer to a specific player
  //
  std::shared_ptr<Player> parsePlayer(Game* game);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns the position in the command
  //
  // @return the desired position
  //
  std::vector<int> parsePosition() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // splits a string of input coordinates into a vector of int
  //
  // @param input string
  //
  // @return the split coordinates
  //
  static std::vector<int> splitCoordinates(std::string* value);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // trims the brackets on the edges of a string
  //
  // @param input string
  //
  // @return correct string
  //
  static std::string trimBrackets(const std::string& str);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if the input got the right amount of parameters for input command type
  //
  // @return bool indicating if number of parameters is according to the command
  //
  bool paramCheck();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for input
  //
  // @param input string
  //
  void setInput(std::string& input);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if requested player exists in the game
  //
  // @param game: in order to access players smart pointer
  //
  // @return bool indicating if entered player exists in the game
  //
  bool checkPlayers(Game* game);
};

#endif
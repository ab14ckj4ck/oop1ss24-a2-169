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
#include "Command.hpp"

Command::Command(std::string& input) : input_(input), type_(CommandType::HELP)
{
  std::transform(input_.begin(), input_.end(), input_.begin(), ::tolower);
}

CommandType Command::getType() const
{
  return type_;
}

std::vector<std::string> Command::getParameters() const
{
  return parameters_;
}

void Command::parseInput()
{
  parameters_.clear();
  std::istringstream iss(input_);
  std::string token;
  std::string command;

  iss >> command;

  /* Parameters */
  while (iss >> token)
  {
    parameters_.push_back(token);
  }
  if (parameters_.empty())
  {
    return;
  }
  if (type_ == CommandType::PLAYER || type_ == CommandType::INVENTORY || type_ == CommandType::USE ||
      type_ == CommandType::ATTACK || type_ == CommandType::MOVE || type_ == CommandType::LOOT)
  {
    std::transform(parameters_.at(0).begin(), parameters_.at(0).end(), parameters_.at(0).begin(), ::toupper);
  }
}

void Command::findType(const std::string& command)
{
  if (command == "help")
  {
    type_ = CommandType::HELP;
  }
  else if (command == "story")
  {
    type_ = CommandType::STORY;
  }
  else if (command == "map")
  {
    type_ = CommandType::MAP;
  }
  else if (command == "positions")
  {
    type_ = CommandType::POSITIONS;
  }
  else if (command == "player")
  {
    type_ = CommandType::PLAYER;
  }
  else if (command == "inventory")
  {
    type_ = CommandType::INVENTORY;
  }
  else if (command == "move")
  {
    type_ = CommandType::MOVE;
  }
  else if (command == "loot")
  {
    type_ = CommandType::LOOT;
  }
  else if (command == "use")
  {
    type_ = CommandType::USE;
  }
  else if (command == "attack")
  {
    type_ = CommandType::ATTACK;
  }
  else if (command == "quit" || command.empty())
  {
    type_ = CommandType::QUIT;
  }
  else
  {
    type_ = CommandType::UNKNOWN;
  }
}

bool Command::validate()
{
  bool is_correct;

  /* Find CommandType */
  std::istringstream iss(input_);
  std::string command;
  iss >> command;
  findType(command);

  switch (type_)
  {
    case CommandType::HELP:
    case CommandType::QUIT:
    case CommandType::STORY:
    case CommandType::MAP:
    case CommandType::POSITIONS:
      try
      {
        parseInput();
      }
      catch (const std::exception& e)
      {
        is_correct = false;
        break;
      }
      is_correct = true;
      break;

    case CommandType::PLAYER:
    case CommandType::INVENTORY:
      try
      {
        parseInput();
      }
      catch (const std::exception& e)
      {
        is_correct = false;
        break;
      }
      is_correct = true;
      break;

    case CommandType::MOVE:
    case CommandType::LOOT:
    case CommandType::USE:
    case CommandType::ATTACK:
      try
      {
        parseInput();
        if (parameters_.size() >= 2)
        {
          std::transform(parameters_.at(1).begin(), parameters_.at(1).end(), parameters_.at(1).begin(), ::toupper);
        }
      }
      catch (const std::exception& e)
      {
        is_correct = false;
        break;
      }
      is_correct = true;
      break;

    default:
      is_correct = false;
      break;
  }
  return is_correct;
}

bool Command::checkPlayers(Game* game)
{
  std::transform(parameters_.at(0).begin(), parameters_.at(0).end(), parameters_.at(0).begin(), ::toupper);
  for (auto& player : game->getPlayers())
  {
    if ((*player->getAbbreviation()) == parameters_.at(0))
    {
      return true;
    }
  }
  return false;
}

std::shared_ptr<Player> Command::parsePlayer(Game* game)
{
  for (auto player : game->getPlayers())
  {
    if ((*player->getAbbreviation()) == parameters_.at(0))
    {
      return player;
    }
  }
  return nullptr;
}

std::vector<int> Command::parsePosition() const
{
  std::vector<int> position;
  std::string token;
  std::stringstream ss(parameters_.at(1));
  bool delim_bool = false;
  for (auto& c : parameters_.at(1))
  {
    if (c == ',')
    {
      delim_bool = true;
      break;
    }
  }
  if (!delim_bool)
  {
    return {INVALID, INVALID};
  }

  std::getline(ss, token, ',');
  if (token.at(0) == '-')
  {
    token = "11";
  }
  if (!std::all_of(token.begin(), token.end(), ::isdigit) || token.empty())
  {
    return {INVALID, INVALID};
  }
  position.push_back(std::stoi(token));
  std::getline(ss, token, ' ');
  if (token.at(0) == '-')
  {
    token = "11";
  }
  if (!std::all_of(token.begin(), token.end(), ::isdigit) || token.empty())
  {
    return {INVALID, INVALID};
  }
  position.push_back(std::stoi(token));
  return position;
}

std::vector<int> Command::splitCoordinates(std::string* value)
{

  std::string str_crd = trimBrackets(*value);
  std::stringstream ss(str_crd);
  std::string token;
  std::vector<int> split_coord;

  std::getline(ss, token, ',');
  split_coord.push_back(std::stoi(token));
  std::getline(ss, token, ' ');
  split_coord.push_back(std::stoi(token));

  return split_coord;
}

std::string Command::trimBrackets(const std::string& str)
{
  std::string split_cord = str;
  if (!split_cord.empty() && split_cord.front() == '(')
  {
    split_cord.erase(split_cord.begin());
  }
  if (!split_cord.empty() && split_cord.back() == ')')
  {
    split_cord.pop_back();
  }
  return split_cord;
}

bool Command::paramCheck()
{
  if (type_ == CommandType::HELP || type_ == CommandType::STORY || type_ == CommandType::MAP ||
      type_ == CommandType::POSITIONS)
  {
    if (parameters_.empty())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (type_ == CommandType::PLAYER || type_ == CommandType::INVENTORY)
  {
    if (parameters_.size() == 1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (type_ == CommandType::MOVE || type_ == CommandType::LOOT || type_ == CommandType::USE ||
           type_ == CommandType::ATTACK)
  {
    if (parameters_.size() == 2)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (type_ == CommandType::QUIT)
  {
    if (parameters_.empty())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

void Command::setInput(std::string& input)
{
  input_ = input;
  std::transform(input_.begin(), input_.end(), input_.begin(), ::tolower);
}
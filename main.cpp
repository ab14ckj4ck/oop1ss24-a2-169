//---------------------------------------------------------------------------------------------------------------------
//
// Description: Main file of the project and entry point of the program. It reads the command line arguments and
// validates the input. If the input is valid, it creates a new Game object and runs the game.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------

#include "Game.hpp"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Error: Wrong number of parameters!" << std::endl;
    return static_cast<int>(ReturnValues::WRONG_NUMBER_OF_PARAMETERS);
  }

  std::string dungeon_config_path = argv[1];
  std::string story_config_path = argv[2];

  std::string validation_result = Game::validateConfigs(dungeon_config_path, story_config_path);
  if (validation_result != Game::VALID)
  {
    std::cout << "Error: Invalid file (" << validation_result << ")!" << std::endl;
    return static_cast<int>(ReturnValues::INVALID_FILE_OR_INVALID_MAGIC_NUMBER);
  }

  Game* game;
  try
  {
    game = new Game(&dungeon_config_path, &story_config_path);
    game->runGame();
  }
  catch (std::bad_alloc& e)
  {
    return static_cast<int>(ReturnValues::MEMORY_ALLOCATION_FAILED);
  }


  delete game;
  return static_cast<int>(ReturnValues::SUCCESSFUL);
}
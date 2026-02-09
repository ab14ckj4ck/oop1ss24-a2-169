//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Game class, it's constructor and the methods. The Game
// class is used to managed the games workflow.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#ifndef GAME_HPP
#define GAME_HPP

#include "Ammunition.hpp"
#include "Armor.hpp"
#include "Command.hpp"
#include "Door.hpp"
#include "Field.hpp"
#include "HealthPotion.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "ResistancePotion.hpp"
#include "Room.hpp"
#include "Space.hpp"
#include "Treasure.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class Command;
class Player;

enum class ReturnValues
{
  SUCCESSFUL = 0,
  MEMORY_ALLOCATION_FAILED = 1,
  WRONG_NUMBER_OF_PARAMETERS = 2,
  INVALID_FILE_OR_INVALID_MAGIC_NUMBER = 3,
  NOTHING = 4
};
enum TurnType
{
  PRINT,
  ACTION
};
enum ActionCode
{
  ERROR,
  SUCCESS,
  NEW_ROOM,
  DUNGEON_ESCAPE
};

enum class CommandError
{
  NONE,
  INVALID_COMMAND,
  INVALID_PARAMETERS,
  INVALID_PARAM_COUNT,
  INVALID_SAVEFILE,
  INVALID_ITEM_OR_ENTITY,
  INVALID_DOOR,
  INVALID_POSITION,
  INVALID_NO_WEAPON,
  INVALID_NO_AMMO,
};

class Game
{
private:
  bool print_map_status_;
  bool print_story_status_;
  bool players_flight_;
  bool first_zombie;
  bool first_goblin;
  int completed_rooms_;
  int total_actions_;
  bool players_alive_;
  bool was_command_invalid_;
  Map* map_;
  std::vector<std::shared_ptr<Player>> players_;
  std::map<std::string, std::string> story_texts_;
  static const char STORY_DELIMITER_SYMBOL_ = ':';
  static const char NARRATOR_SYMBOL_ = 'N';
  static const char DUNGEON_DELIMITER_SYMBOL_ = ';';
  static const char DOOR_ = 'D';
  static const char ZOMBIE_ = 'Z';
  static const char GOBLIN_ = 'G';
  static const char LICH_ = 'L';
  static const char TREASURE_ = 'T';

  //---------------------------------------------------------------------------------------------------------------------
  //
  // loads Dungeon config
  //
  // @param path: path of config file
  //
  void loadConfig(std::string* path);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Creates rooms by calling Room ctor
  //
  // @param room_info: holds necessary room information
  // @param first_room: holds the value for the is_visited_ member
  //
  void createRoom(std::string room_info, bool first_room);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Creates the entities in a room
  //
  // @param entity_info: hold the necessary entity information
  //
  void createEntity(std::string entity_info);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Places entity in the room and removes previous field
  //
  // @param row: height position in the room
  // @param column: width position in the room
  // @param entity: the entity to be placed in the room
  //
  void placeEntityInRoom(int row, int column, Field* entity);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Adds a room into another rooms adjacency vector
  //
  // @param ajdacent_room_id: id of the room that is added into current_rooms adjacency vector
  //
  void makeRoomsAdjacent(int ajdacent_room_id);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Creates all the items that a treasure chest holds and stores them in its inventory
  //
  // @param treasure: reference to the treasure object
  // @param item_list: string of items that will be created in this function
  //
  void createItemsForTreasure(Treasure& treasure, std::string& items_list);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Finds the item type
  //
  // @param abbreviation: is the abbreviation that is going to be looked for in the item maps
  // @param amount: how often it appears in the treasure
  //
  // @return a pointer to an item
  //
  static Item* createByItemType(std::string& abbreviation, int amount);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // loads Story Texts
  //
  // @param path: path of story text file
  //
  void loadStoryTexts(std::string* path);

  //---------------------------------------------------------------------------------------------------------------------
  //
  //  reads in the player name
  //
  // @param id: id of the player
  //
  // @return the entered player name
  //
  std::string readPlayerName(int id);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // checks if the player name is already taken
  //
  // @param name: wanted name of the player
  //
  // @return a bool indicating if the player name is taken
  //
  bool isPlayerNameTaken(std::string* name);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // assigns a player type to a player
  //
  // @param name: name of the player
  // @param should_exit: bool to determine if the game should exit
  //
  // @return corresponding Entity type
  //
  EntityType readPlayerType(std::string& name, bool& should_exit);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // checks if the player type is already taken
  //
  // @param type: wanted type of the player
  //
  // @return a bool indicating if the player type is taken
  //
  bool isPlayerTypeTaken(EntityType type);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // checks if the input is "quit"
  //
  // @param input: input to be checked
  //
  // @return a bool indicating if the player wants to quit the game
  //
  bool inputIsQuit(std::string* input);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Creates a vector of fields that are hit by an attack
  //
  // @param player_field : field of the player
  // @param target_field : field of the target
  //
  // @return a vector of Field pointers pointing to target fields of an attack
  //
  std::vector<Field*> calculateTargets(Character* entity, std::vector<int>* target_position, Map* map);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Calc's the length of an input attack by player
  //
  // @param player_field : field of the player
  // @param target_field : field of the target
  //
  // @return the calculated range as int
  //
  int calculateRange(Character* entity, std::vector<int>* target_position);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes burst command
  //
  // @param curr_weapon : weapon to use
  // @param player : player that attacks
  //
  void burst(Character* character,
             Map* map,
             int damage,
             int rolled,
             std::vector<Field*>* vec_target_fields_ptr,
             std::vector<int> target_pos);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes hit command
  //
  // @param curr_weapon : weapon to use
  // @param player : player that attacks
  // @param target : target of the attack
  //
  void hit(Character* character,
           std::vector<int>* target_position,
           Map* map,
           int damage,
           int rolled,
           std::vector<Field*>* vec_target_fields_ptr);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes thrust command
  //
  // @param curr_weapon : weapon to use
  // @param player : player that attacks
  // @param target : target of the attack
  //
  void thrust(Character* character,
              std::vector<int>* target_position,
              Map* map,
              int damage,
              int rolled,
              std::vector<Field*>* vec_target_fields_ptr);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes splash command
  //
  // @param character : the character
  // @param target_position : position of the target
  // @param map : pointer to the map
  // @param damage : calculated damage
  // @param rolled : rolled value
  // @param vec_target_fields_ptr : the target fields
  //
  void slash(Character* character,
             std::vector<int>* target_position,
             Map* map,
             int damage,
             int rolled,
             std::vector<Field*>* vec_target_fields_ptr);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes line command
  //
  // @param character : the character
  // @param target_position : position of the target
  // @param map : pointer to the map
  // @param damage : calculated damage
  // @param rolled : rolled value
  // @param vec_target_fields_ptr : the target fields
  //
  void line(Character* character,
            std::vector<int>* target_position,
            Map* map,
            int damage,
            int rolled,
            std::vector<Field*>* vec_target_fields_ptr);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Executes shot command
  //
  // @param character : the character
  // @param target_position : position of the target
  // @param damage : calculated damage
  // @param rolled : rolled value
  // @param vec_target_fields_ptr : the target fields
  //
  void shot(Character* character,
            std::vector<int>* target_position,
            int damage,
            int rolled,
            std::vector<Field*>* vec_target_fields_ptr);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Decreases health of characters on fields
  //
  // @param damage : damage to be dealt
  // @param rolled : rolled value
  // @param vec_target_fields : fields that are hit
  // @param attacker : the character attacking
  //
  void receiveDamage(int damage, int rolled, std::vector<Field*>* vec_target_fields, Character* attacker);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // moves a player to another field
  //
  // @param entity - character to be moved
  // @param map - current_map
  // @param position - coordinates of new field to be moved to
  //
  void movePlayer(Character* entity, Map map, std::vector<int>* position);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Sets the bools for the fields targeted and affected by an attack accordingly
  //
  // @param std::vector<Field*>* : pointer to a vector containing the fields affected and targeted by an attack
  //
  void adjustAttackFields(std::vector<Field*>* fields);

  //---------------------------------------------------------------------------------------------------------------------
  //
  // Prints info about the character affected by an attack
  //
  // @param Character*: pointer to character affected by the attack
  // @param damage: int value
  //
  void printCharacterAffectedByAttack(int damage, Character* entity);

public:
  //---------------------------------------------------------------------------------------------------------------------
  //
  // Game constructor
  //
  // @param config_path: path to config
  //
  Game(std::string* dungeon_config_path, std::string* story_config_path);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // copy constructor for game
  //
  //
  Game(const Game&) = delete;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // destructor vor game instance
  //
  //
  ~Game();
  //--------------------------------------------------------------------------------------------------------------------
  //
  //  value that indicates that both configs are valid
  //
  // @return a const value
  //
  static const std::string VALID;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // moves an Enemy to another field
  //
  // @param entity - enemy to be moved
  //
  void moveEnemy(Character& entity);
  //---------------------------------------------------------------------------------------------------------------------
  //
  // validates the config files
  //
  // @param path: path of config file
  //
  // @return string indicating if the configs were valid
  //
  static std::string validateConfigs(std::string dungeons_path, std::string stories_path);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Pos0: y-Coordinate
  // Pos1: x-Coordinate
  // finds the nearest player according to the position of the attacking enemy
  //
  // @param enemy - attacking enemy
  //
  // @return position of the nearest player
  //
  std::vector<int> findNearestPlayer(Character& enemy);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // increases the number of turns in current phase
  //
  // @param turn - current turn
  //
  static void finishTurn(int* turn);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // determines the type of the turn
  //
  // @param command_: command that was entered by the player
  //
  // @return corresponding turn type
  //
  TurnType getTurnType(Command* command_);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints the help text
  //
  //
  void printHelp();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // de-/activates the story
  //
  //
  void printStory();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // de-/activates the map
  //
  //
  void printMap();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints all players
  //
  //
  void printAllPlayers();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints all enemies
  //
  //
  void printAllEnemies();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints the current positions of the players and enemies
  //
  //
  void printPositions();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints a single player with all its attributes
  //
  // @param command : command that was entered by the player to determine which player to print
  //
  void printPlayer(Command* command);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // prints inventory of a player
  //
  // @param command : command that was entered by the player to determine which player to print
  //
  void printInventory(Command* command);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // finds coordinates of a field and returns {x, y}
  //
  // @param field : field to be found
  //
  // @return the position of a field
  //
  std::vector<int> findCoordinatesField(Field* field) const;

  //--------------------------------------------------------------------------------------------------------------------
  //
  // moves the player to a new field
  //
  // @param command : command that was entered by the player to determine which player to move
  //
  // @return ActionCode indicating succes or an error
  //
  ActionCode movePlayer(Command* command);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // splits a string into two integers
  //
  // @param position : string that contains the coordinates
  //
  // @return the coordinates in correct form
  //
  std::vector<int> splitCoordinates(std::string position);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // function to loot an entity (chest or dead enemy)
  //
  // @param command : command that was entered by the player to determine which player loots which entity
  //
  // @return ActionCode indicating succes or an error
  //
  ActionCode lootEntity(Command* command);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // function to use an item.
  //
  // @param command : command that was entered by the player to determine which player loots which entity
  //
  // @return ActionCode indicating succes or an error
  //
  ActionCode useItem(Command* command);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // handles the player phase
  //
  // @return return value indicating further way of the game
  //
  ReturnValues executePlayerPhase();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // defines which enemy got his turn
  //
  // @param move_counter - to determine enemy depending on current turn in phase
  //
  // @return a pointer to an enemy which is supposed to do something next in the enemy phase
  //
  Character* findExecutingEnemy(int move_counter);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // handles the enemy phase
  //
  // @return return value indicating the further way of the game
  //
  ReturnValues executeEnemyPhase();
  //---------------------------------------------------------------------------------------------------------------------
  //
  // starts the game
  //
  // @return return value indicating the further way of the game
  //
  ReturnValues runGame();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Decides which attack to use
  //
  // @param target_pos : position of the target
  // @param attacker : pointer to the character attacking
  // @param attacker : pointer to the map
  //
  void attack(std::vector<int>* target_pos, Character* attacker, Map* map);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for map output
  //
  // @param status - new status
  //
  void setMapStatus(bool status);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for map status
  //
  // @return the status of the map
  //
  bool getMapStatus() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // setter for story output
  //
  // @param status - new status
  //
  void setStoryStatus(bool status);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for story status
  //
  // @return status of the stories
  //
  bool getStoryStatus() const;
  //--------------------------------------------------------------------------------------------------------------------
  //
  // finds an player by an abbreviation
  //
  // @param abbreviation - abbreviation to be searched for
  //
  // @return a smart pointer to the desired player
  //
  std::shared_ptr<Player> findPlayer(char abbreviation);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // returns all players in game
  //
  // @return the smart pointers to all the players
  //
  std::vector<std::shared_ptr<Player>> getPlayers();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if players alive
  //
  //
  void checkHealth();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // ckecks if players flew from the dungeon
  //
  //
  void checkFlight();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if the lich died
  //
  // @return a bool indicating if the lich is still alive
  //
  bool checkLich();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if other dependencies for game end are met
  //
  //
  void checkEnd();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // the end phase of the game
  //
  // @return return value indicating the further way of the game
  //
  ReturnValues endPhase();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // outputs the final game stats
  //
  //
  void printStats();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // saves final game stats to a file
  //
  //
  void saveGameToFile();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // getter for map_
  //
  // @return a pointer to the map
  //
  Map* getMap() { return map_; }
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Pos0 = x-Coordinate
  // Pos1 = y-Coordinate
  // searches for a possible movement spot for the character
  //
  // @param random_index - index of the random spot an entity would move to
  // @param entity_pos - position of the entity
  //
  // @return a valid postion of a movement spot
  //
  std::vector<int> findMovementSpot(int random_index, std::vector<int> entity_pos);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // places players around the door of the room they're coming from
  //
  // @param last_room_id - id of the room they are coming from
  //
  void placePlayersAroundDoor(int last_room_id);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // handles the changing of a room
  //
  // @param new_room_id - id of the new room
  //
  void changeRoom(int new_room_id);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // sort's items in the inventory on given (description) criteria
  //
  // @param inventory - inv to be sorted
  //
  void sortInventoryItems(std::vector<Item*>* inventory);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // outputs the attack itselfe
  //
  // @param attacker
  // @param position - pos that got attacked
  // @param affected_fields - fields that got affected by the damage pattern
  // @param rolled - number rolled by the dice
  //
  void printAttack(Character* attacker, std::vector<int>* position, std::vector<Field*>* affected_fields, int rolled);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // determines the current count and upper bound for the dice roll depenting on weapon type and entity type
  //
  // @param count - how many dice rolls
  // @param max - upper bound for dice roll
  // @param curr_weapon - current weapon equipped
  // @param entity - attacking entity
  //
  void findDiceRoll(int& count, int& max, WeaponType curr_weapon, Character* entity);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if the input coordinates are within the bounds of the map
  //
  // @param x - x-Coordinate
  // @param y - y-Coordinate
  // @param map - current map
  //
  // @return a bool indicating if a position is within bounds
  //
  bool isWithinBounds(int x, int y, Map* map);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if the attack type is set correctly depending on the enemy
  //
  // @param entity - entity for check
  //
  void checkWeaponEntityType(Character* entity);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if the player got ammunition to use his range weapons
  //
  // @param attacker - attacking entity
  //
  void checkBowAndCrossbow(Character* attacker);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // checks if there are still enemies alive in the room
  //
  //
  void checkRoom();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // combines all checks for game ending criteria
  //
  //
  void checkAll();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // reads input and creates an command
  //
  // @return a smart pointer to the command
  //
  std::shared_ptr<Command> readInput();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // handles errors in command input
  //
  // @param error_code - to determine the right error output
  //
  void handleInputErrors(CommandError error_code);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // outputs the lootable inventory of entity
  //
  // @param inventory_loot - lootable inventory
  //
  void printInventoryItems(std::vector<Item*>* inventory_loot);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // deletes an equipped item out of the inventory
  //
  // @param inventory - entity's inventory
  // @param item - item to be removed
  //
  void deleteEquippedItem(std::vector<Item*>* inventory, Item* item);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // unequippes current used items (curr_weapon_ & curr_armor_)
  //
  // @param player - entity to be unequipped
  //
  void unequippItem(Character* player);
  //--------------------------------------------------------------------------------------------------------------------
  //
  // outputs story thext for the room
  //
  //
  void printRoomText();
  //--------------------------------------------------------------------------------------------------------------------
  //
  // Pos0 = x-Coordinate
  // Pos1 = y-Coordinate
  // determines the adjacent fields to the given field
  //
  // @param entity_pos - position of the field you want the adjacent fields for
  //
  // @return the adjacent fields to an entity position
  //
  std::vector<std::pair<Field*, std::vector<int>>> findAdjacentFields(std::vector<int>& entity_pos);
};

#endif
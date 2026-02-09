//---------------------------------------------------------------------------------------------------------------------
//
// Description: This file contains the declaration of the Game class, it's constructor and the methods. The Game
// class is used to managed the games workflow.
//
// Group : <169>
//
// Author: Florian Scherer, Fabio Boran, Felix Jopkiewicz
//---------------------------------------------------------------------------------------------------------------------
#include "Game.hpp"

#include <utility>

const std::string Game::VALID = "valid";

Game::Game(std::string* dungeon_config_path, std::string* story_config_path) : print_map_status_(true)
                                                                             , print_story_status_(true)
                                                                             , completed_rooms_(0)
                                                                             , total_actions_(0) {
    map_ = new Map();
    loadConfig(dungeon_config_path);
    loadStoryTexts(story_config_path);
    players_flight_ = false;
    players_alive_ = true;
    first_zombie = true;
    first_goblin = true;
    was_command_invalid_ = false;
    std::cout << story_texts_.at("N_INTRO");
    completed_rooms_ = 0;
}

Game::~Game() {
    delete map_;
}

class CustomError : public std::exception {
public:
    explicit CustomError(std::string message) : message_(std::move(message)) {}

    const char* what() const throw() override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class NoAmmoException : public std::exception {
public:
    explicit NoAmmoException(std::string message) : message_(std::move(message)) {}

    const char* what() const throw() override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class DoorLockedException : public std::exception {
public:
    explicit DoorLockedException(std::string message) : message_(std::move(message)) {}
    const char* what() const throw() override { return message_.c_str(); }

private:
    std::string message_;
};

ReturnValues Game::runGame() {
    int player_count = 0;
    std::cout << "How many players want to join the adventure? (1 to 3)\n> ";
    while(player_count < 1 || player_count > 3) {
        std::string input;
        std::getline(std::cin, input);
        if(std::cin.eof()) {
            return ReturnValues::SUCCESSFUL;
        }
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
        if(input.empty()) {
            std::cout << "Please enter a number of players between 1 and 3.\n> ";
            continue;
        }
        input = input.substr(0, 6);
        if(inputIsQuit(&input)) {
            return ReturnValues::SUCCESSFUL;
        }
        try {
            player_count = std::stoi(input);
        }
        catch(std::invalid_argument& e) {
            player_count = 99;
        }
        if(player_count < 1 || player_count > 3) {
            std::cout << "Please enter a number of players between 1 and 3.\n> ";
        }
    }
    for(int i = 0; i < player_count; ++i) {
        std::string name = readPlayerName(i);
        bool should_exit = false;
        EntityType type = readPlayerType(name, should_exit);
        if(type == EntityType::LICH) {
            return ReturnValues::SUCCESSFUL;
        }
        if(should_exit) {
            return ReturnValues::SUCCESSFUL;
        }
        std::shared_ptr<Player> player;
        std::vector<Item*> items;
        switch(type) {
        case EntityType::BARBARIAN: player = std::make_shared<Player>(12, 1, 4, 2, EntityType::BARBARIAN, &name, i,
                                                                      DamageType::NONE);
            items.push_back(new Weapon("GAXE", 1));
            items.push_back(new Weapon("HAXE", 2));
            for(auto item : items) {
                if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                    if(weapon) {
                        weapon->fillOutWeaponObject();
                    }
                }
                else if(Armor* armor = dynamic_cast<Armor*>(item)) {
                    if(armor) {
                        armor->fillOutArmorObject();
                    }
                }
            }
            player->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(0)));
            player->addToInventory(items);
            deleteEquippedItem(player->getInventory(), dynamic_cast<Weapon*>(items.at(0)));
            players_.push_back(player);
            break;

        case EntityType::WIZARD: player = std::make_shared<Player>(6, 4, 1, 0, EntityType::WIZARD, &name, i,
                                                                   DamageType::NONE);
            items.push_back(new Weapon("QFRC", 1));
            items.push_back(new Weapon("QACD", 1));
            items.push_back(new Weapon("DAGG", 1));
            for(auto item : items) {
                if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                    if(weapon) {
                        weapon->fillOutWeaponObject();
                    }
                }
                else if(Armor* armor = dynamic_cast<Armor*>(item)) {
                    if(armor) {
                        armor->fillOutArmorObject();
                    }
                }
            }
            player->addToInventory(items);
            player->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(0)));
            deleteEquippedItem(player->getInventory(), dynamic_cast<Weapon*>(items.at(0)));
            player->setCurrentArmor(nullptr);
            players_.push_back(player);
            break;

        case EntityType::ROGUE: player = std::make_shared<Player>(8, 3, 2, 1, EntityType::ROGUE, &name, i,
                                                                  DamageType::NONE);
            items.push_back(new Armor("LARM", 1));
            items.push_back(new Weapon("DAGG", 2));
            items.push_back(new Weapon("RAPI", 1));
            items.push_back(new Weapon("SBOW", 1));
            for(auto item : items) {
                if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                    if(weapon) {
                        weapon->fillOutWeaponObject();
                    }
                }
                else if(Armor* armor = dynamic_cast<Armor*>(item)) {
                    if(armor) {
                        armor->fillOutArmorObject();
                    }
                }
            }
            items.push_back(new Ammunition("ARRW", 20));
            player->addToInventory(items);
            player->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(2)));
            deleteEquippedItem(player->getInventory(), dynamic_cast<Weapon*>(items.at(2)));
            player->setCurrentArmor(dynamic_cast<Armor*>(items.at(0)));
            deleteEquippedItem(player->getInventory(), dynamic_cast<Armor*>(items.at(0)));
            players_.push_back(player);
            break;

        default: break;
        }
    }
    int id = 1;
    std::cout << "\n-- Players --------------------------------------" << std::endl;
    for(auto& player : players_) {
        std::cout << "  Player " << id << ": " << Player::getPlayerTypeText(player->getEntityType()) << " ["
            << player->getEntityTypeAbbreviation() << "] \"" << player->getName() << "\"\n";
        id++;
    }
    ReturnValues exit_code = ReturnValues::NOTHING;
    std::cout << std::endl;
    printRoomText();

    placePlayersAroundDoor(0);

    while(exit_code == ReturnValues::NOTHING) {
        exit_code = executePlayerPhase();

        if(exit_code == ReturnValues::NOTHING) {
            exit_code = executeEnemyPhase();
        }
    }
    return exit_code;
}

void Game::printRoomText() {
    std::string room_text = "N_ROOM_" + std::to_string(map_->getCurrentRoom()->getRoomId());
    std::cout << story_texts_.at(room_text);

    bool found_z = false, found_g = false, found_l = false;
    for(auto enemy : *map_->getCurrentRoom()->getEnemiesInRoom()) {
        if(enemy->getEntityType() == EntityType::GOBLIN && !found_g) {
            found_g = true;
        }
        if(enemy->getEntityType() == EntityType::LICH && !found_l) {
            found_l = true;
        }
        if(enemy->getEntityType() == EntityType::ZOMBIE && !found_z) {
            found_z = true;
        }
    }

    if(found_g && first_goblin) {
        std::cout << story_texts_.at("N_ENEMY_G");
        first_goblin = false;
    }
    if(found_l) {
        std::cout << story_texts_.at("N_ENEMY_L");
    }
    if(found_z && first_zombie) {
        std::cout << story_texts_.at("N_ENEMY_Z");
        first_zombie = false;
    }
}

void Game::checkAll() {
    checkEnd();
    checkRoom();
}

void Game::checkRoom() {
    int enemies_in_room = map_->getCurrentRoom()->getEnemiesInRoom()->size();
    int dead_enemies = 0;
    for(auto enemy : *getMap()->getCurrentRoom()->getEnemiesInRoom()) {
        if(!enemy->isAlive()) {
            dead_enemies++;
        }
    }
    if(enemies_in_room == dead_enemies) {
        Room* curr_room = getMap()->getCurrentRoom();
        for(int row = 0; row < curr_room->getHeight(); row++) {
            for(int col = 0; col < curr_room->getWidth(); col++) {
                Door* door = dynamic_cast<Door*>(curr_room->findField(col, row));
                if(door != nullptr) {
                    door->setIsLocked(false);
                }
            }
        }
        if(!curr_room->getCompleted()) {
            completed_rooms_++;
            curr_room->setCompleted(true);
        }
    }
    for(auto& enemy : *map_->getCurrentRoom()->getEnemiesInRoom()) {
        if(enemy->getEntityType() == EntityType::GOBLIN) {
            first_goblin = false;
        }
        if(enemy->getEntityType() == EntityType::ZOMBIE) {
            first_zombie = false;
        }
    }
}

std::string Game::readPlayerName(int id) {
    std::string input = "99999999999";
    std::cout << "\nPlayer " << id + 1 << " what do you wish to be called? (max length 10 characters)\n> ";
    while(input.length() > 10 || isPlayerNameTaken(&input)) {
        std::getline(std::cin, input);
        if(input.length() > 10 || isPlayerNameTaken(&input)) {
            std::cout << "Please enter a unique name with not more than 10 characters.\n> ";
        }
    }
    return input;
}

bool Game::isPlayerNameTaken(std::string* name) {
    for(auto& player : players_) {
        if(*name == player->getName()) {
            return true;
        }
    }
    return false;
}

EntityType Game::readPlayerType(std::string& name, bool& should_exit) {
    std::cout << name << ", please choose a player type:\n  [W] Wizard     "
        << static_cast<int>(isPlayerTypeTaken(EntityType::WIZARD)) << "/1\n  [B] Barbarian  "
        << static_cast<int>(isPlayerTypeTaken(EntityType::BARBARIAN)) << "/1\n  [R] Rogue      "
        << static_cast<int>(isPlayerTypeTaken(EntityType::ROGUE)) << "/1\n> ";
    std::string input;
    while(input.length() != 1) {
        std::getline(std::cin, input);
        if(std::cin.eof()) {
            return EntityType::LICH;
        }
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        if(input.empty()) {
            std::cout << "Please enter a letter representing your desired player type (W, B, or R).\n> ";
            continue;
        }

        if(inputIsQuit(&input)) {
            should_exit = true;
            return EntityType::WIZARD;
        }
        if(!(input == "W" || input == "B" || input == "R" || input == "w" || input == "b" || input == "r")) {
            std::cout << "Please enter a letter representing your desired player type (W, B, or R).\n> ";
            input = "";
            continue;
        }
        EntityType type;
        switch(input.at(0)) {
        case 'W':
        case 'w': type = EntityType::WIZARD;
            break;
        case 'B':
        case 'b': type = EntityType::BARBARIAN;
            break;
        case 'R':
        case 'r': type = EntityType::ROGUE;
            break;
        }
        if(isPlayerTypeTaken(type)) {
            std::cout << "This player type is no longer available. Please choose a different player type.\n> ";
            input = ""; // clear input so loop repeats
            continue;
        }
        return type;
    }
    return EntityType::WIZARD;
}

bool Game::isPlayerTypeTaken(EntityType type) {
    for(auto& player : players_) {
        if(type == player->getEntityType()) {
            return true;
        }
    }
    return false;
}

bool Game::inputIsQuit(std::string* input) {
    std::transform(input->begin(), input->end(), input->begin(), [](unsigned char c) { return std::tolower(c); });
    return (*input == "quit" || input->empty());
}

void Game::loadStoryTexts(std::string* path) {
    std::ifstream file(*path);
    std::string line;
    // skip the first line (magic number)
    std::getline(file, line);
    int delimiter_position = 0;

    while(std::getline(file, line)) {
        std::string story;
        delimiter_position = line.find(STORY_DELIMITER_SYMBOL_);
        std::string key = line.substr(0, delimiter_position);
        char type_abbreviation = key.at(0);

        // remove key from line because it is not needed anymore
        line.erase(0, delimiter_position + 1);

        // build the formatted story text
        if(line.at(0) != ';' && line.at(0) != '\n') {
            if(type_abbreviation == Game::NARRATOR_SYMBOL_) {
                story += "** ";
            }
            else {
                story += "!! ";
            }
        }
        for(int i = 0; i < static_cast<int>(line.length()); i++) {
            if(line.at(i) == ';') {
                story += '\n';
                if(i < static_cast<int>(line.length()) - 1 && line.at(i + 1) != '\n') {
                    if(line.at(i + 1) != ';') {
                        if(type_abbreviation == Game::NARRATOR_SYMBOL_) {
                            story += "** ";
                        }
                        else {
                            story += "!! ";
                        }
                    }
                }
            }
            else {
                story += line.at(i);
            }
        }
        story_texts_.insert(std::make_pair(key, story));
    }
}

std::string Game::validateConfigs(std::string dungeons_path, std::string stories_path) {
    std::ifstream dungeons_file(dungeons_path);
    if(!dungeons_file.is_open()) {
        return dungeons_path;
    }

    std::ifstream stories_file(stories_path);
    if(!stories_file.is_open()) {
        return stories_path;
    }

    std::string line;
    std::getline(dungeons_file, line);
    if(line != "DUNGEON") {
        return dungeons_path;
    }

    std::getline(stories_file, line);
    if(line != "STORY") {
        return stories_path;
    }

    return Game::VALID;
}

void Game::loadConfig(std::string* path) {
    // Need this vector of room info to build the adjacent rooms
    std::vector<std::string> rooms_info;
    std::ifstream file(*path);
    std::string line;
    // skip the first line (magic number)
    std::getline(file, line);
    bool first_room = true;
    int delimiter_position = 0;

    // create the rooms
    while(std::getline(file, line)) {
        delimiter_position = line.find(DUNGEON_DELIMITER_SYMBOL_);
        std::string room_info = line.substr(0, delimiter_position);
        createRoom(room_info, first_room);
        first_room = false;
        line.erase(0, delimiter_position + 1);
        // save rest of the room info regarding its entities in this vector
        rooms_info.push_back(line);
    }
    // manage creating the other entities
    int current_room_id = 1;
    map_->setCurrentRoom(map_->getRoomBasedOnID(current_room_id));
    for(auto& info : rooms_info) {
        while(!info.empty()) {
            delimiter_position = info.find(DUNGEON_DELIMITER_SYMBOL_);
            std::string entity_info = info.substr(0, delimiter_position);
            createEntity(entity_info);
            // erase the part of the string that represents the entity that was just created
            info.erase(0, delimiter_position + 1);
        }
        map_->getCurrentRoom()->sortEnemies();
        current_room_id++;
        if(size_t(current_room_id) <= rooms_info.size()) {
            map_->setCurrentRoom(map_->getRoomBasedOnID(current_room_id));
        }
    }
    map_->setCurrentRoom(map_->getRoomBasedOnID(1));
}

void Game::createEntity(std::string entity_info) {
    int column = std::stoi(std::string(1, entity_info.at(4)));
    int row = std::stoi(std::string(1, entity_info.at(2)));
    char info = entity_info.at(0);
    Space* space;
    std::vector<Item*> items;
    switch(info) {
    case DOOR_: {
        const int id = std::stoi(std::string(1, entity_info.at(6)));
        Door* door = new Door(id, entity_info.at(0));
        placeEntityInRoom(row, column, door);
        if(door->getId() != 0) {
            makeRoomsAdjacent(door->getId());
        }
        break;
    }
    case ZOMBIE_: {
        const int id = std::stoi(std::string(1, entity_info.at(6)));
        Character* zombie = new Enemy(15, 0, 3, 1, EntityType::ZOMBIE, id, DamageType::NONE);
        space = new Space(entity_info.at(0));
        items.push_back(new Weapon("DAGG", 1));
        for(auto item : items) {
            if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                if(weapon) {
                    weapon->fillOutWeaponObject();
                    zombie->getInventory()->push_back(weapon);
                }
            }
        }
        zombie->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(0)));
        space->setCharacter(zombie);
        placeEntityInRoom(row, column, space);
        map_->getCurrentRoom()->getEnemiesInRoom()->push_back(dynamic_cast<Enemy*>(zombie));

        zombie->getLootableInventory()->push_back(new Weapon("DAGG", 1));
        break;
    }
    case GOBLIN_: {
        const int id = std::stoi(std::string(1, entity_info.at(6)));
        Character* goblin = new Enemy(10, 4, 3, 4, EntityType::GOBLIN, id, DamageType::NONE);
        space = new Space(entity_info.at(0));
        items.push_back(new Weapon("HAXE", 1));
        items.push_back(new Weapon("SBOW", 1));
        items.push_back(new Ammunition("ARRW", 300000));
        for(auto item : items) {
            if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                if(weapon) {
                    weapon->fillOutWeaponObject();
                    goblin->getInventory()->push_back(weapon);
                }
            }
            if(Ammunition* ammunition = dynamic_cast<Ammunition*>(item)) {
                if(ammunition) {
                    goblin->getInventory()->push_back(ammunition);
                }
            }
        }
        goblin->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(0)));
        space->setCharacter(goblin);
        placeEntityInRoom(row, column, space);
        map_->getCurrentRoom()->getEnemiesInRoom()->push_back(dynamic_cast<Enemy*>(goblin));

        goblin->getLootableInventory()->push_back(new Weapon("HAXE", 1));
        goblin->getLootableInventory()->push_back(new Weapon("SBOW", 1));
        goblin->getLootableInventory()->push_back(new Ammunition("ARRW", 3));
        break;
    }
    case LICH_: {
        const int id = std::stoi(std::string(1, entity_info.at(6)));
        Character* lich = new Enemy(50, 8, 2, 6, EntityType::LICH, id, DamageType::COLD);
        space = new Space(entity_info.at(0));
        lich->setResistantTo(DamageType::COLD);
        items.push_back(new Weapon("QFIR", 1));
        items.push_back(new Weapon("QCLD", 1));
        items.push_back(new Weapon("QACD", 1));
        items.push_back(new Weapon("QFRC", 1));
        for(auto item : items) {
            if(Weapon* weapon = dynamic_cast<Weapon*>(item)) {
                if(weapon) {
                    weapon->fillOutWeaponObject();
                    lich->getInventory()->push_back(weapon);
                }
            }
        }
        lich->setCurrentWeapon(dynamic_cast<Weapon*>(items.at(0)));
        space->setCharacter(lich);
        placeEntityInRoom(row, column, space);
        map_->getCurrentRoom()->getEnemiesInRoom()->push_back(dynamic_cast<Enemy*>(lich));
        break;
    }
    case TREASURE_: {
        // get value_needed_to_unlock_
        int second_colon_pos = entity_info.find(':', entity_info.find(':') + 1);
        int second_comma_pos = entity_info.find(',', entity_info.find(',') + 1);
        int value_unlock = std::stoi(entity_info.substr(second_colon_pos + 1, second_comma_pos - second_colon_pos - 1));
        Treasure* treasure = new Treasure(entity_info.at(0), value_unlock);
        std::string items_list = entity_info.substr(second_comma_pos + 1);
        createItemsForTreasure(*treasure, items_list);
        placeEntityInRoom(row, column, treasure);
        break;
    }
    }
}

void Game::createItemsForTreasure(Treasure& treasure, std::string& items_list) {
    std::vector<std::string> item_infos;
    std::stringstream ss(items_list);
    std::string element;
    while(std::getline(ss, element, ',')) {
        item_infos.push_back(element);
    }

    int abbreviation_position = 0;
    int amount_position = 1;
    while(size_t(amount_position) <= item_infos.size()) {
        Item* new_item = createByItemType(item_infos.at(abbreviation_position),
                                          std::stoi(item_infos.at(amount_position)));
        treasure + new_item;
        abbreviation_position = abbreviation_position + 2;
        amount_position = amount_position + 2;
    }
}

Item* Game::createByItemType(std::string& abbreviation, int amount) {
    if(Armor::armor_items_.count(abbreviation)) {
        Armor* armor = new Armor(abbreviation, amount);
        armor->fillOutArmorObject();
        return armor;
    }
    if(Weapon::weapon_items_.count(abbreviation)) {
        Weapon* weapon = new Weapon(abbreviation, amount);
        weapon->fillOutWeaponObject();
        return weapon;
    }
    if(Ammunition::ammunition_items_.count(abbreviation)) {
        Ammunition* ammunition = new Ammunition(abbreviation, amount);
        return ammunition;
    }
    if(ResistancePotion::resistance_potion_items_.count(abbreviation)) {
        ResistancePotion* resistance_potion = new ResistancePotion(abbreviation, amount);
        return resistance_potion;
    }
    if(HealthPotion::health_potion_items_.count(abbreviation)) {
        HealthPotion* health_potion = new HealthPotion(abbreviation, amount);
        health_potion->fillOutHealthPotionObject();
        return health_potion;
    }
    return nullptr;
}

void Game::placeEntityInRoom(int row, int column, Field* entity) {
    row--;
    column--;
    delete map_->getCurrentRoom()->getFields()->at(row).at(column);
    map_->getCurrentRoom()->getFields()->at(row).at(column) = entity;
}

void Game::makeRoomsAdjacent(int ajdacent_room_id) {
    Room* adjacent_room = map_->getRoomBasedOnID(ajdacent_room_id);
    map_->getCurrentRoom()->getAdjacentRooms()->at(ajdacent_room_id) = adjacent_room;
}

void Game::createRoom(std::string room_info, bool first_room) {
    map_->getRooms()->push_back(new Room(std::stoi(std::string(1, room_info.at(0))),
                                         std::stoi(std::string(1, room_info.at(2))),
                                         std::stoi(std::string(1, room_info.at(4))),
                                         first_room));
}

void Game::setMapStatus(bool status) {
    print_map_status_ = status;
}

bool Game::getMapStatus() const {
    return print_map_status_;
}

void Game::setStoryStatus(bool status) {
    print_story_status_ = status;
}

bool Game::getStoryStatus() const {
    return print_story_status_;
}

std::vector<std::shared_ptr<Player>> Game::getPlayers() {
    return players_;
}

std::shared_ptr<Player> Game::findPlayer(char abbreviation) {
    abbreviation = toupper(abbreviation);
    for(auto player : getPlayers()) {
        if(player->getAbbreviation()->at(0) == abbreviation) {
            return player;
        }
    }
    return nullptr;
}

void Game::checkEnd() {
    checkHealth();
    checkFlight();
    if(players_alive_ <= 0) {
        endPhase();
    }
}

void Game::checkFlight() {
    if(players_flight_) {
        endPhase();
    }
}

void Game::checkHealth() {
    int death_counter = 0;
    for(auto& player : getPlayers()) {
        if(player->getCurrentHealth() <= 0) {
            death_counter++;
        }
    }
    if(size_t(death_counter) == getPlayers().size()) {
        players_alive_ = false;
    }
}

ReturnValues Game::endPhase() {
    if(!players_alive_) {
        std::cout << story_texts_.at("N_DEFEAT");
    }
    else if(!players_flight_) {
        std::cout << story_texts_.at("N_COMPLETION");
    }
    else {
        std::cout << story_texts_.at("N_FLIGHT");
    }
    printStats();
    std::cout << story_texts_.at("N_SCORING_FILE");
    saveGameToFile();
    return ReturnValues::SUCCESSFUL;
}

bool Game::checkLich() {
    Room* current_room = map_->getCurrentRoom();
    for(int row = 0; row < current_room->getHeight(); row++) {
        for(int col = 0; col < current_room->getWidth(); col++) {
            Space* space = dynamic_cast<Space*>(current_room->findField(col, row));
            if(space != nullptr) {
                if(space->getCharacter() != nullptr) {
                    if(space->getCharacter()->getEntityType() == EntityType::LICH) {
                        if(space->getCharacter()->getCurrentHealth() <= 0 || !space->getCharacter()->isAlive()) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Game::printStats() {
    std::cout << "-- Players --------------------------------------" << std::endl;
    auto players = getPlayers();
    players.erase(std::remove(players.begin(), players.end(), nullptr), players.end());
    std::sort(players.begin(), players.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
        return a->getId() < b->getId();
    });
    std::vector<std::shared_ptr<Player>> alive_players;
    std::vector<std::shared_ptr<Player>> dead_players;
    for(auto& player : players_) {
        if(player->isAlive()) {
            alive_players.push_back(player);
        }
        else {
            dead_players.push_back(player);
        }
    }
    for(auto& player : alive_players) {
        std::cout << "  " << player->getPlayerTypeText(player->getEntityType()) << " [" << player->getAbbreviation()->
                                                                                                   at(0)
            << "] \"" << player->getName() << "\" survived." << std::endl;
    }
    for(auto& player : dead_players) {
        std::cout << "  " << player->getPlayerTypeText(player->getEntityType()) << " [" << player->getAbbreviation()->
                                                                                                   at(0)
            << "] \"" << player->getName() << "\", rest in peace." << std::endl;
    }

    std::cout << "\n-- Statistics -----------------------------------" << std::endl;
    std::cout << "  " << completed_rooms_ << " rooms completed" << std::endl;
    std::cout << "  " << total_actions_ << " performed actions\n" << std::endl;
}

void Game::saveGameToFile() {
    std::string input;
    while(true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if(inputIsQuit(&input)) {
            return;
        }

        std::ofstream save_file(input);
        if(!save_file.is_open()) {
            std::cout << story_texts_.at("E_SCORING_FILE_NOT_WRITABLE");
            std::cout << story_texts_.at("N_SCORING_FILE");
        }
        else {
            std::vector<std::shared_ptr<Player>> alive_players;
            std::vector<std::shared_ptr<Player>> dead_players;
            for(auto& player : players_) {
                if(player->isAlive()) {
                    alive_players.push_back(player);
                }
                else {
                    dead_players.push_back(player);
                }
            }
            save_file << "-- Players --------------------------------------" << std::endl;
            for(auto& player : alive_players) {
                save_file << "  " << player->getPlayerTypeText(player->getEntityType()) << " ["
                    << player->getAbbreviation()->at(0) << "] \"" << player->getName() << "\" survived." << std::endl;
            }
            for(auto& player : dead_players) {
                save_file << "  " << player->getPlayerTypeText(player->getEntityType()) << " ["
                    << player->getAbbreviation()->at(0) << "] \"" << player->getName() << "\", rest in peace."
                    << std::endl;
            }
            save_file << "\n-- Statistics -----------------------------------" << std::endl;
            save_file << "  " << completed_rooms_ << " rooms completed" << std::endl;
            save_file << "  " << total_actions_ << " performed actions\n" << std::endl;
            break;
        }
    }
}

void Game::checkWeaponEntityType(Character* entity) {
    WeaponType curr_weap_type = entity->getCurrentWeapon()->getWeaponType();
    if(entity->getEntityType() == EntityType::WIZARD || entity->getEntityType() == EntityType::LICH) {
        if(curr_weap_type == WeaponType::QUARTERSTAFF_OF_ACID) {
            entity->getCurrentWeapon()->setDamagePattern(DamagePattern::SHOT);
            entity->getCurrentWeapon()->setAttackType(AttackType::RANGED);
            entity->getCurrentWeapon()->setDamageType(DamageType::ACID);
        }
        else if(curr_weap_type == WeaponType::QUARTERSTAFF_OF_COLD) {
            entity->getCurrentWeapon()->setDamagePattern(DamagePattern::LINE);
            entity->getCurrentWeapon()->setAttackType(AttackType::MELEE);
            entity->getCurrentWeapon()->setDamageType(DamageType::COLD);
        }
        else if(curr_weap_type == WeaponType::QUARTERSTAFF_OF_FIRE) {
            entity->getCurrentWeapon()->setDamagePattern(DamagePattern::BURST);
            entity->getCurrentWeapon()->setAttackType(AttackType::RANGED);
            entity->getCurrentWeapon()->setDamageType(DamageType::FIRE);
        }
        else if(curr_weap_type == WeaponType::QUARTERSTAFF_OF_FORCE) {
            entity->getCurrentWeapon()->setDamagePattern(DamagePattern::HIT);
            entity->getCurrentWeapon()->setAttackType(AttackType::MELEE);
            entity->getCurrentWeapon()->setDamageType(DamageType::FORCE);
        }
    }
    else {
        if(curr_weap_type == WeaponType::QUARTERSTAFF_OF_ACID || curr_weap_type == WeaponType::QUARTERSTAFF_OF_COLD ||
            curr_weap_type == WeaponType::QUARTERSTAFF_OF_FIRE || curr_weap_type == WeaponType::QUARTERSTAFF_OF_FORCE) {
            entity->getCurrentWeapon()->setDamagePattern(DamagePattern::HIT);
            entity->getCurrentWeapon()->setAttackType(AttackType::MELEE);
            entity->getCurrentWeapon()->setDamageType(DamageType::PHYSICAL);
        }
    }
}

void Game::checkBowAndCrossbow(Character* attacker) {
    if(dynamic_cast<Enemy*>(attacker)) {
        return;
    }
    Weapon* weapon = attacker->getCurrentWeapon();
    if(weapon->getWeaponType() == WeaponType::SHORTBOW || weapon->getWeaponType() == WeaponType::LONGBOW) {
        if(std::any_of(attacker->getInventory()->begin(), attacker->getInventory()->end(), [](Item* item) {
            if(Ammunition* ammunition = dynamic_cast<Ammunition*>(item)) {
                return ammunition->getAmmunitionType() == AmmunitionType::ARROW;
            }
            return false;
        })) {
            return;
        }
        else {
            throw NoAmmoException(story_texts_.at("E_ATTACK_NO_AMMUNITION"));
        }
    }
    else if(weapon->getWeaponType() == WeaponType::LIGHT_CROSSBOW ||
        weapon->getWeaponType() == WeaponType::HEAVY_CROSSBOW) {
        if(std::any_of(attacker->getInventory()->begin(), attacker->getInventory()->end(), [](Item* item) {
            if(Ammunition* ammunition = dynamic_cast<Ammunition*>(item)) {
                return ammunition->getAmmunitionType() == AmmunitionType::BOLT;
            }
            return false;
        })) {
            return;
        }
        else {
            throw NoAmmoException(story_texts_.at("E_ATTACK_NO_AMMUNITION"));
        }
    }
}

void Game::attack(std::vector<int>* target_pos, Character* attacker, Map* map) {
    int rolled = 0;
    if(attacker->getCurrentWeapon() == nullptr) {
        throw CustomError(story_texts_.at("E_ATTACK_NO_WEAPON_EQUIPPED"));
    }
    checkWeaponEntityType(attacker);
    if(attacker->getCurrentWeapon()->getAttackType() == AttackType::MELEE) {
        std::vector<int> a_pos = {
                map_->getCurrentRoom()->findRow(attacker),
                map_->getCurrentRoom()->findColumn(attacker)
            };
        std::vector<std::pair<Field*, std::vector<int>>> adj_fields = findAdjacentFields(a_pos);
        for(auto& field : adj_fields) {
            // y - x
            std::reverse(field.second.begin(), field.second.end());
        }
        if(!any_of(adj_fields.begin(), adj_fields.end(),
                   [target_pos](const std::pair<Field*, std::vector<int>>& field) {
                       return field.second == *target_pos;
                   })) {
            throw CustomError(story_texts_.at("E_INVALID_POSITION"));
        }
    }
    checkBowAndCrossbow(attacker);
    int damage = attacker->calculateDamage(attacker->getCurrentWeapon(), rolled);
    std::vector<Field*> affected_fields;
    switch(attacker->getCurrentWeapon()->getDamagePattern()) {
    case DamagePattern::HIT: hit(attacker, target_pos, map, damage, rolled, &affected_fields);
        break;
    case DamagePattern::THRUST: thrust(attacker, target_pos, map, damage, rolled, &affected_fields);
        break;
    case DamagePattern::SLASH: slash(attacker, target_pos, map, damage, rolled, &affected_fields);
        break;
    case DamagePattern::LINE: line(attacker, target_pos, map, damage, rolled, &affected_fields);
        break;
    case DamagePattern::SHOT: shot(attacker, target_pos, damage, rolled, &affected_fields);
        break;
    case DamagePattern::BURST: burst(attacker, map, damage, rolled, &affected_fields, *target_pos);
        break;
    }
}

void Game::printCharacterAffectedByAttack(int damage, Character* entity) {
    if(entity->getPreviousHealth() > 0) {
        std::string entity_name;
        bool resistance_modifier = entity->getWasResistant();
        int resistance_value = 0;

        if(resistance_modifier) {
            resistance_value = 50;
        }
        else {
            resistance_value = 100;
        }
        if(entity->getCurrentHealth() < 0) {
            entity->setCurrentHealth(0);
        }
        Player* player = dynamic_cast<Player*>(entity);
        if(player != nullptr) {
            entity_name = player->getName();
            std::cout << entity_name << " loses " << (entity->getPreviousHealth() - entity->getCurrentHealth()) <<
                " health ("
                << damage << " * " << resistance_value << " % - " << entity->getCurrentArmorValue() << ")."
                << std::endl;
            if(player->getCurrentHealth() <= 0) {
                std::cout << entity_name << " was defeated." << std::endl;
                player->setPreviousHealth(0);
            }
        }
        else {
            entity_name = entity->getTypeName();
            std::cout << entity_name << " " << entity->getId() << " loses "
                << (entity->getPreviousHealth() - entity->getCurrentHealth()) << " health (" << damage << " * "
                << resistance_value << " % - " << entity->getArmorValue() << ")." << std::endl;
            if(entity->getCurrentHealth() <= 0) {
                std::cout << entity_name << " " << entity->getId() << " was defeated." << std::endl;
                entity->setPreviousHealth(0);
            }
        }
        entity->setWasResistant(false);
    }
}

int Game::calculateRange(Character* entity, std::vector<int>* target_position) {
    int entity_x = getMap()->getCurrentRoom()->findColumn(entity);
    int entity_y = getMap()->getCurrentRoom()->findRow(entity);
    int x = abs(entity_x - target_position->at(1));
    int y = abs(entity_y - target_position->at(0));
    int range = sqrt(x * x + y * y);
    return range;
}

std::vector<Field*> Game::calculateTargets(Character* entity, std::vector<int>* target_position, Map* map) {
    int entity_x = map->getCurrentRoom()->findColumn(entity);
    int entity_y = map->getCurrentRoom()->findRow(entity);
    int target_x = target_position->at(0);
    int target_y = target_position->at(1);
    std::vector<Field*> vec_target_fields_ptr;
    vec_target_fields_ptr.push_back(map->getCurrentRoom()->findField(target_x, target_y));
    Field* field_ptr;

    if(entity_x == target_x && entity_y != target_y) {
        if(entity_y < target_y) {
            for(int i = entity_y; i < target_y; i++) {
                field_ptr = map->getCurrentRoom()->findField(entity_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_y > target_y) {
            for(int i = entity_y; i > target_y; i--) {
                field_ptr = map->getCurrentRoom()->findField(entity_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
    }
    else if(entity_x != target_x && entity_y == target_y) {
        if(entity_x < target_x) {
            for(int i = entity_x; i < target_x; i++) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_x > target_x) {
            for(int i = entity_x; i > target_x; i--) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
    }
    else if(entity_x != target_x && entity_y != target_y) {
        if(entity_x < target_x && entity_y < target_y) {
            for(int i = entity_x; i < target_x; i++) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
            for(int i = entity_y; i < target_y; i++) {
                field_ptr = map->getCurrentRoom()->findField(target_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_x > target_x && entity_y > target_y) {
            for(int i = entity_x; i > target_x; i--) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
            for(int i = entity_y; i > target_y; i--) {
                field_ptr = map->getCurrentRoom()->findField(target_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_x < target_x && entity_y > target_y) {
            for(int i = entity_x; i < target_x; i++) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
            for(int i = entity_y; i > target_y; i--) {
                field_ptr = map->getCurrentRoom()->findField(target_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_x > target_x && entity_y < target_y) {
            for(int i = entity_x; i > target_x; i--) {
                field_ptr = map->getCurrentRoom()->findField(i, entity_y);
                vec_target_fields_ptr.push_back(field_ptr);
            }
            for(int i = entity_y; i < target_y; i++) {
                field_ptr = map->getCurrentRoom()->findField(target_x, i);
                vec_target_fields_ptr.push_back(field_ptr);
            }
        }
        else if(entity_x != target_x && entity_y != target_y) {
            if(entity_x < target_x && entity_y < target_y) {
                for(int i = 0; i <= target_x - entity_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x + i, entity_y + i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x > target_x && entity_y > target_y) {
                for(int i = 0; i <= entity_x - target_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x - i, entity_y - i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x < target_x && entity_y > target_y) {
                for(int i = 0; i <= target_x - entity_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x + i, entity_y - i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x > target_x && entity_y < target_y) {
                for(int i = 0; i <= entity_x - target_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x - i, entity_y + i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
        }
        else if(entity_x != target_x && entity_y != target_y) {
            if(entity_x < target_x && entity_y < target_y) {
                for(int i = 0; i <= target_x - entity_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x + i, entity_y + i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x > target_x && entity_y > target_y) {
                for(int i = 0; i <= entity_x - target_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x - i, entity_y - i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x < target_x && entity_y > target_y) {
                for(int i = 0; i <= target_x - entity_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x + i, entity_y - i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
            else if(entity_x > target_x && entity_y < target_y) {
                for(int i = 0; i <= entity_x - target_x; i++) {
                    field_ptr = map->getCurrentRoom()->findField(entity_x - i, entity_y + i);
                    vec_target_fields_ptr.push_back(field_ptr);
                }
            }
        }
    }
    adjustAttackFields(&vec_target_fields_ptr);
    return vec_target_fields_ptr;
}

void Game::hit(Character* character,
               std::vector<int>* target_position,
               Map* map,
               int damage,
               int rolled,
               std::vector<Field*>* vec_target_fields_ptr) {
    int x = map->getCurrentRoom()->findColumn(character);
    int y = map->getCurrentRoom()->findRow(character);
    bool is_x_adjacent = x - target_position->at(1) <= 1 && x - target_position->at(1) >= -1;
    bool is_y_adjacent = y - target_position->at(0) <= 1 && y - target_position->at(0) >= -1;
    if(is_x_adjacent && is_y_adjacent) {
        vec_target_fields_ptr->push_back(
            map->getCurrentRoom()->findField(target_position->at(1), target_position->at(0)));

        receiveDamage(damage, rolled, vec_target_fields_ptr, character);
    }
}

void Game::thrust(Character* character,
                  std::vector<int>* target_position,
                  Map* map,
                  int damage,
                  int rolled,
                  std::vector<Field*>* vec_target_fields_ptr) {
    int thrust_range = 2;
    int character_x = map->getCurrentRoom()->findColumn(character);
    int character_y = map->getCurrentRoom()->findRow(character);
    int target_x = target_position->at(1);
    int target_y = target_position->at(0);

    int dx = (target_x - character_x) ? (target_x - character_x) / abs(target_x - character_x) : 0;
    int dy = (target_y - character_y) ? (target_y - character_y) / abs(target_y - character_y) : 0;

    if(dx == 0 && dy == 0) {
        throw CustomError(story_texts_.at("E_INVALID_POSITION"));
    }

    for(int i = 1; i <= thrust_range; ++i) {
        int nx = character_x + i * dx;
        int ny = character_y + i * dy;
        if(isWithinBounds(nx, ny, map)) {
            vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(nx, ny));
        }
    }
    receiveDamage(damage, rolled, vec_target_fields_ptr, character);
}

void Game::printAttack(Character* attacker,
                       std::vector<int>* position,
                       std::vector<Field*>* affected_fields,
                       int rolled) {
    Player* player = dynamic_cast<Player*>(attacker);
    if(player != nullptr) {
        std::cout << player->getTypeName() << " [" << *player->getAbbreviation() << "] \"" << player->getName()
            << "\" used \"" << player->getCurrentWeapon()->getName() << "\" on (" << position->at(0) + 1 << ","
            << position->at(1) + 1 << ") affecting";
    }
    Enemy* enemy = dynamic_cast<Enemy*>(attacker);
    if(enemy != nullptr) {
        std::cout << std::endl;
        std::cout << enemy->getTypeName() << " " << enemy->getId() << " [" << *enemy->getAbbreviation() << enemy->
            getId()
            << "] used \"" << enemy->getCurrentWeapon()->getName() << "\" on (" << position->at(0) + 1 << ","
            << position->at(1) + 1 << ") affecting";
    }
    size_t i = 0;
    for(int row = 0; row < map_->getCurrentRoom()->getHeight(); row++) {
        for(int col = 0; col < map_->getCurrentRoom()->getWidth(); col++) {
            auto field = map_->getCurrentRoom()->findField(col, row);
            if(std::find(affected_fields->begin(), affected_fields->end(), field) != affected_fields->end()) {
                std::cout << " (" << row + 1 << "," << col + 1 << ")";
                if(i < affected_fields->size() - 1) {
                    std::cout << ",";
                }
                else {
                    std::cout << ".";
                }
                i++;
            }
        }
    }

    int count = 0;
    int max = 0;

    findDiceRoll(count, max, attacker->getCurrentWeapon()->getWeaponType(), attacker);
    std::cout << "\n[Dice Roll] " << count << " d" << max << " resulting in a total value of " << rolled << ".\n"
        << std::endl;
}

void Game::findDiceRoll(int& count, int& max, WeaponType curr_weapon, Character* entity) {
    switch(curr_weapon) {
    case WeaponType::MACE: count = 1;
        max = 6;
        break;
    case WeaponType::WARHAMMER: count = 1;
        max = 10;
        break;
    case WeaponType::HANDAXE: count = 1;
        max = 6;
        break;
    case WeaponType::GREATAXE: count = 1;
        max = 12;
        break;
    case WeaponType::LONGSWORD: count = 1;
        max = 10;
        break;
    case WeaponType::GREATSWORD: count = 2;
        max = 6;
        break;
    /* Vitality */
    case WeaponType::DAGGER: count = 1;
        max = 4;
        break;
    case WeaponType::RAPIER: count = 1;
        max = 8;
        break;
    case WeaponType::SHORTSWORD:
    case WeaponType::SHORTBOW: count = 1;
        max = 6;
        break;
    case WeaponType::LONGBOW:
    case WeaponType::LIGHT_CROSSBOW: count = 1;
        max = 8;
        break;
    case WeaponType::HEAVY_CROSSBOW: count = 1;
        max = 10;
        break;

    /* Staff's */
    case WeaponType::QUARTERSTAFF_OF_FIRE: if(entity->getEntityType() == EntityType::WIZARD || entity->getEntityType()
            == EntityType::LICH) {
            count = 3;
            max = 6;
            break;
        }
        else {
            count = 1;
            max = 6;
            break;
        }
    case WeaponType::QUARTERSTAFF_OF_COLD: if(entity->getEntityType() == EntityType::WIZARD || entity->getEntityType()
            == EntityType::LICH) {
            count = 2;
            max = 10;
            break;
        }
        else {
            count = 1;
            max = 6;
            break;
        }
    case WeaponType::QUARTERSTAFF_OF_ACID:
    case WeaponType::QUARTERSTAFF_OF_FORCE: if(entity->getEntityType() == EntityType::WIZARD || entity->getEntityType()
            == EntityType::LICH) {
            count = 1;
            max = 10;
            break;
        }
        else {
            count = 1;
            max = 6;
            break;
        }
    }
}

bool Game::isWithinBounds(int x, int y, Map* map) {
    if(x >= 0 && x < map->getCurrentRoom()->getWidth() && y >= 0 && y < map->getCurrentRoom()->getHeight()) {
        return true;
    }
    return false;
}

void Game::slash(Character* character,
                 std::vector<int>* target_position,
                 Map* map,
                 int damage,
                 int rolled,
                 std::vector<Field*>* vec_target_fields_ptr) {
    int hitrange = 1;
    int entity_x = map->getCurrentRoom()->findColumn(character);
    int entity_y = map->getCurrentRoom()->findRow(character);
    int y = (*target_position).at(0);
    int x = (*target_position).at(1);
    if(isWithinBounds(x, y, map) && calculateRange(character, target_position) <= hitrange) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(x, y));
    }

    if(calculateRange(character, target_position) <= hitrange) {
        if(x == entity_x && y != entity_y) {
            if(isWithinBounds(x - 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x - 1, y));
            if(isWithinBounds(x + 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x + 1, y));
        }

        else if(y == entity_y && x != entity_x) {
            if(isWithinBounds(x, y - 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y - 1));
            if(isWithinBounds(x, y + 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y + 1));
        }

        else if(x < entity_x && y < entity_y) {
            if(isWithinBounds(x + 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x + 1, y));
            if(isWithinBounds(x, y + 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y + 1));
        }

        else if(x > entity_x && y > entity_y) {
            if(isWithinBounds(x - 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x - 1, y));
            if(isWithinBounds(x, y - 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y - 1));
        }

        else if(x < entity_x && y > entity_y) {
            if(isWithinBounds(x + 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x + 1, y));
            if(isWithinBounds(x, y - 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y - 1));
        }

        else if(x > entity_x && y < entity_y) {
            if(isWithinBounds(x - 1, y, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x - 1, y));
            if(isWithinBounds(x, y + 1, map)) vec_target_fields_ptr->push_back(
                map->getCurrentRoom()->findField(x, y + 1));
        }

        receiveDamage(damage, rolled, vec_target_fields_ptr, character);
    }
}

void Game::line(Character* character,
                std::vector<int>* target_position,
                Map* map,
                int damage,
                int rolled,
                std::vector<Field*>* vec_target_fields_ptr) {
    int character_x = map->getCurrentRoom()->findColumn(character);
    int character_y = map->getCurrentRoom()->findRow(character);
    int target_y = target_position->at(0);
    int target_x = target_position->at(1);

    int dx = (target_x - character_x) ? (target_x - character_x) / abs(target_x - character_x) : 0;
    int dy = (target_y - character_y) ? (target_y - character_y) / abs(target_y - character_y) : 0;

    if(dx == 0 && dy == 0) {
        throw CustomError(story_texts_.at("E_INVALID_POSITION"));
    }

    int nx = character_x + dx;
    int ny = character_y + dy;

    while(isWithinBounds(nx, ny, map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(nx, ny));
        nx += dx;
        ny += dy;
    }

    receiveDamage(damage, rolled, vec_target_fields_ptr, character);
}

void Game::shot(Character* character,
                std::vector<int>* target_position,
                int damage,
                int rolled,
                std::vector<Field*>* vec_target_fields_ptr) {
    vec_target_fields_ptr->push_back(map_->getCurrentRoom()->findField(target_position->at(1), target_position->at(0)));
    receiveDamage(damage, rolled, vec_target_fields_ptr, character);
}

void Game::burst(Character* character,
                 Map* map,
                 int damage,
                 int rolled,
                 std::vector<Field*>* vec_target_fields_ptr,
                 std::vector<int> target_pos) {
    if(isWithinBounds(target_pos.at(1), target_pos.at(0), map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(target_pos[1], target_pos[0]));
    }
    if(isWithinBounds(target_pos.at(1) - 1, target_pos.at(0), map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(target_pos[1] - 1, target_pos[0]));
    }
    if(isWithinBounds(target_pos.at(1) + 1, target_pos.at(0), map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(target_pos[1] + 1, target_pos[0]));
    }
    if(isWithinBounds(target_pos.at(1), target_pos.at(0) - 1, map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(target_pos[1], target_pos[0] - 1));
    }
    if(isWithinBounds(target_pos.at(1), target_pos.at(0) + 1, map)) {
        vec_target_fields_ptr->push_back(map->getCurrentRoom()->findField(target_pos[1], target_pos[0] + 1));
    }

    receiveDamage(damage, rolled, vec_target_fields_ptr, character);
}

void Game::moveEnemy(Character& entity) {
    Room* curr_room = map_->getCurrentRoom();
    std::vector<int> entity_pos;
    entity_pos.push_back(curr_room->findRow(&entity));
    entity_pos.push_back(curr_room->findColumn(&entity));
    std::vector<std::pair<Field*, std::vector<int>>> adjacent_fields = findAdjacentFields(entity_pos);
    int random_move = Oop::Random::getInstance().getRandomNumber(adjacent_fields.size());

    // y - x
    std::vector<int> new_pos;
    Enemy* enemy;
    Space* space = dynamic_cast<Space*>(adjacent_fields.at(random_move - 1).first);
    if(space != nullptr && space->getCharacter() == nullptr) {
        Field* old_field = curr_room->findField(entity_pos.at(1), entity_pos.at(0));
        Space* space_old = dynamic_cast<Space*>(old_field);
        space_old->setCharacter(nullptr);
        space->setCharacter(&entity);
        new_pos = adjacent_fields.at(random_move - 1).second;
        enemy = dynamic_cast<Enemy*>(&entity);
        std::cout << std::endl;
    }
    else {
        std::reverse(adjacent_fields.at(random_move - 1).second.begin(),
                     adjacent_fields.at(random_move - 1).second.end());
        std::vector<std::pair<Field*, std::vector<int>>> adj_fields_target =
            findAdjacentFields(adjacent_fields.at(random_move - 1).second);
        size_t space_num = 0;
        while(true) {
            space = dynamic_cast<Space*>(adj_fields_target.at(space_num).first);
            if(space != nullptr) {
                if(space->getCharacter() == nullptr || space->getCharacter() == &entity) {
                    Field* old_field = curr_room->findField(entity_pos.at(1), entity_pos.at(0));
                    Space* space_old = dynamic_cast<Space*>(old_field);
                    space_old->setCharacter(nullptr);
                    space->setCharacter(&entity);
                    new_pos = adj_fields_target.at(space_num).second;
                    enemy = dynamic_cast<Enemy*>(&entity);
                    std::cout << std::endl;
                    break;
                }
            }
            space_num++;
            if(space_num == adj_fields_target.size()) {
                enemy = dynamic_cast<Enemy*>(&entity);
                std::reverse(entity_pos.begin(), entity_pos.end());
                new_pos = entity_pos;
                break;
            }
        }
    }
    std::cout << entity.getTypeName() << " " << enemy->getId() << " [" << *enemy->getAbbreviation() << entity.getId()
        << "] "
        << "moved to (" << new_pos.at(1) + 1 << "," << new_pos.at(0) + 1 << ")." << std::endl;
}

// entity_pos = y - x
std::vector<std::pair<Field*, std::vector<int>>> Game::findAdjacentFields(std::vector<int>& entity_pos) {
    std::vector<std::pair<Field*, std::vector<int>>> adjacent_fields;
    std::vector<int> new_pos;
    int field_counter = 1;
    while(true) {
        // new_pos = x - y
        new_pos = findMovementSpot(field_counter, entity_pos);
        if(isWithinBounds(new_pos.at(0), new_pos.at(1), map_)) {
            Field* new_field = map_->getCurrentRoom()->findField(new_pos.at(0), new_pos.at(1));
            std::pair<Field*, std::vector<int>> temp_pair = std::make_pair(new_field, new_pos);
            adjacent_fields.push_back(temp_pair);
        }
        if(field_counter == 8) {
            return adjacent_fields;
        }
        field_counter++;
    }
}

// entity_pos = y - x
std::vector<int> Game::findMovementSpot(int random_index, std::vector<int> entity_pos) {
    std::vector<int> new_pos;
    switch(random_index) {
    case 1: new_pos = {entity_pos[1], entity_pos[0] - 1};
        break;
    case 2: new_pos = {entity_pos[1] + 1, entity_pos[0] - 1};
        break;
    case 3: new_pos = {entity_pos[1] + 1, entity_pos[0]};
        break;
    case 4: new_pos = {entity_pos[1] + 1, entity_pos[0] + 1};
        break;
    case 5: new_pos = {entity_pos[1], entity_pos[0] + 1};
        break;
    case 6: new_pos = {entity_pos[1] - 1, entity_pos[0] + 1};
        break;
    case 7: new_pos = {entity_pos[1] - 1, entity_pos[0]};
        break;
    case 8: new_pos = {entity_pos[1] - 1, entity_pos[0] - 1};
        break;
    }
    // new_pos = x - y
    return new_pos;
}

struct FieldWithCoordinates {
    Field* field;
    int x;
    int y;
};

void Game::receiveDamage(int damage, int rolled, std::vector<Field*>* vec_target_fields, Character* attacker) {
    std::vector<int> target_pos = findCoordinatesField(vec_target_fields->at(0));
    std::reverse(target_pos.begin(), target_pos.end());
    printAttack(attacker, &target_pos, vec_target_fields, rolled);
    std::vector<FieldWithCoordinates> fields_with_coordinates;
    for(int row = 0; row < map_->getCurrentRoom()->getHeight(); row++) {
        for(int col = 0; col < map_->getCurrentRoom()->getWidth(); col++) {
            Field* field = map_->getCurrentRoom()->findField(col, row);
            if(std::find(vec_target_fields->begin(), vec_target_fields->end(), field) != vec_target_fields->end()) {
                fields_with_coordinates.push_back({field, col, row});
            }
        }
    }
    std::sort(fields_with_coordinates.begin(),
              fields_with_coordinates.end(),
              [](FieldWithCoordinates& a, FieldWithCoordinates& b) {
                  if(a.y == b.y) {
                      return a.x < b.x;
                  }
                  else {
                      return a.y < b.y;
                  }
              });

    vec_target_fields->clear();
    for(auto field : fields_with_coordinates) {
        vec_target_fields->push_back(field.field);
    }

    for(auto field : *vec_target_fields) {
        int temp_damage = damage;
        if(attacker->getCurrentWeapon()->getWeaponType() == WeaponType::SHORTBOW ||
            attacker->getCurrentWeapon()->getWeaponType() == WeaponType::LONGBOW) {
            attacker->discardUsed(AmmunitionType::ARROW);
        }
        else if(attacker->getCurrentWeapon()->getWeaponType() == WeaponType::LIGHT_CROSSBOW ||
            attacker->getCurrentWeapon()->getWeaponType() == WeaponType::HEAVY_CROSSBOW) {
            attacker->discardUsed(AmmunitionType::BOLT);
        }
        Space* space = dynamic_cast<Space*>(field);
        if(dynamic_cast<Space*>(field) != nullptr && space->getCharacter() != nullptr && space->getCharacter()->
                                                                                                isAlive()) {
            space->getCharacter()->setPreviousHealth(space->getCharacter()->getCurrentHealth());
            if(space->getCharacter()->getResistantTo() == attacker->getCurrentWeapon()->getDamageType()) {
                space->getCharacter()->setWasResistant(true);
            }
            if(attacker->getCurrentWeapon()->getDamageType() == space->getCharacter()->getResistantTo()) {
                temp_damage = temp_damage / 2;
            }
            temp_damage = temp_damage - space->getCharacter()->getCurrentArmorValue();
            if(temp_damage < 0) {
                temp_damage = 0;
            }
            space->getCharacter()->setCurrentHealth(space->getCharacter()->getCurrentHealth() - temp_damage);
            printCharacterAffectedByAttack(damage, space->getCharacter());
            if(space->getCharacter()->getCurrentHealth() <= 0) {
                space->getCharacter()->setIsAlive(false);
                unequippItem(space->getCharacter());
                sortInventoryItems(space->getCharacter()->getInventory());
                space->getCharacter()->setCurrentHealth(0);
                checkHealth();
                if(!players_alive_) {
                    return;
                }
                if(!checkLich()) {
                    return;
                }
            }
        }
    }
}

void Game::unequippItem(Character* entity) {
    if(entity->getCurrentWeapon() != nullptr) {
        entity->getInventory()->push_back(entity->getCurrentWeapon());
        entity->setCurrentWeapon(nullptr);
    }
    if(entity->getCurrentArmor() != nullptr) {
        entity->getInventory()->push_back(entity->getCurrentArmor());
        entity->setCurrentArmor(nullptr);
    }
}

std::vector<int> Game::findNearestPlayer(Character& enemy) {
    int nearest_distance = 999999;
    Room* curr_room = map_->getCurrentRoom();
    std::vector<Character*> possible_targets;
    Character* nearest_player = players_.at(0).get();
    int enemy_row = curr_room->findRow(&enemy);
    int enemy_col = curr_room->findColumn(&enemy);

    for(auto& player : players_) {
        if(!player->isAlive()) {
            continue;
        }
        int player_col = curr_room->findColumn(player.get());
        int player_row = curr_room->findRow(player.get());

        int distance = abs(player_row - enemy_row) + abs(player_col - enemy_col);

        if(distance < nearest_distance ||
            (distance == nearest_distance && player->getCurrentHealth() < nearest_player->getCurrentHealth())) {
            nearest_distance = distance;
            nearest_player = player.get();
            possible_targets.clear();
            possible_targets.push_back(player.get());
        }
        else if(distance == nearest_distance && player->getCurrentHealth() == nearest_player->getCurrentHealth()) {
            possible_targets.push_back(player.get());
        }
    }
    if(possible_targets.size() == 1) {
        std::vector<int> pos = {
                curr_room->findColumn(possible_targets.at(0)),
                curr_room->findRow(possible_targets.at(0))
            };
        return pos;
    }
    for(int i = 0; i <= nearest_distance; i++) // top right
    {
        for(auto possible_target : possible_targets) {
            if(enemy_row + i - nearest_distance == curr_room->findRow(possible_target) &&
                enemy_col + i == curr_room->findColumn(possible_target)) {
                std::vector<int> pos = {curr_room->findColumn(possible_target), curr_room->findRow(possible_target)};
                return pos;
            }
        }
    }
    for(int i = 0; i <= nearest_distance; i++) // bottom right
    {
        for(auto possible_target : possible_targets) {
            if(enemy_row + i == curr_room->findRow(possible_target) &&
                enemy_col + nearest_distance - i == curr_room->findColumn(possible_target)) {
                std::vector<int> pos = {curr_room->findColumn(possible_target), curr_room->findRow(possible_target)};
                return pos;
            }
        }
    }
    for(int i = 0; i <= nearest_distance; i++) // bottom left
    {
        for(auto possible_target : possible_targets) {
            if(enemy_row + nearest_distance - i == curr_room->findRow(possible_target) &&
                enemy_col - i == curr_room->findColumn(possible_target)) {
                std::vector<int> pos = {curr_room->findColumn(possible_target), curr_room->findRow(possible_target)};
                return pos;
            }
        }
    }
    for(int i = 0; i <= nearest_distance; i++) // top left
    {
        for(auto possible_target : possible_targets) {
            if(enemy_row - i == curr_room->findRow(possible_target) &&
                enemy_col - nearest_distance + i == curr_room->findColumn(possible_target)) {
                std::vector<int> pos = {curr_room->findColumn(possible_target), curr_room->findRow(possible_target)};
                return pos;
            }
        }
    }
    return {INVALID, INVALID};
}

void Game::adjustAttackFields(std::vector<Field*>* fields) {
    std::vector<Field*> concerned_fields = *fields;
    concerned_fields.at(0)->setIsAttacksTargetField(true);
    for(size_t i = 1; i < fields->size(); ++i) {
        fields->at(i)->setIsAttacksAffectedField(true);
    }
}

void Game::finishTurn(int* turn) {
    (*turn)++;
}

TurnType Game::getTurnType(Command* command_) {
    if(command_->getType() == CommandType::HELP || command_->getType() == CommandType::MAP ||
        command_->getType() == CommandType::POSITIONS || command_->getType() == CommandType::PLAYER ||
        command_->getType() == CommandType::INVENTORY || command_->getType() == CommandType::STORY) {
        return PRINT;
    }
    else {
        return ACTION;
    }
}

void Game::printHelp() {
    std::cout << "-- Commands ----------------------------------------" << std::endl;
    std::cout << "- help" << std::endl;
    std::cout << "     Prints this help text.\n" << std::endl;
    std::cout << "- quit" << std::endl;
    std::cout << "    Terminates the game.\n" << std::endl;
    std::cout << "- story" << std::endl;
    std::cout << "    Activates or deactivates the stories of the rooms.\n" << std::endl;
    std::cout << "- map" << std::endl;
    std::cout << "    Activates or deactivates the map.\n" << std::endl;
    std::cout << "- positions" << std::endl;
    std::cout << "    Prints the current positions of the players and enemies.\n" << std::endl;
    std::cout << "- player <PLAYER_TYPE_ABBREVIATION>" << std::endl;
    std::cout << "    Prints player information of the specific player.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        << std::endl;
    std::cout << "- inventory <PLAYER_TYPE_ABBREVIATION>" << std::endl;
    std::cout << "    Prints the inventory of the specific player.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        << std::endl;
    std::cout << "* move <PLAYER_TYPE_ABBREVIATION> <POSITION>" << std::endl;
    std::cout << "    Lets the player move to an adjacent field.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
        << std::endl;
    std::cout << "* loot <PLAYER_TYPE_ABBREVIATION> <POSITION>" << std::endl;
    std::cout << "    Lets the player loot an adjacent chest.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
        << std::endl;
    std::cout << "* use <PLAYER_TYPE_ABBREVIATION> <ITEM_ABBREVIATION>" << std::endl;
    std::cout << "    Lets the player use a potion or equip armor or weapons.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        "    <ITEM_ABBREVIATION>: abbreviation of an item in the players inventory\n"
        << std::endl;
    std::cout << "* attack <PLAYER_TYPE_ABBREVIATION> <POSITION>" << std::endl;
    std::cout << "    Lets the player attack with the equipped weapon on the given position.\n"
        "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
        "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
        << std::endl;
    std::cout << "-- Legend ------------------------------------------\n"
        "- display commands\n* action commands (count as actions)\n"
        "----------------------------------------------------"
        << std::endl;
}

void Game::printStory() {
    if(getStoryStatus()) {
        setStoryStatus(false);
    }
    else {
        setStoryStatus(true);
    }
}

void Game::printMap() {
    if(getMapStatus()) {
        setMapStatus(false);
    }
    else {
        setMapStatus(true);
        map_->drawCurrentRoom();
    }
}

void Game::printAllPlayers() {
    for(auto player : getPlayers()) {
        if(player->isAlive()) {
            std::cout << player->getTypeName() << " [" << *player->getAbbreviation() << "] \"" << player->getName()
                << "\" on (" << getMap()->getCurrentRoom()->findRow(player.get()) + 1 << ","
                << getMap()->getCurrentRoom()->findColumn(player.get()) + 1 << ")" << std::endl;
        }
    }
}

void Game::printAllEnemies() {
    Room* curr_room = map_->getCurrentRoom();
    for(auto enemy : *curr_room->getEnemiesInRoom()) {
        std::cout << enemy->getTypeName() << " " << enemy->getId() << " [" << *enemy->getAbbreviation() << enemy->
            getId()
            << "] on (" << curr_room->findRow(enemy) + 1 << "," << curr_room->findColumn(enemy) + 1 << ")";
        std::cout << std::endl;
    }
}

void Game::printPositions() {
    printAllPlayers();
    printAllEnemies();
}

void Game::printPlayer(Command* command) {
    Room* curr_room = getMap()->getCurrentRoom();
    std::vector<std::string> parameters = command->getParameters();
    std::shared_ptr<Player> player_entity = findPlayer(parameters[0].at(0));
    std::shared_ptr<Player> player = player_entity;
    const int value_width = 5;
    int armor_value;
    if(player_entity->getCurrentArmor() != nullptr) {
        armor_value = player_entity->getCurrentArmorValue();
    }
    else {
        armor_value = player_entity->getArmorValue();
    }

    int max_label_width = std::max({10, 14, 10, 8, 8});

    std::cout << player->getTypeName() << " [" << *player->getAbbreviation() << "] \"" << player_entity->getName()
        << "\" on (" << curr_room->findRow(player.get()) + 1 << "," << curr_room->findColumn(player.get()) + 1
        << ")\n";

    std::cout << std::right << "  Armor Value: " << std::setw(max_label_width + value_width - 11) << armor_value << "\n"
        << "  Current Health: " << std::setw(max_label_width + value_width - 14) << player->getCurrentHealth()
        << "\n"
        << "  Max Health: " << std::setw(max_label_width + value_width - 10) << player->getMaxHealth() << "\n"
        << "  Strength: " << std::setw(max_label_width + value_width - 8) << player->getStrength() << "\n"
        << "  Vitality: " << std::setw(max_label_width + value_width - 8) << player->getVitality() << std::endl;
}

void Game::sortInventoryItems(std::vector<Item*>* inventory) {
    for(auto it = inventory->begin(); it != inventory->end(); ++it) {
        auto next = it + 1;
        while(next != inventory->end()) {
            if((*it)->getAbbreviation() == (*next)->getAbbreviation()) {
                (*it)->setAmount((*it)->getAmount() + (*next)->getAmount());
                next = inventory->erase(next);
            }
            else {
                ++next;
            }
        }
    }
    std::sort(inventory->begin(), inventory->end(), [](const Item* a, const Item* b) {
        return a->getAbbreviation() < b->getAbbreviation();
    });
}

void Game::printInventory(Command* command) {
    std::vector<std::string> parameters = command->getParameters();
    std::shared_ptr<Player> player = findPlayer(parameters[0].at(0));
    if(player == nullptr) {
        return;
    }
    std::vector<Item*>* inventory = player->getInventory();
    sortInventoryItems(player->getInventory());
    std::cout << "Inventory \"" << player->getName() << "\"" << std::endl;
    std::string armor_abbr = "-";
    std::string armor_name = "None";
    std::string weapon_abbr = "-";
    std::string weapon_name = "None";

    if(player->getCurrentArmor() != nullptr) {
        armor_abbr = player->getCurrentArmor()->getAbbreviation();
        armor_name = player->getCurrentArmor()->getName();
    }
    if(player->getCurrentWeapon() != nullptr) {
        weapon_abbr = player->getCurrentWeapon()->getAbbreviation();
        weapon_name = player->getCurrentWeapon()->getName();
    }

    std::cout << "  Equipped Armor: [" << armor_abbr << "] " << armor_name << std::endl;
    std::cout << "  Equipped Weapon: [" << weapon_abbr << "] " << weapon_name << std::endl;
    printInventoryItems(inventory);
}

std::vector<int> Game::findCoordinatesField(Field* field) const {
    for(int row = 0; row < map_->getCurrentRoom()->getHeight(); row++) {
        for(int col = 0; col < map_->getCurrentRoom()->getWidth(); col++) {
            if(map_->getCurrentRoom()->findField(col, row) == field) {
                return {col, row};
            }
        }
    }
    return {INVALID, INVALID};
}

ActionCode Game::movePlayer(Command* command) {
    Room* curr_room = getMap()->getCurrentRoom();
    std::vector<std::string> parameters = command->getParameters();
    std::shared_ptr<Player> player = findPlayer(parameters.at(0).at(0));
    std::vector<int> coordinates = Command::splitCoordinates(&parameters[1]);
    coordinates[0] -= 1;
    coordinates[1] -= 1;

    Field* old_field = curr_room->findField(curr_room->findColumn(player.get()), curr_room->findRow(player.get()));
    Field* new_field = curr_room->findField(coordinates[1], coordinates[0]);

    std::vector<int> old_field_pos = findCoordinatesField(old_field);
    std::vector<int> new_field_pos = findCoordinatesField(new_field);

    if((std::max(abs(old_field_pos[0] - new_field_pos[0]), abs(old_field_pos[1] - new_field_pos[1])) > 1) ||
        !isWithinBounds(new_field_pos[0], new_field_pos[1], map_)) {
        throw CustomError(story_texts_["E_INVALID_POSITION"]);
    }
    if(dynamic_cast<Treasure*>(new_field) != nullptr) {
        throw CustomError(story_texts_["E_INVALID_POSITION"]);
    }
    Space* new_space = dynamic_cast<Space*>(new_field);
    if(new_space != nullptr) {
        Space* old_space = dynamic_cast<Space*>(old_field);
        if(new_space == nullptr || new_space->getCharacter() != nullptr) {
            throw CustomError(story_texts_["E_INVALID_POSITION"]);
        }

        old_space->setCharacter(nullptr);
        new_space->setCharacter(player.get());
        std::cout << player->getTypeName() << " [" << *player->getAbbreviation() << "]"
            << " \"" << player->getName() << "\" moved to (" << parameters[1] << ")." << std::endl;
        return ActionCode::SUCCESS;
    }
    Door* door = dynamic_cast<Door*>(new_field);
    if(door != nullptr) {
        if(door->getIsLocked()) {
            throw DoorLockedException(story_texts_["E_MOVE_LOCKED_DOOR"]);
            return ActionCode::ERROR;
        }
        else {
            std::cout << player->getTypeName() << " [" << *player->getAbbreviation() << "]"
                << " \"" << player->getName() << "\" moved to (" << parameters[1] << ")." << std::endl;
            if(door->getId() == 0) {
                players_flight_ = true;
                return ActionCode::DUNGEON_ESCAPE;
            }
            changeRoom(door->getId());
            return ActionCode::NEW_ROOM;
        }
    }
    else {
        return ActionCode::ERROR;
    }
    return ActionCode::ERROR;
}

std::vector<int> Game::splitCoordinates(std::string position) {
    std::vector<int> coordinates;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while((pos = position.find(delimiter)) != std::string::npos) {
        token = position.substr(0, pos);
        coordinates.push_back(stoi(token));
        position.erase(0, pos + delimiter.length());
    }
    coordinates.push_back(stoi(position));
    return coordinates;
}

ActionCode Game::lootEntity(Command* command) {
    std::vector<std::string> parameters = command->getParameters();
    std::shared_ptr<Player> player = findPlayer(parameters[0].at(0));
    std::vector<int> coordinates = splitCoordinates(parameters[1]);
    coordinates[0] -= 1;
    coordinates[1] -= 1;
    Field* field = getMap()->getCurrentRoom()->findField(coordinates[1], coordinates[0]);
    if(dynamic_cast<Door*>(field)) {
        throw CustomError(story_texts_["E_INVALID_POSITION"]);
    }
    Treasure* treasure = dynamic_cast<Treasure*>(field);
    if(treasure != nullptr) {
        bool key_value_chest = treasure->tryToOpenChest(player->getVitality());
        if(key_value_chest) {
            std::cout << "** Looting successful! You have received the following items ..." << std::endl;
            if(treasure->getInventory()->empty() || treasure->getInventory()->at(0) == nullptr) {
                treasure = nullptr;
                Space* space = new Space(' ');
                placeEntityInRoom(coordinates[0] + 1, coordinates[1] + 1, space);
                return ActionCode::SUCCESS;
            }
            printInventoryItems(treasure->getInventory());
            player->addLootedInventory(treasure->getInventory());
            sortInventoryItems(player->getInventory());
            treasure = nullptr;
            Space* space = new Space(' ');
            placeEntityInRoom(coordinates[0] + 1, coordinates[1] + 1, space);
        }
        return ActionCode::SUCCESS;
    }
    Space* space = dynamic_cast<Space*>(field);
    if(space != nullptr) {
        if(space->getCharacter() == nullptr || space->getCharacter() == player.get() || space->getCharacter()->
                                                                                               isAlive()) {
            throw CustomError(story_texts_["E_INVALID_POSITION"]);
        }
        std::cout << "** Looting successful! You have received the following items ..." << std::endl;
        if(space->getCharacter() != nullptr && !space->getCharacter()->isAlive()) {
            std::vector<Item*>* loot_inventory = nullptr;
            if(space->getCharacter()->getEntityType() == EntityType::ZOMBIE ||
                space->getCharacter()->getEntityType() == EntityType::GOBLIN ||
                space->getCharacter()->getEntityType() == EntityType::LICH) {
                loot_inventory = space->getCharacter()->getLootableInventory();
            }
            else {
                loot_inventory = space->getCharacter()->getInventory();
            }
            if(loot_inventory != nullptr) {
                printInventoryItems(loot_inventory);
                player->addLootedInventory(loot_inventory);
                auto it = std::remove(map_->getCurrentRoom()->getEnemiesInRoom()->begin(),
                                      map_->getCurrentRoom()->getEnemiesInRoom()->end(),
                                      space->getCharacter());
                map_->getCurrentRoom()->getEnemiesInRoom()->
                      erase(it, map_->getCurrentRoom()->getEnemiesInRoom()->end());
                sortInventoryItems(player->getInventory());
            }
            else {
                return ActionCode::SUCCESS;
            }
            Enemy* e = dynamic_cast<Enemy*>(space->getCharacter());
            if(e) {
                delete space->getCharacter();
            }
            space->setCharacter(nullptr);
            return ActionCode::SUCCESS;
        }
        else if(space->getCharacter() == nullptr) {
            return ActionCode::SUCCESS;
        }
        else {
            throw CustomError(story_texts_["E_INVALID_POSITION"]);
        }
    }
    return ActionCode::ERROR;
}

void Game::printInventoryItems(std::vector<Item*>* loot_inventory) {
    std::string armor_output, weapon_output, consum_output;
    sortInventoryItems(loot_inventory);
    for(auto item : *loot_inventory) {
        switch(item->getItemType()) {
        case ItemType::ARMOR: {
            Armor* armor = dynamic_cast<Armor*>(item);
            if(armor != nullptr) {
                armor_output += "[" + armor->getAbbreviation() + "] " + armor->getName() + " (" +
                    std::to_string(armor->getAmount()) + "), ";
            }
            break;
        }
        case ItemType::WEAPON: {
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            if(weapon != nullptr) {
                weapon_output += "[" + weapon->getAbbreviation() + "] " + weapon->getName() + " (" +
                    std::to_string(weapon->getAmount()) + "), ";
            }
            break;
        }
        case ItemType::CONSUMABLE: {
            Consumable* consumable = dynamic_cast<Consumable*>(item);
            if(consumable != nullptr) {
                consum_output += "[" + consumable->getAbbreviation() + "] " + consumable->getName() + " (" +
                    std::to_string(consumable->getAmount()) + "), ";
            }
            break;
        }
        }
    }

    if(!armor_output.empty()) {
        std::cout << "  Armor: " << armor_output.substr(0, armor_output.length() - 2) << std::endl;
    }
    if(!weapon_output.empty()) {
        std::cout << "  Weapons: " << weapon_output.substr(0, weapon_output.length() - 2) << std::endl;
    }
    if(!consum_output.empty()) {
        std::cout << "  Consumables: " << consum_output.substr(0, consum_output.length() - 2) << std::endl;
    }
}

ActionCode Game::useItem(Command* command) {
    std::vector<std::string> parameters = command->getParameters();
    std::shared_ptr<Player> player = findPlayer(parameters[0].at(0));
    if(player->getCurrentWeapon() != nullptr &&
        player->getCurrentWeapon()->getAbbreviation() == command->getParameters()[1]) {
        player->getInventory()->push_back(player->getCurrentWeapon());
        player->setCurrentWeapon(nullptr);
        sortInventoryItems(player->getInventory());
        return ActionCode::SUCCESS;
    }
    else if(player->getCurrentArmor() != nullptr &&
        player->getCurrentArmor()->getAbbreviation() == command->getParameters()[1]) {
        player->getInventory()->push_back(player->getCurrentArmor());
        player->setCurrentArmor(nullptr);
        sortInventoryItems(player->getInventory());
        return ActionCode::SUCCESS;
    }

    std::string item_abbr = command->getParameters()[1];

    auto real_item_it = std::find_if(Item::abbr_to_name.begin(),
                                     Item::abbr_to_name.end(),
                                     [&](std::pair<std::string, std::string> i) { return i.first == item_abbr; });
    if(real_item_it == Item::abbr_to_name.end()) {
        throw CustomError(story_texts_["E_INVALID_PARAM"]);
    }
    auto it = std::find_if(player->getInventory()->begin(), player->getInventory()->end(), [&](Item* i) {
        return i->getAbbreviation() == item_abbr;
    });
    if(it == player->getInventory()->end()) {
        throw CustomError(story_texts_["E_ENTITY_OR_ITEM_UNAVAILABLE"]);
    }
    Item* item = *it;
    if(item->getItemType() == ItemType::CONSUMABLE) {
        Consumable* consumable = dynamic_cast<Consumable*>(item);
        Potion* potion = dynamic_cast<Potion*>(consumable);
        if(potion != nullptr) {
            player->usePotion(potion);
            return ActionCode::SUCCESS;
        }
        Ammunition* ammunition = dynamic_cast<Ammunition*>(consumable);
        if(ammunition != nullptr) {
            throw CustomError(story_texts_["E_INVALID_PARAM"]);
        }
        return ActionCode::SUCCESS;
    }
    else if(item->getItemType() == ItemType::ARMOR) {
        Armor* armor = dynamic_cast<Armor*>(item);
        if(player->getCurrentArmor() != nullptr) {
            if(std::find(player->getInventory()->begin(), player->getInventory()->end(), player->getCurrentArmor()) !=
                player->getInventory()->end()) {
                player->getCurrentArmor()->setAmount(player->getCurrentArmor()->getAmount() + 1);
            }
            else {
                player->getInventory()->push_back(player->getCurrentArmor());
            }
        }
        player->setCurrentArmor(nullptr);
        player->setCurrentArmor(armor);
        deleteEquippedItem(player->getInventory(), armor);

        return ActionCode::SUCCESS;
    }
    else if(item->getItemType() == ItemType::WEAPON) {
        Weapon* weapon = dynamic_cast<Weapon*>(item);
        if(player->getCurrentWeapon() != nullptr) {
            if(std::find(player->getInventory()->begin(), player->getInventory()->end(), player->getCurrentWeapon()) !=
                player->getInventory()->end()) {
                player->getCurrentWeapon()->setAmount(player->getCurrentWeapon()->getAmount() + 1);
            }
            else {
                player->getInventory()->push_back(player->getCurrentWeapon());
            }
        }
        player->setCurrentWeapon(nullptr);
        player->setCurrentWeapon(weapon);
        deleteEquippedItem(player->getInventory(), weapon);
        return ActionCode::SUCCESS;
    }
    return ActionCode::ERROR;
}

void Game::deleteEquippedItem(std::vector<Item*>* inventory, Item* item) {
    auto it = std::find(inventory->begin(), inventory->end(), item);

    if(it != inventory->end()) {
        if((*it)->getAmount() > 1) {
            (*it)->setAmount((*it)->getAmount() - 1);
        }
        else {
            inventory->erase(it);
        }
    }
}

void Game::handleInputErrors(CommandError error_code) {
    switch(error_code) {
    case CommandError::NONE: std::cout << story_texts_["N_PROMPT_MESSAGE"];
        break;
    case CommandError::INVALID_PARAM_COUNT: std::cout << story_texts_["E_INVALID_PARAM_COUNT"];
        break;
    case CommandError::INVALID_PARAMETERS: std::cout << story_texts_["E_INVALID_PARAM"];
        break;
    case CommandError::INVALID_COMMAND: std::cout << story_texts_["E_UNKNOWN_COMMAND"];
        break;
    case CommandError::INVALID_ITEM_OR_ENTITY: std::cout << story_texts_["E_ENTITY_OR_ITEM_UNAVAILABLE"];
        break;
    case CommandError::INVALID_POSITION: std::cout << story_texts_["E_INVALID_POSITION"];
        break;
    case CommandError::INVALID_DOOR: std::cout << story_texts_["E_MOVE_LOCKED_DOOR"];
        break;
    case CommandError::INVALID_NO_WEAPON: std::cout << story_texts_["E_ATTACK_NO_WEAPON_EQUIPPED"];
        break;
    case CommandError::INVALID_NO_AMMO: std::cout << story_texts_["E_ATTACK_NO_AMMUNITION"];
        break;
    case CommandError::INVALID_SAVEFILE: std::cout << story_texts_["E_SCORING_FILE_NOT_WRITEABLE"];
        break;
    }
}

std::shared_ptr<Command> Game::readInput() {
    CommandError error_code = CommandError::NONE;
    while(true) {
        if(!was_command_invalid_) {
            handleInputErrors(error_code);
        }
        was_command_invalid_ = false;
        std::cout << "> ";
        std::string input;
        getline(std::cin, input);
        if(std::cin.eof()) {
            return nullptr;
        }
        std::string input_copy = input;
        input_copy.erase(std::remove_if(input_copy.begin(), input_copy.end(), ::isspace), input_copy.end());
        if(input_copy.empty()) {
            error_code = CommandError::INVALID_COMMAND;
            continue;
        }
        std::shared_ptr<Command> command = std::make_shared<Command>(input);
        if(!command->validate()) {
            error_code = CommandError::INVALID_COMMAND;
            continue;
        }
        if(!command->paramCheck()) {
            error_code = CommandError::INVALID_PARAM_COUNT;
            continue;
        }
        if(command->getType() == CommandType::PLAYER || command->getType() == CommandType::INVENTORY ||
            command->getType() == CommandType::MOVE || command->getType() == CommandType::LOOT ||
            command->getType() == CommandType::USE || command->getType() == CommandType::ATTACK) {
            if(!std::isalpha(command->getParameters().at(0).at(0)) || command->getParameters().at(0).size() != 1) {
                error_code = CommandError::INVALID_PARAMETERS;
                continue;
            }
            std::vector<int> position;
            if(command->getType() == CommandType::MOVE || command->getType() == CommandType::LOOT ||
                command->getType() == CommandType::ATTACK) {
                try {
                    position = command->parsePosition();
                    if(position.at(0) == INVALID || position.at(1) == INVALID) {
                        error_code = CommandError::INVALID_PARAMETERS;
                        continue;
                    }
                }
                catch(std::invalid_argument&) {
                    error_code = CommandError::INVALID_PARAMETERS;
                    continue;
                }
                catch(std::out_of_range&) {
                    error_code = CommandError::INVALID_PARAMETERS;
                    continue;
                }
            }
            if(command->getParameters().at(0).at(0) != 'B' && command->getParameters().at(0).at(0) != 'W' &&
                command->getParameters().at(0).at(0) != 'R') {
                error_code = CommandError::INVALID_PARAMETERS;
                continue;
            }
            if(command->getType() == CommandType::USE) {
                std::map<std::string, std::string> item_map_cut = Item::abbr_to_name;
                item_map_cut.erase("ARRW");
                item_map_cut.erase("BOLT");
                if(!std::any_of(item_map_cut.begin(), item_map_cut.end(),
                                [&](const std::pair<std::string, std::string>& i) {
                                    return i.first == command->getParameters().at(1);
                                })) {
                    error_code = CommandError::INVALID_PARAMETERS;
                    continue;
                }
            }
            if(!command->checkPlayers(this)) {
                error_code = CommandError::INVALID_ITEM_OR_ENTITY;
                continue;
            }
            if(!findPlayer(command->getParameters().at(0)[0])->isAlive()) {
                error_code = CommandError::INVALID_ITEM_OR_ENTITY;
                continue;
            }
            if(command->getType() == CommandType::LOOT || command->getType() == CommandType::ATTACK ||
                command->getType() == CommandType::MOVE) {
                if(position.at(0) <= 0 || position.at(0) > 9 || position.at(1) <= 0 || position.at(1) > 9 ||
                    !isWithinBounds(position.at(1) - 1, position.at(0) - 1, map_)) {
                    error_code = CommandError::INVALID_POSITION;
                    continue;
                }
            }
        }
        return command;
    }
    return nullptr;
}

void Game::placePlayersAroundDoor(int last_room_id) {
    int door_x;
    int door_y;
    int placeable_player = std::count_if(
        players_.begin(), players_.end(), [](std::shared_ptr<Player>& player) { return player->isAlive(); });
    std::vector<std::shared_ptr<Player>> players = getPlayers();
    players.erase(std::remove_if(players.begin(),
                                 players.end(),
                                 [](const std::shared_ptr<Player>& player) { return !player->isAlive(); }),
                  players.end());
    for(int row = 0; row < map_->getCurrentRoom()->getHeight(); row++) {
        for(int col = 0; col < map_->getCurrentRoom()->getWidth(); col++) {
            Field* field = map_->getCurrentRoom()->findField(col, row);
            Door* door = dynamic_cast<Door*>(field);
            if(door != nullptr) {
                if(door->getId() == last_room_id) {
                    door_x = col;
                    door_y = row;
                }
            }
        }
    }
    int placed_player = 0;
    int distance = 1;
    while(placed_player < placeable_player) {
        for(int y = door_y - distance; y <= door_y; y++) {
            for(int x = door_x; x <= door_x + distance; x++) {
                if(isWithinBounds(x, y, map_)) {
                    Field* field = map_->getCurrentRoom()->findField(x, y);
                    Space* space = dynamic_cast<Space*>(field);
                    if(space != nullptr && space->getCharacter() == nullptr) {
                        space->setCharacter(players.at(placed_player).get());
                        placed_player++;
                        if(placed_player == placeable_player) {
                            return;
                        }
                    }
                }
            }
        }
        for(int y = door_y + 1; y <= door_y + distance; y++) {
            for(int x = door_x + distance; x >= door_x; x--) {
                if(isWithinBounds(x, y, map_)) {
                    Field* field = map_->getCurrentRoom()->findField(x, y);
                    Space* space = dynamic_cast<Space*>(field);
                    if(space != nullptr && space->getCharacter() == nullptr) {
                        space->setCharacter(players.at(placed_player).get());
                        placed_player++;
                        if(placed_player == placeable_player) {
                            return;
                        }
                    }
                }
            }
        }
        for(int y = door_y + distance; y >= door_y; y--) {
            for(int x = door_x - 1; x >= door_x - distance; x--) {
                if(isWithinBounds(x, y, map_)) {
                    Field* field = map_->getCurrentRoom()->findField(x, y);
                    Space* space = dynamic_cast<Space*>(field);
                    if(space != nullptr && space->getCharacter() == nullptr) {
                        space->setCharacter(players.at(placed_player).get());
                        placed_player++;
                        if(placed_player == placeable_player) {
                            return;
                        }
                    }
                }
            }
        }
        for(int y = door_y; y >= door_y - distance; y--) {
            for(int x = door_x - distance; x <= door_x - 1; x++) {
                if(isWithinBounds(x, y, map_)) {
                    Field* field = map_->getCurrentRoom()->findField(x, y);
                    Space* space = dynamic_cast<Space*>(field);
                    if(space != nullptr && space->getCharacter() == nullptr) {
                        space->setCharacter(players.at(placed_player).get());
                        placed_player++;
                        if(placed_player == placeable_player) {
                            return;
                        }
                    }
                }
            }
        }
        distance++;
    }
}

ReturnValues Game::executePlayerPhase() {
    TurnType last_turn_type = ACTION;
    int curr_turn = 0;
    int max_turns = getPlayers().size();
    while(curr_turn < max_turns && checkLich()) {
        checkRoom();
        checkHealth();
        if(!checkLich() || !players_alive_ || players_flight_) {
            endPhase();
            return ReturnValues::SUCCESSFUL;
        }
        if(last_turn_type == ACTION) {
            std::cout << "\n-- ROOM " << map_->getCurrentRoom()->getRoomId() << " (" << completed_rooms_ << "/"
                << map_->getRooms()->size() << " completed) --------------------\n\n";
        }
        if(print_map_status_ && last_turn_type == ACTION) {
            map_->drawCurrentRoom();
        }
        if(last_turn_type == ACTION) {
            map_->getCurrentRoom()->printEnemies();
        }

        std::shared_ptr<Command> command = readInput();
        if(command == nullptr) {
            return ReturnValues::SUCCESSFUL;
        }
        if(command->getType() == CommandType::QUIT) {
            return ReturnValues::SUCCESSFUL;
        }
        last_turn_type = getTurnType(command.get());
        if(last_turn_type == PRINT) {
            switch(command->getType()) {
            case CommandType::HELP: printHelp();
                break;
            case CommandType::MAP: printMap();
                break;
            case CommandType::POSITIONS: printPositions();
                break;
            case CommandType::PLAYER: printPlayer(command.get());
                break;
            case CommandType::INVENTORY: printInventory(command.get());
                break;
            case CommandType::STORY: printStory();
                break;
            default: break;
            }
            continue;
        }

        else if(last_turn_type == ACTION) {
            CommandType type = command->getType();
            std::vector<int> pos = command->parsePosition();
            std::shared_ptr<Player> player = command->parsePlayer(this);
            switch(type) {
            case CommandType::MOVE: {
                ActionCode move_return;
                try {
                    total_actions_++;
                    move_return = movePlayer(command.get());
                }
                catch(CustomError& e) {
                    was_command_invalid_ = true;
                    std::cout << e.what();
                    total_actions_--;
                    last_turn_type = PRINT;
                    continue;
                }
                catch(DoorLockedException& e) {
                    was_command_invalid_ = true;
                    std::cout << e.what();
                    total_actions_--;
                    last_turn_type = PRINT;
                    continue;
                }
                if(move_return == ActionCode::NEW_ROOM) {
                    curr_turn = 0;
                    continue;
                }
                else if(move_return == ActionCode::ERROR) {
                    continue;
                }
                else if(move_return == ActionCode::SUCCESS) {
                    break;
                }
                else if(move_return == ActionCode::DUNGEON_ESCAPE) {
                    endPhase();
                    return ReturnValues::SUCCESSFUL;
                }
                break;
            }
            case CommandType::LOOT: try {
                    lootEntity(command.get());
                }
                catch(CustomError& e) {
                    was_command_invalid_ = true;
                    std::cout << e.what();
                    last_turn_type = TurnType::PRINT;
                    continue;
                }
                total_actions_++;
                break;
            case CommandType::USE: try {
                    useItem(command.get());
                }
                catch(CustomError& e) {
                    std::cout << e.what();
                    was_command_invalid_ = true;
                    last_turn_type = TurnType::PRINT;
                    continue;
                }
                total_actions_++;
                break;
            case CommandType::ATTACK: try {
                    pos[0] -= 1;
                    pos[1] -= 1;
                    attack(&pos, player.get(), map_);
                }
                catch(const CustomError& e) {
                    was_command_invalid_ = true;
                    std::cout << e.what();
                    last_turn_type = TurnType::PRINT;
                    continue;
                }
                catch(const NoAmmoException& e) {
                    was_command_invalid_ = true;
                    std::cout << e.what();
                    last_turn_type = TurnType::PRINT;
                    continue;
                }
                total_actions_++;
                break;
            default: continue;
            }
            finishTurn(&curr_turn);
        }
        checkRoom();
        checkHealth();
    }
    if(!checkLich() || !players_alive_ || players_flight_) {
        endPhase();
        return ReturnValues::SUCCESSFUL;
    }
    return ReturnValues::NOTHING;
}

Character* Game::findExecutingEnemy(int move_counter) {
    std::vector<Enemy*>* entities = getMap()->getCurrentRoom()->getEnemiesInRoom();
    std::sort(entities->begin(), entities->end(), [](const Character* a, const Character* b) {
        if(a->getEntityType() != b->getEntityType()) {
            return a->getEntityType() < b->getEntityType();
        }
        return a->getId() < b->getId();
    });
    entities->erase(
        std::remove_if(entities->begin(), entities->end(), [](Character* entity) { return !entity->isAlive(); }),
        entities->end());
    return (*entities)[move_counter];
}

ReturnValues Game::executeEnemyPhase() {
    int move_counter = 0;
    int enemies_alive = 0;
    for(auto enemy : *map_->getCurrentRoom()->getEnemiesInRoom()) {
        if(enemy->isAlive()) {
            enemies_alive++;
        }
    }
    if(getMap()->getCurrentRoom()->getEnemiesInRoom()->empty()) {
        return ReturnValues::NOTHING;
    }
    while(move_counter < enemies_alive) {
        if(move_counter == 0) {
            std::cout << "\n-- ROOM " << map_->getCurrentRoom()->getRoomId() << " (" << completed_rooms_ << "/"
                << map_->getRooms()->size() << " completed) --------------------\n\n";
        }
        if(print_map_status_ && move_counter == 0) {
            getMap()->drawCurrentRoom();
        }
        if(move_counter == 0) {
            getMap()->getCurrentRoom()->printEnemies();
        }
        Character* entity = findExecutingEnemy(move_counter);
        if(entity == nullptr) {
            return ReturnValues::NOTHING;
        }
        Enemy* entity_enemy = dynamic_cast<Enemy*>(entity);
        int players_alive = 0;
        for(auto& player : getPlayers()) {
            if(player->isAlive()) {
                players_alive++;
            }
        }
        std::vector<int> nearest_player;
        if(players_alive > 0) {
            nearest_player = findNearestPlayer(*entity);
        }
        else {
            return ReturnValues::NOTHING;
        }
        std::vector<int> enemy_pos = {
                map_->getCurrentRoom()->findColumn(entity_enemy),
                map_->getCurrentRoom()->findRow(entity_enemy)
            };
        entity_enemy->setCurrentWeapon(nullptr);
        entity_enemy->chooseWeapon(&enemy_pos, &nearest_player);
        if(entity_enemy->getCurrentWeapon() == nullptr) {
            moveEnemy(*entity_enemy);
        }
        else {
            if(nearest_player.at(0) == INVALID || nearest_player.at(1) == INVALID) {
                return ReturnValues::MEMORY_ALLOCATION_FAILED;
            }
            // Y _ X
            std::vector<int> target_pos = {nearest_player.at(1), nearest_player.at(0)};
            attack(&target_pos, entity_enemy, map_);
        }

        move_counter++;
        checkRoom();
        checkHealth();
        if(!players_alive_ || !checkLich()) {
            endPhase();
            return ReturnValues::SUCCESSFUL;
        }
    }
    return ReturnValues::NOTHING;
}

void Game::changeRoom(int target_room_id) {
    for(auto& player : players_) {
        player->setResistantTo(DamageType::NONE);
    }
    if(target_room_id == 0) {
        return;
    }
    Room* target_room = map_->getRoomBasedOnID(target_room_id);
    if(target_room == nullptr) {
        return;
    }
    for(int row = 0; row < map_->getCurrentRoom()->getHeight(); row++) {
        for(int col = 0; col < map_->getCurrentRoom()->getWidth(); col++) {
            Field* field = map_->getCurrentRoom()->findField(col, row);
            Space* space = dynamic_cast<Space*>(field);
            if(space != nullptr && space->getCharacter() != nullptr && space->getCharacter()->isAlive()) {
                space->setCharacter(nullptr);
            }
        }
    }
    map_->setPreviousRoom(map_->getCurrentRoom());
    map_->setCurrentRoom(target_room);
    if(getStoryStatus() && !target_room->getIsVisited()) {
        printRoomText();
    }
    map_->getCurrentRoom()->setIsVisited(true);
    placePlayersAroundDoor(map_->getPreviousRoom()->getRoomId());
}

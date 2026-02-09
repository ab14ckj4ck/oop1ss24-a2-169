# Milestone 1
The **goal** of Milestone 1 (M1) is to implement a **basic structure** for the game and create **some of the output** to the console.

This includes creating basic classes in an object-oriented structure based on your [design document](Design_Document.md).

## Table of Contents
_General information about the game:_
* [Definitions](#definitions)
* [Important Rules](#important-rules)
* [Item Overview](Items.md) (file describing the different items)
* [Character Overview](Characters.md) (file describing the different characters)
* [Placeholder List](Placeholder_List.md) (file listing all placeholders used in the description)

_The game consists of the following parts:_
1. **Initialisation of the Game**
   1. [Config files](#config-files)
   2. [Starting the Program](#starting-the-program)
   3. [Parsing the Story Config](#parsing-the-story-config)
   4. [Parsing the Dungeon Config](#parsing-the-dungeon-config)
   5. [Start Message](#start-message)
   6. [Initialising the Players](#initialising-the-players)
2. **Game Logic**
   1. [Player Phase](#player-phase)
   2. [Printing the Current Room](#printing-the-current-room)
   3. [Basic Command Handling](#basic-command-handling)
   4. Command Handling and Commands Overview _(see [Milestone 2](Milestone_2.md))_
   5. Display Commands _(see [Milestone 2](Milestone_2.md))_
   6. Action Commands _(see [Milestone 2](Milestone_2.md))_
   7. Enemy Phase _(see [Milestone 2](Milestone_2.md))_
3. **Final Phase**
   1. End of Game _(see [Milestone 2](Milestone_2.md))_
   2. Scoring _(see [Milestone 2](Milestone_2.md))_
   3. Save Scoring to File _(see [Milestone 2](Milestone_2.md))_


<details>
<summary><h2> Definitions</h2></summary>

The following definitions are important for the whole game description.

<details>
<summary><h3> Dungeon, Rooms, Fields and Doors </h3></summary>

A dungeon consists of multiple rooms which are connected by doors. Each dungeon has a single entrance/exit door.
The players start the game in the first room (room with id `1`, the one the entrance/exit door is in) and move through the dungeon to discover new rooms, fight enemies, etc.

A room consists of fields, which are arranged in a grid. A room can have multiple doors.

Doors between rooms may be locked.

As long as the door they want to pass is unlocked the players can choose freely which door they want to 
go through next. This means they are not bound to always move forward. They can for example decide to go to the previous room,
or even go to the entrance/exit door, to end the game early.

</details>
<details>
<summary><h3> Completed Rooms</h3></summary>

A room counts as **completed** as soon as there are no enemies left in it.

</details>
<details>
<summary><h3> Current Room</h3></summary>

During the game there is always exactly one room that is active at the moment. All (not defeated) player characters will always
be in the same room. Only the enemies in the current room are active and only fields and entities in the current room can 
be interacted with.

Only the current room map will be shown in the output. There is no overview map of the whole dungeon.

</details>
<details>
<summary><h3> Adjacent Fields</h3></summary>

In this game adjacent means directly next to the field including the diagonal fields. That means that each field has a total of 8 
adjacent fields, unless it lies on the edge of the room map.

</details>
<details>
<summary><h3> Entities</h3></summary>

Entities always have a position in a room and are displayed in the room map.
Possible entities are characters (players & enemies), doors, treasure chests and death locations (= positions where a character
has been defeated).

</details>
<details>
<summary><h3> Position </h3></summary>

The position of an entity is always given in the format `<ROW>,<COLUMN>` (no whitespaces in between). It describes where 
in the current room an entity is located.

</details>
<details>
<summary><h3> Items</h3></summary>

Items only exist in an entity's inventory (characters, treasure chests or death locations) and are not displayed in the room map. 
There are consumable items, armor items and weapon items. 
</details>

</details>

<details>
<summary><h2> Important Rules</h2></summary>

The following rules give insight on how some important general things should be handled in the game.

<details>
<summary><h3> Collision Handling </h3></summary>

On **one field** there can only be **one entity**. Since some entities can move, there may be collisions when an entity tries
to move onto a field that is already occupied by another entity.

If a collision occurs this is handled differently depending on what caused it.
If it happens because of ...
* a **user input**, the user input is seen as **invalid** and the command is not executed.
* the **program logic**, the **next empty adjacent (see [Definitions](#definitions)) field** is used, where next means starting north and going clockwise.
  If no empty field can be found this way, the next circle around is used to find an empty field (again starting north going clockwise), and so on.

**Attention**: For the purposes of collision handling , the field the entity started on counts as **empty**, to make sure
that entities do not move away from their intended destination instead of towards it. If that field is the
first empty field that is found, the entity will stay there (see the last example below). This means that an empty field
will always be found in the current room.

#### Examples
In these examples the enemy `Z` tries to move to the position `2,1`.
The problem is that on position `2,1` there is already another entity. Below are examples stating the initial and the
resulting state of `Z` trying to move to `2,1`.

> **Note**: This is just an illustration for easier understanding, the output of a room will be explained in more detail further down.

```
   Initial State         Resulting State
     1   2   3             1   2   3  
   +---+---+---+         +---+---+---+
 1 |   |   |   |       1 | Z |   |   |
   +---+---+---+         +---+---+---+
 2 | D | Z |   |       2 | D |   |   |
   +---+---+---+         +---+---+---+
 3 |   |   |   |       3 |   |   |   |
   +---+---+---+         +---+---+---+
```
```
   Initial State         Resulting State
     1   2   3             1   2   3  
   +---+---+---+         +---+---+---+
 1 | B | T |   |       1 | B | T |   |
   +---+---+---+         +---+---+---+
 2 | D |   |   |       2 | D | Z |   |
   +---+---+---+         +---+---+---+
 3 | Z |   |   |       3 |   |   |   |
   +---+---+---+         +---+---+---+
```
```
   Initial State         Resulting State
     1   2   3             1   2   3  
   +---+---+---+         +---+---+---+
 1 | T | B |   |       1 | T | B |   |
   +---+---+---+         +---+---+---+
 2 | D | X |   |       2 | D | X |   |
   +---+---+---+         +---+---+---+
 3 | G | Z |   |       3 | G | Z |   |
   +---+---+---+         +---+---+---+
```
</details>
<details>
<summary><h3> Moving through the Dungeon</h3></summary>

All players move to another room at the moment one of them uses a door. When the players enter a room they spawn around 
the door they came from. This is handled like a collision with the door for every player in player order by id (see [Collision Handling](#collision-handling)).

In the following examples the players are represented with their ids as they would spawn when entering this room through the door `D`. 

> **Note**: This is just an illustration for easier understanding, the output of a room will be explained in more detail further down.

```
     1   2   3          1   2   3          1   2   3  
   +---+---+---+      +---+---+---+      +---+---+---+
 1 |<1>|<2>|   |    1 |   |   |   |    1 |   |   |   |
   +---+---+---+      +---+---+---+      +---+---+---+
 2 | D |<3>|   |    2 |<1>|<2>|   |    2 |   |<3>|<1>|
   +---+---+---+      +---+---+---+      +---+---+---+
 3 |   |   |   |    3 | D |<3>|   |    3 |   |<2>| D |
   +---+---+---+      +---+---+---+      +---+---+---+
```

Whenever the players enter a room **for the first time**, all doors in this room are locked (even the door they came
from). Every room contains at least one enemy. The doors will remain locked until all enemies are defeated. Then, all doors in this room are unlocked and will stay unlocked, even if the players leave and reenter the room at a later time.

</details>
<details>
<summary><h3> Display Commands vs. Actions</h3></summary>

Display Commands only print information to the command line and therefore do not 
change anything in the game. They do not count as actions.

All other commands are Action Commands that change something in the game and count as 
actions.

This differentiation is needed as the `Player Phase` ends after a specific amount of **actions** (not all commands). 
Additionally, at the end of the game only the amount of **actions** is needed for scoring.

The different Display and Action Commands are described in detail in [Milestone 2](Milestone_2.md).

</details>
<details>
<summary><h3> Random Number Generation </h3></summary>

The class `Random` ([Random.hpp](../Random.hpp) and [Random.cpp](../Random.cpp)) has been provided for you.
This class should be used to randomly generate numbers and **must not be changed**. If you
change anything inside this class, it might not be compatible with the test system anymore.

You should **always** use this class whenever a decision in the game needs to happen randomly. For example, if an enemy
randomly decides between attacking with a *handaxe* (possibility 1) or a *shortbow* (possibility 2), you should use the `Random` class to
generate a number from **1** to the **number of total possibilities** (both inclusive). In this example, you would have
to generate a random number between 1 and 2. If the generated number is 1, the enemy will attack with the handaxe, and if the generated
number is 2, it will attack with the shortbow.

You should also use this class to implement the dice mechanics.

</details>
<details>
<summary><h3> Rolling Dice</h3></summary>

In this game many decisions are made by rolling different dice.

These are marked as `<AMOUNT_OF_DICE> d<DICE_TYPE>`, where
- `<AMOUNT_OF_DICE>` specifies how many dice should be rolled and
-  `<DICE_TYPE>` specifies how many sides the dice should have

This could for example be `1 d4`, `3 d6` or even `2 d20`.

To roll a dice the `Random` class must be used. The lower bound for every dice is `1` and the upper bound is the same as 
the `<DICE_TYPE>`.

As sometimes more than one dice of the same category are rolled at the same time (`<AMOUNT_OF_DICE>` greater 1),
the results of every rolled dice should be added up and only the total printed.

Printing the result of a dice roll should look like this:
```
[Dice Roll] <AMOUNT_OF_DICE> d<DICE_TYPE> resulting in a total value of <SUM_OF_RESULTS>.\n
\n
```

#### Example
`3 d6` (results of the random function: 1, 6, 2)
```
[Dice Roll] 3 d6 resulting in a total value of 9.\n
\n
```
</details>
</details>

## Initialisation of the Game

<details>
<summary><h3> Config Files</h3></summary>

Two different kinds of config files are used:
* a dungeon config file containing the configuration of the rooms
* a story config file containing story related texts

</details>

<details>
<summary><h3> Starting the Program</h3></summary>

The program is started with two command line parameters which contain the paths to the config files that should be opened.
```
./a2 <DUNGEON_CONFIG_FILE> <STORY_CONFIG_FILE>
```

This is an example for a valid program start:
```
./a2 configs/dungeon_config.txt configs/story_config.txt
```
If the program is called

* with more or less command line parameters or
* one of the config files cannot be opened for reading or
* does not start with the correct [magic number](<https://en.wikipedia.org/wiki/Magic_number_(programming)>),

the corresponding error message should be printed and the program should end with the correct return value
(see [Return Values and Error Messages](#return-values-and-error-messages)).

The config files are text files. If the config file starts with the correct magic number (the string `STORY\n` for the 
story config file and the string `DUNGEON\n` for the dungeon config file), you can assume that it is formatted correctly
and only contains valid information. Further checks of the file content are not necessary.

</details>

<details>
<summary><h3> Parsing the Story Config</h3></summary>

Following the _magic number_ (`STORY\n`) this file consists of lines of key value pairs. The key and the value are separated by a `:`.
Each line represents a story related text bit. The `<TEXT>` should be printed whenever the corresponding `<STORY_KEY>` 
occurs in the assignment description.

```
<STORY_KEY>:<TEXT>\n
```

- Each `<STORY_KEY>` is **unique** in the story config and written in **uppercase with underlines**. 
- Each `<STORY_KEY>` starts with the type abbreviation followed by an underline and the remaining key.

| Type Abbreviation | Type          | Symbol |
|-------------------|---------------|--------|
| N                 | Narrator      | `**`   |
| E                 | Error Message | `!!`   |

- Every `;` in the `<TEXT>` should be replaced with `\n` in the output.
- Every line in the output that contains at least one character should start with a **symbol** (depending on the type)  and one space (as seen in the example below).
- Every empty line in the output should **not** start with a symbol and a space.
- The `<TEXT>` is **whitespace sensitive**, meaning a whitespace in the config file should also be printed in the output.


#### Example

This could be the beginning of a valid story config file.
```
STORY\n
N_REMAINING_KEY:;text with spaces;and multiple lines;;\n
```
The following should be printed whenever the story key `N_REMAINING_KEY` occurs in the assignment description.
```
\n
** text with spaces\n
** and multiple lines\n
\n
```

</details>

<details>
<summary><h3> Parsing the Dungeon Config</h3></summary>

Same as in the story config there is a _magic number_ (`DUNGEON\n)` which is followed by the rooms in the dungeon.

Each line represents one room. It starts with the room info. After that, an arbitrary number of entities the room contains
are listed **in no specific order**. Entities are separated from the room info and from each other by a `;`.

The room info starts with the room id. This is followed by a `:` and the dimensions of the room.

An entity is always listed in the following way:
- entity abbreviation
- `:`
- entity position in the room as `<ROW>,<COLUMN>`
- `:`
- 1 or more parameters that give additional info about the entity, separated by `,`

This means a single line in the dungeon config file has the following syntax:
```
<ROOM_ID>:<WIDTH>,<HEIGHT>;<ENTITY_ABBREVIATION>:<ROW>,<COLUMN>:<PARAMETERS>; ... ;\n
```
#### Possible Entities in Config File
| Abbreviation | Entity         | Nr of Parameters | Parameters                                                        |
|--------------|----------------|------------------|-------------------------------------------------------------------|
| D            | Door           | 1                | `0` (representing entrance/exit) or `<ROOM_ID>` of connected room |
| Z            | Zombie         | 1                | `<ENEMY_ID>`                                                      |          
| G            | Goblin         | 1                | `<ENEMY_ID>`                                                      |          
| L            | Lich           | 1                | `<ENEMY_ID>`                                                      | 
| T            | Treasure Chest | 1 to many        | `<VALUE_NEEDED_TO_UNLOCK>`, `<LIST_OF_ITEMS>`                     |

##### Enemies
Enemy ids are unique over the type of enemy per room, but different enemy types with the same id may exist.

##### Treasure Chest
`<VALUE_NEEDED_TO_UNLOCK>` is the value the player needs to beat to open the treasure chest. This is described further in Milestone 2.

The Treasure Chest has a list of the items it contains as a parameter (`<LIST_OF_ITEMS>`). Every item in the chest is 
represented in the config file as the abbreviation of the item (see [Items Overview](Items.md)) followed by the amount, 
separated by a `,`. Items are separated from each other also by a `,`.

The items may appear in the list in the config file in any order, but in the output they should always be printed in 
**alphabetical order** by the item abbreviation.

#### Example
This could be the beginning of a valid dungeon config file.
```
DUNGEON\n
1:8,5;D:1,1:0;G:5,3:1;T:1,5:10,NEHP,2,BOLT,20;Z:4,5:1;Z:3,7:2;D:5,8:2;\n
```

Using this configuration the room with the id `1` is `8` fields wide and `5` fields high. It contains:
- one door at position `1,1` that connects to the exit/entrance of the dungeon
- a goblin at position `5,3` with enemy id `1`
- a treasure chest at position `1,5` that can be unlocked by beating a value of `10` and contains 2 `Normal Health Potion` and 20 `Bolt`
- a zombie at position `4,5` with enemy id `1`
- a zombie at position `3,7` with enemy id `2`
- one door at position `5,8` that connects to the room with id `2`

How this would be printed as a room map is shown further down (see [Printing the Current Room](#printing-the-current-room) > Room Map).

</details>

<details>
<summary><h3> Start Message</h3></summary>

At the beginning of the game the first story message should be printed.\
The story key for this is: `N_INTRO`

</details>

## Initialising the Players

In the following section there are some user inputs, keep in mind the [General Notes](../README.md#assignment-description) 
which apply for the whole assignment.

<details>
<summary><h3> Number of Players</h3></summary>

To determine the number of players that are playing, print the message below.
Valid inputs are integers between 1 and 3 inclusive.

```
How many players want to join the adventure? (1 to 3)\n
> 
```
If the user input is invalid, the following error message should be printed:
```
Please enter a number of players between 1 and 3.\n
> 
```

> **Attention**: The `quit` command and `EOF` are also valid inputs here.

</details>

<details>
<summary><h3> Choose Player Name and Type</h3></summary>

To get the player's name and which player type they want to play, each player is asked to choose both of them in player order by id.

#### Name
The name can have up to 10 characters and needs to be unique, meaning that two players are not allowed to 
have the same name in game. The name should be saved exactly like the user input, without e.g. changing the case or trimming 
any whitespaces.

```
\n
Player <PLAYER_ID> what do you wish to be called? (max length 10 characters)\n
> 
```
- <PLAYER_ID> should be replaced by `1`, `2` or `3`

If the user input is invalid, the following error message will be printed:
```
Please enter a unique name with not more than 10 characters.\n
> 
```

> **Note**: For simplicity, you don't need to handle the `quit` command \ `EOF` here. Any string of correct length that 
> is input here is considered a name, so a player could call themselves "quit" if they wanted to.

#### Player Type
There are three different player types a player can choose from.
More information about the player types can be found in [this file describing all characters](Characters.md).

```
<PLAYER_NAME>, please choose a player type:\n
  [W] Wizard     <AMOUNT>/1\n
  [B] Barbarian  <AMOUNT>/1\n
  [R] Rogue      <AMOUNT>/1\n
> 
```

Here `<AMOUNT>` should be initialised to 0 and increased by 1 whenever a player selects a given player type.
The maximum amount of people playing as a certain player type in this game is 1, so a single
player type cannot be selected twice. 

On a correct input the next player chooses a name and type.\
If the input is invalid one of the following error messages should be printed 
depending on the input. If the input was ...

... not one of the possible types (not `W`, `B`, or `R`).
```
Please enter a letter representing your desired player type (W, B, or R).\n
> 
```
... was a type already chosen by another player.
```
This player type is no longer available. Please choose a different player type.\n
> 
```

> **Attention**: The `quit` command and `EOF` are also valid inputs here. How to handle this is described further down.

</details>

<details>
<summary><h3> Printing the Players</h3></summary>

At the end of the initialisation the players should be printed to the console in player order by id in the following format:
```
\n
-- Players --------------------------------------\n
  Player <PLAYER_ID>: <PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>"\n
  <every other player in the same format as in the line above>
\n
```

</details>

## Player Phase

After printing the players, the first `Player Phase` starts by entering the first room. The players spawn at the door with
id `0` (dungeon entrance/exit) in the room with id `1`.

The **first** player phase starts by [printing the current room](#printing-the-current-room) as shown below, followed by the command prompt. 
The different commands and the command handling are described in detail in [Milestone 2](Milestone_2.md).

The players can enter commands until they have performed `<NUMBER_OF_PLAYERS>` **actions**, which ends the `Player Phase`
and starts the next phase. This can either be the `Enemy Phase`(see [Milestone_2.md](Milestone_2.md)) or, if there are 
no more enemies in the current room, the next `Player Phase`.

`<NUMBER_OF_PLAYERS>` is the number of players at the start of the game and does not change, even if players are defeated.

Whenever the players change to a different room, the action counter in the current `Player Phase` resets to `0`.

<details>
<summary><h3> Printing the Current Room</h3></summary>

The current room is printed with the following elements:
- Room Story (on first entrance only)
- Room Header
- Room Map
- List of Enemies

For a full example see [complete example output of Milestone 1](#example-of-milestone-1-output).

### Room Story
When a room is entered for the first time in the game, the corresponding message of the story config should be printed.\
The story key for this is: `N_ROOM_<ROOM_ID>`

If the room contains an **[enemy type](Characters.md#enemy-characters)** that has not occurred in the game before, 
additionally the message of the new enemy type should be printed. The next time that enemy type appears **will not** 
trigger that output again. The message should only be printed once, even if the room contains multiple enemies of the type.
The story key for this is: `N_ENEMY_<ENEMY_TYPE_ABBREVIATION>`

If multiple enemy types appear for the first time in the same room, the messages should be printed in alphabetical order
of the `<ENEMY_TYPE_ABBREVIATION>`.

The printing of the whole room story can be turned off with the `story` command (see Milestone 2).

### Room Header
Everytime the room is printed (even when the room story or room map is not) the following header of the current room should be printed:

```
\n
-- ROOM <CURRENT_ROOM_ID> (<NUMBER_OF_COMPLETED_ROOMS>/<NUMBER_OF_ROOMS_IN_DUNGEON> completed) --------------------\n
\n
```

#### Example
```
\n
-- ROOM 1 (0/4 completed) --------------------\n
\n
```

### Room Map
A room is represented as a grid of fields. Every field can either be empty or hold one of the possible entities and
additional information. 

As seen in the example below, a field output consists of the following:
```
<SYMBOL><ENTITY_ABBREVIATION><ID>
```
- `<SYMBOL>` gives additional information about the current state of the entity
- `<ENTITY_ABBREVIATION>` shows the type of entity that is on the field
- `<ID>` is either `<ENEMY_ID>` (if the entity is an enemy) or `<ROOM_ID>` (if the entity is a door, this is the id of the 
  room it leads to, or `0` if it is the dungeon entrance/exit)

Fields are separated by horizontal and vertical lines in the output.

If there is no `<SYMBOL>`, `<ENTITY_ABBREVIATION>` or `<ID>` to print, a space should be printed instead. 
This way the `<ENTITY_ABBREVIATION>` should always be directly under the column number.

Possible Entities:

| Abbreviation | Entity         |
|--------------|----------------|
| W            | Wizard         |
| B            | Barbarian      |
| R            | Rogue          |
| Z            | Zombie         |
| G            | Goblin         |                                                              
| L            | Lich           |
| D            | Door           |
| T            | Treasure Chest |
| X            | Death Location |

Possible Symbols:

| Symbol | Meaning                               | Affected Entity      |
|--------|---------------------------------------|----------------------|
| #      | entity is locked                      | Door, Treasure Chest |

For visualisation matters no numbers with more than one digit will appear in the Room Map in any way. 

#### Example
Room in [dungeon config](#parsing-the-dungeon-config):
```
     1   2   3   4   5   6   7   8  \n
   +---+---+---+---+---+---+---+---+\n
 1 |#D0| B |   |   |#T |   |   |   |\n
   +---+---+---+---+---+---+---+---+\n
 2 | R | W |   |   |   |   |   |   |\n
   +---+---+---+---+---+---+---+---+\n
 3 |   |   |   |   |   |   | Z2|   |\n
   +---+---+---+---+---+---+---+---+\n
 4 |   |   |   |   | Z1|   |   |   |\n
   +---+---+---+---+---+---+---+---+\n
 5 |   |   | G1|   |   |   |   |#D2|\n
   +---+---+---+---+---+---+---+---+\n
```

In addition to the entities in the dungeon config this example includes additional things that occur in the game.
- The players are situated as they spawned coming from the left door (see also [Important Rules](#important-rules) > Moving through the Dungeon).
- Both doors have the locked symbol `#` because the players aren't able to leave the room as long there are still enemies.
- The treasure chest is also still locked (`#`) as nobody has opened it yet.

Printing the map can be turned off with the `map` command (see Milestone 2).

### Printing the Enemies
Next all enemies in the current room are printed to show their health points. They should be printed in **alphabetical order
by enemy abbreviation and by ascending id**. An enemy output consists of the following:
```
<ENEMY_ABBREVIATION><ENEMY_ID>: <CURRENT_HEALTH>/<MAX_HEALTH>
```
All enemies should be printed in a single line and separated by `,`. After the last enemy, no `,` and space should
be printed. There are three spaces in front of the first enemy. If there are no enemies, an empty line should be printed.

#### Example
```
   G1: 10/21, Z1: 10/11, Z2: 11/11\n
```


</details>

<details>
<summary><h3> Basic Command Handling</h3></summary>

### Prompting the User for Input
To prompt the user for input in the `Player Phase`, next the following should be printed:
- the message with the story key `N_PROMPT_MESSAGE`
- the command prompt

The **command prompt** is printed like this (note the trailing space!):
```
> 
```
After printing the command prompt, the program waits for an input.
Like with other user input, the [General Notes](../README.md#general-notes) apply.

#### Example
```
\n
** What do you want to do?\n
> 
```
> **Note**: The `\n` is part of the `N_PROMPT_MESSAGE` and doesn't need to be printed additionally.


### Command: quit / EOF
**Syntax**: `quit` or `EOF` (End of File, not the string "EOF")

This is a special command that terminates the game with the return value `0`. It should be possible to use this command anytime the
program waits for user input. All resources should be freed before termination.

Further commands as well as error handling are described in [Milestone 2](Milestone_2.md).

</details>

## Return Values and Error Messages
| Return Value | Description                                                                          | Error Message                          |
|--------------|--------------------------------------------------------------------------------------|----------------------------------------|
| 0            | Game was ended with a command or game has ended successfully                         | no error message                       |
| 1            | Memory could not be allocated                                                        | `Error: Not enough memory!\n`          |
| 2            | Wrong number of command line parameters                                              | `Error: Wrong number of parameters!\n` |
| 3            | Config file cannot be opened for reading or does not start with correct magic number | `Error: Invalid file (<FILE_PATH>)!\n` | 

Here `<FILE_PATH>` represents the path to the file throwing an error e.g. `configs/story_config.txt`. If both files throw an error,
the message should only be printed for the first one, so the dungeon config file.

## Example of Milestone 1 Output

<details>
<summary>Example</summary>

```

** Welcome to the exciting world of Dungeons & Classes!

How many players want to join the adventure? (1 to 3)
> 2

Player 1 what do you wish to be called? (max length 10 characters)
> David
David, please choose a player type:
  [W] Wizard     0/1
  [B] Barbarian  0/1
  [R] Rogue      0/1
> b

Player 2 what do you wish to be called? (max length 10 characters)
> Alex
Alex, please choose a player type:
  [W] Wizard     0/1
  [B] Barbarian  1/1
  [R] Rogue      0/1
> R

-- Players --------------------------------------
  Player 1: Barbarian [B] "David"
  Player 2: Rogue [R] "Alex"

** You have entered the dungeon of the evil Lich!
** Help the anxious souls of the nearby village by freeing
** them from the monsters that haunt it!

** Beware, there are zombies in this room!

-- ROOM 1 (0/5 completed) --------------------

     1   2   3   4   5  
   +---+---+---+---+---+
 1 |   |   |#D2|   |   |
   +---+---+---+---+---+
 2 |   |   |   | Z1|   |
   +---+---+---+---+---+
 3 | B | R |   |   |   |
   +---+---+---+---+---+
 4 |#D0|   |   |   |   |
   +---+---+---+---+---+
   Z1: 15/15

** What do you want to do?
> quit
```
</details>
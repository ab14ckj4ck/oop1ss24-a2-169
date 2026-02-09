# Milestone 2
After the basic structure was implemented in Milestone 1, this milestone mostly deals with the game logic. 

## Table of Contents
_General information about the game:_
* Definitions _(see [Milestone 1](Milestone_1.md))_
* Important Rules _(see [Milestone 1](Milestone_1.md))_
* [Item Overview](Items.md) (file describing the different items)
* [Character Overview](Characters.md) (file describing the different characters)
* [Placeholder List](Placeholder_List.md) (file listing all placeholders used in the description)

1. **Initialisation of the Game**
    1. Config files _(see [Milestone 1](Milestone_1.md))_
    2. Starting the Program _(see [Milestone 1](Milestone_1.md))_
    3. Parsing the Story Config _(see [Milestone 1](Milestone_1.md))_
    4. Parsing the Dungeon Config _(see [Milestone 1](Milestone_1.md))_
    5. Start Message _(see [Milestone 1](Milestone_1.md))_
    6. Initialising the Players _(see [Milestone 1](Milestone_1.md))_
2. **Game Logic**
    1. Player Phase _(see [Milestone 1](Milestone_1.md))_
    2. Printing the Current Room _(see [Milestone 1](Milestone_1.md))_
    3. Basic Command Handling _(see [Milestone 1](Milestone_1.md))_
    4. [Command Handling and Commands Overview](#command-handling-and-commands-overview)
    5. [Display Commands](#display-commands)
    6. [Action Commands](#action-commands)
    7. [Enemy Phase](#enemy-phase)
3. **Final Phase**
    1. [End of Game](#end-of-game)
    2. [Scoring](#scoring)
    3. [Save Scoring to File](#save-to-file)


<details>
<summary><h2> Command Handling and Commands Overview</h2></summary>

As already mentioned, during the `Player Phase` the players can enter commands until
they have performed `<NUMBER_OF_PLAYERS>` **actions**, which ends the `Player Phase` and starts the next phase. The next phase can
either be the [`Enemy Phase`](#enemy-phase) or, if there are no more enemies in the current room, the next `Player Phase`.

`<NUMBER_OF_PLAYERS>` is the number of players at the start of the game and does not change, even if players are defeated.

How the user is prompted for input is described in [Milestone 1](Milestone_1.md).

<details>
<summary><h3> Invalid Commands Handling </h3></summary>

Whenever a command input is entered it should be checked for validity. If one of the following errors occurs,
the corresponding error message with the listed story key is printed. Then print the command prompt again (without any 
preceding prompt messages). The player should now be able to enter a new command. 

An invalid "action" command **does not count as an action**.

For each invalid command only print one error message. If several errors occur, the error with the highest priority
is printed. The earlier the command is listed in the table below, the higher its priority.

| Errors                           | Description                                                                                                                                                              | Story Key                      |
|----------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------|
| **unknown command**              | the entered command is not known                                                                                                                                         | `E_UNKNOWN_COMMAND`            |
| **invalid number of parameters** | there are more or less parameters than the given command should have                                                                                                     | `E_INVALID_PARAM_COUNT`        |
| **invalid parameter value**      | one of the parameters entered does not have a value that can be expected (see [Possible Parameters](#possible-parameters)) (e.g. number instead of a possible character) | `E_INVALID_PARAM`              |
| **entity or item not available** | the entity is not in the current room (e.g. player already defeated) or the item is neither in the inventory of the player nor equipped        | `E_ENTITY_OR_ITEM_UNAVAILABLE` |
| **invalid position**             | the position is outside the room, out of reach (e.g. not adjacent if needed) or does not point to an expected entity                                                     | `E_INVALID_POSITION`           |

> **Attention:** The "invalid number of parameters" error can also apply to the `quit` command.

</details>

<details>
<summary><h3> Command Types </h3></summary>

There are two different command types (other than `quit`/`EOF`):
- [Display Commands](#display-commands)
- [Action Commands](#action-commands)

Display Commands only print information to the command line and therefore do not change anything
in the game. They do not count as actions.

All other commands are Action Commands that change something in the game and count as **actions**.

</details>

<details>
<summary><h3> Possible Parameters </h3></summary>

The following table shows the possible parameters a command can have and which input values are valid for the corresponding type 
(further constraints may be listed in the command description itself).

|                              | Description                             | valid parameter values                                              | Example |
|------------------------------|-----------------------------------------|---------------------------------------------------------------------|---------|
| `<PLAYER_TYPE_ABBREVIATION>` | abbreviation of a player type           | `W`, `B`, `R`                                                       | `W`     |
| `<POSITION>`                 | position of a field in the current room | format: `<ROW>,<COLUMN>` (both integers, no whitespaces in between) | `2,1`   |
| `<ITEM_ABBREVIATION>`        | abbreviation of an item                 | an item abbreviation from the [item overview](Items.md)             | `PARM`  |

If an entered parameter does not fit the expected type, this causes an **invalid parameter value** error.

</details>
</details>

<details>
<summary><h2> Display Commands</h2></summary>

In this section all display commands are described.
These commands print game information or handle display options.

**Display commands do not ...**
* **count as actions**: will not increase the number of performed actions
* **affect the map**: therefore, the map should not be printed afterward


<details>
<summary><h3> Display Command: help</h3></summary>

**Syntax**: `help`

This command outputs the following help text:
```
-- Commands ----------------------------------------\n
- help\n
     Prints this help text.\n
\n
- quit\n
    Terminates the game.\n
\n
- story\n
    Activates or deactivates the stories of the rooms.\n
\n
- map\n
    Activates or deactivates the map.\n
\n
- positions\n
    Prints the current positions of the players and enemies.\n
\n
- player <PLAYER_TYPE_ABBREVIATION>\n
    Prints player information of the specific player.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
\n
- inventory <PLAYER_TYPE_ABBREVIATION>\n
    Prints the inventory of the specific player.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
\n
* move <PLAYER_TYPE_ABBREVIATION> <POSITION>\n
    Lets the player move to an adjacent field.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
    <POSITION>: field at position: <ROW>,<COLUMN>\n
\n
* loot <PLAYER_TYPE_ABBREVIATION> <POSITION>\n
    Lets the player loot an adjacent chest.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
    <POSITION>: field at position: <ROW>,<COLUMN>\n
\n
* use <PLAYER_TYPE_ABBREVIATION> <ITEM_ABBREVIATION>\n
    Lets the player use a potion or equip armor or weapons.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
    <ITEM_ABBREVIATION>: abbreviation of an item in the players inventory\n
\n
* attack <PLAYER_TYPE_ABBREVIATION> <POSITION>\n
    Lets the player attack with the equipped weapon on the given position.\n
    <PLAYER_TYPE_ABBREVIATION>: type of the player\n
    <POSITION>: field at position: <ROW>,<COLUMN>\n
\n
-- Legend ------------------------------------------\n
- display commands\n
* action commands (count as actions)\n
----------------------------------------------------\n
```
</details>

<details>
<summary><h3> Display Command: story</h3></summary>

**Syntax**: `story`

This command influences whether the Room Story (see [Milestone_1](Milestone_1.md) > Printing the Current Room) will be printed when entering a room for 
the first time. Initially the story output is active.

If the output of the story is **active** and the command `story` is entered, the output is deactivated.
The room story will not be printed when entering new rooms for the first time.

If the output of the story is **not active** and the command `story` is entered, the output is activated.

> **Attention**: A room counts as already having been entered for the first time as soon as the players enter it, even if 
> the story output is not active at that moment.

</details>

<details>
<summary><h3> Display Command: map</h3></summary>

**Syntax**: `map`

This command influences whether the Room Map (see [Milestone_1](Milestone_1.md) > Printing the Current Room) will be printed when entering a room and after 
an action command. 
Initially the output of the map is active.

If the output of the map is **active** and the command `map` is entered, the output is deactivated. 
From now on, the map is not printed anymore.

If the output of the map is **not active** and the command `map` is entered, the output is activated. **The map
is printed once immediately.** The output of the map then stays active from now on.

</details>

<details>
<summary><h3> Display Command: positions</h3></summary>

**Syntax**: `positions`

This command outputs for every player and enemy in the current room the positions on the map.

First all players are printed in player order by id, then all enemies are printed in **alphabetical order and by ascending id**.

#### Printing a Player
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" on (<ROW>,<COLUMN>)\n
```
#### Printing an Enemy
```
<ENEMY_TYPE_NAME> <ENEMY_ID> [<ENEMY_TYPE_ABBREVIATION><ENEMY_ID>] on (<ROW>,<COLUMN>)\n
```
#### Example
```
Wizard [W] "John" on (2,1)\n
Barbarian [B] "Lisa" on (2,3)\n
Rogue [R] "Max" on (4,1)\n
Zombie 1 [Z1] on (2,2)\n
Zombie 2 [Z2] on (3,4)\n
```

</details>

<details>
<summary><h3> Display Command: player</h3></summary>

**Syntax**: `player <PLAYER_TYPE_ABBREVIATION>`

This command outputs the current player information of the specified player.

The values that should be inserted for the different player attributes are further described in the
[character overview](Characters.md).

```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" on (<ROW>,<COLUMN>)\n
  Armor Value: <AV>\n
  Current Health: <CURRENT_HEALTH>\n
  Max Health: <MAX_HEALTH>\n
  Strength: <STR>\n
  Vitality: <VIT>\n
```
- `<AV>` is the player type base armor value or the armor value of an equipped armor item, whichever is higher

The values of the player attributes should be **right aligned**. To make that work you can assume that no value can have
more than **5 digits** and that there is at least one space between the longest attribute name and the value. This way 
you can prefix every number with spaces so that the value always aligns correctly.

The following illustration shows what the output should look like. Spaces should be printed as they are, X are either 
replaced by a single digit of a number or a space, depending on how many digits the number has.
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" on (<ROW>,<COLUMN>)\n
  Armor Value:    XXXXX\n
  Current Health: XXXXX\n
  Max Health:     XXXXX\n
  Strength:       XXXXX\n
  Vitality:       XXXXX\n
```


</details>

<details>
<summary><h3> Display Command: inventory </h3></summary>

**Syntax**: `inventory <PLAYER_TYPE_ABBREVIATION>`

This command outputs equipped items and the inventory of the specified player. 

Each inventory consists of the armor items, weapons and consumables the player possesses.
If the player **doesn't have any item of one item type in their inventory** (e.g. no weapons) the whole section of this type should
be **skipped**. 

An item with the amount zero should never be printed.

First the `<PLAYER_NAME>` of the player is printed, which is followed by the different item sections:
```
Inventory "<PLAYER_NAME>"\n
```

#### Equipped Items
The player can have at most a single armor and a single weapon equipped. These items are printed next in the following format:

```
  Equipped Armor: [<ARMOR_ABBREVIATION>] <ARMOR_NAME>\n
  Equipped Weapon: [<WEAPON_ABBREVIATION>] <WEAPON_NAME>\n
```
If there is no weapon or no armor equipped, the abbreviation should be replaced by `-` and the name replaced by `None`.
Equipped items should **not** be considered again in the sections below.

#### Armor Items 
Next all **unequipped** armor items are listed in a single line, sorted by their item abbreviation in **alphabetical 
order** and separated by `,`.

```
  Armor: [<ARMOR_ABBREVIATION>] <ARMOR_NAME> (<AMOUNT>), <...>, [<ARMOR_ABBREVIATION>] <ARMOR_NAME> (<AMOUNT>)\n
```

#### Weapon Items
Next all **unequipped** weapon items are listed in a single line, sorted by their item abbreviation in **alphabetical 
order** and separated by `,`.

```
  Weapons: [<WEAPON_ABBREVIATION] <WEAPON_NAME> (<AMOUNT>), <...>, [<WEAPON_ABBREVIATION>] <WEAPON_NAME> (<AMOUNT>)\n
```

#### Consumable Items
Next all consumable items are listed in a single line, sorted by their item abbreviation in **alphabetical order** and
separated by `,`.

```
  Consumables: [<CONSUMABLE_ABBREVIATION>] <CONSUMABLE_NAME> (<AMOUNT>), <...>, [<CONSUMABLE_ABBREVIATION>] <CONSUMABLE_NAME> (<AMOUNT>)\n
```

#### Example

```
Inventory "John"\n
  Equipped Armor: [-] None\n
  Equipped Weapon: [DAGG] Dagger\n
  Armor: [CHML] Chain Mail (1), [LARM] Leather Armor (1)\n
  Weapons: [DAGG] DAGGER (2), [SBOW] Shortbow (1)\n
  Consumables: [ARRW] Arrow (12)\n
```
</details>
</details>

<details>
<summary><h2> Action Commands</h2></summary>

These commands count as actions and therefore after successful execution of these commands:
* the number of performed **actions** increases by one
* the **current room is printed** as described in Milestone 1


<details>
<summary><h3> Action Command: move</h3></summary>

**Syntax**: `move <PLAYER_TYPE_ABBREVIATION> <POSITION>`

With this command the specified player can move to an adjacent `<POSITION>`.

The `<POSITION>` is **invalid** if it is not adjacent to the player, is outside the current room's boundaries,
or does not point to an empty field or a door.
How **invalid positions** are handled is described in the invalid commands section (see 
[Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands).

If a player's **position changes**, the following message should be printed:
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" moved to (<POSITION>).\n
```

#### Move to Empty Field
If the player moves to an empty field their position changes to the given `<POSITION>`.

#### Move to Door
In addition to an empty field the player can also move onto a field that contains a door.

If this **door is locked** the message with the story key `E_MOVE_LOCKED_DOOR` is printed. 
This is **not counted** as a performed action and the player's position does not change.
Then print the command prompt again (without any preceding prompt messages), and wait for a new user input.

If this  **door is unlocked**, first the position change message is printed. Then all players enter the room the door
goes to. They now spawn in the new room around the door which would go back to the previous room. For how that works and
some examples see [Milestone 1](Milestone_1.md) > Important Rules > Moving through the Dungeon.

> **Attention:** Moving to a new room resets the action counter in the current Player Phase to `0`.

</details>

<details>
<summary><h3> Action Command: loot</h3></summary>

**Syntax**: `loot <PLAYER_TYPE_ABBREVIATION> <POSITION>`

With this command the specified player can loot an adjacent treasure chest or death location.

The `<POSITION>` is invalid, if it's not adjacent to the player, is outside the current room's boundaries, or does not
point to a Death Location or Treasure Chest.
How **invalid positions** are handled is described in the invalid commands section (see
[Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands).

#### Death Location (`X`)
A death location appears whenever a player or enemy is defeated and holds the items the character had in their inventory and 
any items they had equipped.

Now a player can [successfully loot](#successful-loot) the death location and the death location is deleted from the game afterward.

#### Treasure Chest (`T`)
The location and contents of a treasure chest are specified in the dungeon config. 

Before a player can loot a locked treasure chest they need to roll `1 d20` to decide if they are able to **unlock the chest**.
To open the chest the value of `1 d20 + <VIT>` needs to be higher than the `<VALUE_NEEDED_TO_UNLOCK>` in the dungeon config.

`<VIT>` is the vitality stat of the player.

Print the following to inform the player of what they need to roll to open the chest:
```
** To unlock this chest you need to roll at least <MIN_VALUE_TO_ROLL> to reach the <VALUE_NEEDED_TO_UNLOCK> needed.\n
```
`<MIN_VALUE_TO_ROLL>` is calculated with `<VALUE_NEEDED_TO_UNLOCK> - <VIT>`.

Then roll the dice (see [Milestone 1](Milestone_1.md) > Important Rules) and print the dice roll output.

If the **chest could not be opened** (`<MIN_VALUE_TO_ROLL>` was not reached), print the message with the story key 
`N_LOOT_CHEST_LOCKED`. The player does not obtain anything. The command still counts as valid and therefore
also as an **action**.

If the player was able to unlock the chest, they can now successfully loot the treasure chest and the treasure chest is
deleted from the game afterward.


#### Successful Loot
On success the player obtains all items stored in the given entity. The entity is removed from the game.

First the message which has the story key `N_LOOT_SUCCESSFUL` should be printed.
Then, all received items should be listed in the same format as in the `inventory` command.

```
  Armor: [<ARMOR_ABBREVIATION>] <ARMOR_NAME> (<AMOUNT>), <...>, [<ARMOR_ABBREVIATION>] <ARMOR_NAME> (<AMOUNT>)\n
  Weapons: [<WEAPON_ABBREVIATION] <WEAPON_NAME> (<AMOUNT>), <...>, [<WEAPON_ABBREVIATION>] <WEAPON_NAME> (<AMOUNT>)\n
  Consumables: [<CONSUMABLE_ABBREVIATION>] <CONSUMABLE_NAME> (<AMOUNT>), <...>, [<CONSUMABLE_ABBREVIATION>] <CONSUMABLE_NAME> (<AMOUNT>)\n
```

If the entity **doesn't have any item of one item type in it** (e.g. no weapons) the whole section of this type should
be **skipped**.

The new items should be added to the player's inventory.

#### Example
```
  Armor: [BPLT] Breast Plate (1)\n
  Consumables: [ARRW] Arrow (20), [NHEP] Normal Health Potion (2)\n
```

</details>

<details>
<summary><h3> Action Command: use</h3></summary>

**Syntax**: `use <PLAYER_TYPE_ABBREVIATION> <ITEM_ABBREVIATION>`

The `use` command enables a player to use certain items.

Further information about items can be found in the [item overview](Items.md).

#### Ammunition
Ammunition cannot be used with the `use` command. If the item specified is ammunition, this should be handled like an 
**invalid parameter value** error as described in the invalid commands section (see
[Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands).

#### Potions
If the item is a potion, it will have an effect on the player and disappear from their inventory and thus also from the game.
When the player uses a potion, the following message should be printed:
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" consumed "<CONSUMABLE_NAME>".\n
```
If the specified potion is not in the player's inventory, this should be handled as described in 
the invalid commands section (see [Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands: **entity or item not available**).

##### Health Potions

If the potion is a health potion, it heals the player by the amount specified in the potion's description. 

A player's health *cannot* exceed its maximum value, which is defined in the [characters](Characters.md) file. Any healing 
that would go above the maximum is wasted and does not do anything.

First, roll the dice specified in the potion's description and print the dice roll output.

Then following message should be printed:
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" regenerates <HEALTH> health.\n
```
- `<HEALTH>` should be replaced by the result of the calculation in the potion's description, limited by the amount of 
  health missing until the player reaches maximum health

##### Resistance Potions

If the potion is a resistance potion, it makes the player resistant to a certain type of damage. This means the player will take only 
50 % of all damage received of that type (the halved damaged is rounded down to the next integer). This effect lasts until the player leaves the current room.

Effects of multiple resistance potions *do not stack*. If a character is affected by potion A and uses
potion B, the effect of potion B replaces the effect of potion A.

The following message should be printed:
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" is now resistant to "<DAMAGE_TYPE>" until leaving the room.\n
```
- `<DAMAGE_TYPE>` should be replaced by the damage type in the potion's description (`Fire`, `Cold`, `Force` or `Acid`).

#### Equipment

If the item is an armor or a weapon item, it will change the player's equipment.

A player character can have at most **one armor item** and **one weapon item** equipped at any point in the game.

If the player already has an item with the specified `<ITEM_ABBREVIATION>` equipped, the item should be removed from the
equipped items and put back in the player's inventory.

If the player does not have an item with the specified `<ITEM_ABBREVIATION>` equipped, and has the specified item in 
their inventory, the item should be removed from the inventory and equipped. Any previously equipped item in the same 
slot (armor or weapon) should be put back into the inventory.

If the player has the specified item neither equipped, nor in their inventory, this should be handled as described in 
the invalid commands section (see [Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands: **entity or item not available**).


</details>

<details>
<summary><h3> Action Command: attack</h3></summary>

**Syntax**: `attack <PLAYER_TYPE_ABBREVIATION> <POSITION>`

With this command a player can perform attacks using their equipped weapon that target the specified position.

If **no weapon is equipped**, the attack fails without counting as a performed action and the error message with the
story key `E_ATTACK_NO_WEAPON_EQUIPPED` should be printed instead. Then print the command prompt again (without any 
preceding prompt messages), and wait for a new user input.

Specific information about all weapons can be found in the [item overview](Items.md#weapon-items).

#### Attack Types
Every weapon has exactly one **type of attack** that can be performed with it. The type of attack affects which positions
count as valid for the `<POSITION>` parameter. How **invalid positions** are handled is described in the invalid commands section (see
[Command Handling and Commands Overview](#command-handling-and-commands-overview) > Invalid Commands).

| Type   | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
|--------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Melee  | Close combat. <br/> Only fields adjacent to the attacker's position are valid.                                                                                                                                                                                                                                                                                                                                                                                                                                    | 
| Ranged | Ranged combat. <br/> All fields in the current room are valid. <br/> Consumes one instance of the required ammunition from the player's inventory (arrows for bows, bolts for crossbows, no ammunition for quarterstaffs). <br/> Should the player not have any ammunition for the weapon they are trying to shoot with, the error message with the story key `E_ATTACK_NO_AMMUNITION` should be printed instead and the attack fails and does not count as an action. Then print the command prompt again (without any preceding prompt messages), and wait for a new user input. **Important**: Enemies have infinite ammunition. |

> **Note**: Quarterstaffs' attack type changes depending on which player type has equipped them.


#### Damage Types
Different weapons deal different types of damage. The possible damage types are:
- Physical
- Fire
- Cold
- Force
- Acid

Some characters are affected differently by some damage types.

> **Note**: Quarterstaffs' damage type changes depending on which player type has equipped them.

#### Damage Patterns

Each weapon deals damage in specific area of the room. This can be a single field (only the field targeted by the 
`<POSITION>` parameter) or multiple fields (the `<POSITION>` and others around it).

As long as the targeted position is valid, the attack will be performed, even if parts of the damage pattern are outside the room map.

The different damage patterns can be found in the [damage patterns](Damage_Patterns.md) file.

> **Note**: Quarterstaffs' damage pattern changes depending on which player type has equipped them.
> 
#### Damage Amount

Each weapon deals a specific amount of damage. The damage is calculated by a dice roll plus optionally one of the player's stats.

An attack deals the same amount of damage on all fields affected by it, to all characters currently on the affected fields
(players, enemies, and possibly the attacker themselves). If no characters are located in the damage area, nobody receives any
damage, but the attack still counts as an action.

> **Note**: Quarterstaffs' damage amount changes depending on which player type has equipped them.

#### Modifying Damage

Before the damage amount is applied to a character and reduces their health, it can be affected by effects and equipment.

##### Resistances

First, all resistances are taken into account. If a character is **resistant** to a damage type, it receives only 
**50 %** of the damage dealt of this type.

The damage is rounded down to the next integer after applying the modification.

##### Armor

Next, the damage is reduced by the target character's armor value (`AV`). This value represents the amount of damage 
(regardless of what damage type) a character can withstand without losing any health.

Each player type has a basic `AV` stat from the beginning, and armor items also have a certain armor value. The damage
will always be reduced by whichever `AV` is higher, the player's or the equipped armor item's.

Enemies cannot equip armor items, so their basic `AV` is always used to reduce the damage taken.

All characters' armor values can be found in the [character overview](Characters.md).

##### Example

For example, if character A hits character B with an attack worth 10 points of damage, but character B has 2 `AV`,
the attack will reduce character B's health by 8. 

If character B is **resistant** to the damage type of character A's attack, the same attack would do 10 * 50 % = 5 points of damage.
This means, reduced by character B's `AV`, the remaining damage is 3.

#### Reducing the Health of and Defeating Characters

If there is any damage remaining after the above modifications, the target character's health is reduced by this amount.

If the character's current health is 0 or below afterwards, the character is **defeated**. 

A Death Location (`X`) will be created on the defeated character's position. 
For a defeated player, the death location contains all items in the inventory of the player at their time of death 
and all equipped items. For a defeated enemy, the death location contains all items listed as loot in their description in 
the [character overview](Characters.md).

The defeated character will no longer be shown on the room map. A **defeated player character** can no longer be used 
to perform actions in the [Player Phase](Milestone_1.md#player-phase). A **defeated enemy character** can no longer attack or 
move in the [Enemy Phase](#enemy-phase). Defeated characters will stay defeated for the rest of the game, even if the players leave and reenter the room at a later time.

>**Hint**: You will still need defeated player characters for the final scoring.

#### Output
Upon successful command execution, first the following message should be printed:
```
<PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" used "<WEAPON_NAME>" on (<POSITION>) affecting <POSITION_LIST>.\n
```
- `<WEAPON_NAME>` should be replaced by the full name of the equipped weapon.
- `<POSITION_LIST>` should be a sequence containing the positions of *all* fields affected by an attack (the ones marked 
with `<!>` and `<+>` in the [damage patterns](Damage_Patterns.md)), sorted first by row and then by column in **ascending
order** using the following format:

```
(<POSITION>), (<POSITION>), ...
```
> **Note**: The `...` should not be printed and only signifies the possibility of a longer list. After the  last element in the list, no comma and space should be printed.

Then roll the dice for the damage amount and print the dice roll output.

Next, in the same order the positions in `<POSITION_LIST>` are printed, the remaining damage (after all modifications) will be
dealt to characters standing on these respective fields. For each field containing a character the following message should be printed:
```
<AFFECTED_CHARACTER_NAME> loses <LOST_HEALTH> health (<TOTAL_DAMAGE> * <RESISTANCE_MODIFIER> - <AV>).\n
```
- `<AFFECTED_CHARACTER_NAME>` is either the name of the player (`<PLAYER_NAME>`), or the type and id of the enemy (e.g. `Zombie 1`).
- `<TOTAL_DAMAGE>` is the amount of damage dealt by the weapon on a single field
- `<RESISTANCE_MODIFIER>` is `100 %` or `50 %` depending on if the character is resistant to the damage type
- `<AV>` is the character's current armor value
- `<LOST_HEALTH>` is min(`<CURRENT_HEALTH>`, max(0, `<REMAINING_DAMAGE>`))
- `<REMAINING_DAMAGE>` is the result of the damage calculation after applying resistance and armor

If a character is **defeated**, the following message should be printed immediately after the message about that 
character losing health:
```
<AFFECTED_CHARACTER_NAME> was defeated.\n
```
</details>
</details>

<details>
<summary><h2>Enemy Phase</h2></summary>

In the enemy phase **every enemy in the current room** either attacks or moves. Enemies should be handled one after the 
other in alphabetical order and by ascending id.

Enemies will first try to **attack** a player and if that's not possible they will then try to **move**.

**After every `Enemy Phase` the current room is printed as described in Milestone 1.**

<details>
<summary><h3> Attacking a Player</h3></summary>

If an enemy wants to attack one of the player characters, first they must choose a target player 
(the player they want to attack), and afterward select one of their weapons to use.

#### Choose Target Player
Each enemy prioritizes the player character **closest** to them as a possible attack target.

The distance between an enemy and a player is calculated as follows:
```
<DISTANCE> = |<PLAYER_ROW> - <ENEMY_ROW>| + |<PLAYER_COLUMN> - <ENEMY_COLUMN>|
```

If multiple player characters share the same distance to the enemy's position, the player with the **least amount of 
health** should be chosen as the target.

If both of these attributes are the same for multiple players, the target player should be the player first in clockwise
order starting from above the enemy's current position (this is similar to the prioritization in collision handling (see [Milestone 1](Milestone_1.md))).

#### Choose Weapon
Each enemy has a set number of weapons they can attack with, which are listed in the enemy's description in the
[character overview](Characters.md#enemy-characters). Each weapon has an attack type, damage type, damage amount and damage pattern. These are the 
same as when a player uses a weapon of the same name.

Melee weapons are only usable if the target player is in a position adjacent to the enemy. Ranged weapons are always usable,
because they can reach every position in the current room.

To decide which weapon an enemy should use to attack the target player, list only those weapons that are **usable on the 
target player** in the order they appear in the enemy's description. Number those weapons starting with 1. Then, use
Random Number Generation (see [Milestone 1](Milestone_1.md)) to generate a random number between 1 and the number of
usable weapons. The result specifies which weapon will be used.

> **Attention**: Choosing a weapon is not a dice roll, therefore no dice roll output should be printed.

> **Attention**: Always generate a random number to choose a weapon, even when there is only one usable weapon. Otherwise your output will not be comparable with the test system.

If no weapons are currently usable, the enemy will move instead.

#### Execute Attack
The enemy now executes an attack with the chosen weapon on the position of the target player. This works the same as when a 
player makes an attack:
- affected fields are chosen according to the damage pattern of the weapon (the `<!>` of the damage pattern illustration 
  should be placed at the position of the target player)
- the basic damage amount is calculated according to the weapon
- for each character on an affected field (player, enemy or attacker themselves):
  - the damage amount is modified according to the resistances of the affected character
  - the damage amount is modified according to the armor value of the affected character
  - the character's health is reduced by the remaining damage
  - if the character is defeated, it will be removed from the map and a death location created at its position 

> **Attention**: Attacks by enemies do **not use ammunition**. They can be performed as often as needed.

#### Output
The output for an enemy attack is very similar to when a player makes an attack. 

Only the first message is slightly different and has a empty line before:
```
\n
<ENEMY_TYPE_NAME> <ENEMY_ID> [<ENEMY_TYPE_ABBREVIATION><ENEMY_ID>] used "<WEAPON_NAME>" on (<POSITION>) affecting <POSITION_LIST>.\n
```
Then roll the dice for the damage amount and print the dice roll output.

The rest of the output works exactly the same as described in the `attack` command. The following 
messages are used as described there:
```
<AFFECTED_CHARACTER_NAME> loses <LOST_HEALTH> health (<TOTAL_DAMAGE> * <RESISTANCE_MODIFIER> - <AV>).\n
```
```
<AFFECTED_CHARACTER_NAME> was defeated.\n
```
</details>
<details>
<summary><h3> Movement</h3></summary>

If none of the enemy's weapons are currently usable on the target player, the enemy will move instead.

When an enemy is moving, Random Number Generation (see [Milestone 1](Milestone_1.md)) is used to generate a random 
number from 1 to the number of adjacent fields of the respective enemy. The fields are numbered in clockwise order 
starting from the field above the enemy's position, which receives the number 1. Then, the enemy should move to the 
target field corresponding to the generated random number. If the field is occupied by another entity, the enemy will be
placed on an empty field in accordance with the collision handling rule (see [Milestone 1](Milestone_1.md)).

The numbers in the following example are just for illustration, they should not be printed.
```
     1   2   3          1   2   3          1   2   3          1   2   3    
   +---+---+---+      +---+---+---+      +---+---+---+      +---+---+---+
 1 |<1>|<2>|   |    1 |   |   |   |    1 |<8>|<1>|<2>|    1 |   |<5>|<1>|
   +---+---+---+      +---+---+---+      +---+---+---+      +---+---+---+
 2 | Z1|<3>|   |    2 |<1>|<2>|   |    2 |<7>| Z1|<3>|    2 |   |<4>| Z1|
   +---+---+---+      +---+---+---+      +---+---+---+      +---+---+---+
 3 |<5>|<4>|   |    3 | Z1|<3>|   |    3 |<6>|<5>|<4>|    3 |   |<3>|<2>|
   +---+---+---+      +---+---+---+      +---+---+---+      +---+---+---+
```

After the enemy has **moved**, the following message should be printed including an empty line before:
```
\n
<ENEMY_TYPE_NAME> <ENEMY_ID> [<ENEMY_TYPE_ABBREVIATION><ENEMY_ID>] moved to (<POSITION>).\n
```

- Here <POSITION> is the position the enemy ends up in, after collision handling if necessary.

> **Attention**: Choosing where to move is not a dice roll, therefore no dice roll output should be printed.
</details>
</details>

## Final Phase

<details>
<summary><h3> End of Game</h3></summary>

> **Note**: The game can be exited at any point in time using the `quit` command or `EOF`, 
> but this does not count as an ending, as no further output needs to be displayed in this case.

The game can end in three different ways:

|                | Description                                                                            | Story Key      |
|----------------|----------------------------------------------------------------------------------------|----------------|
| **Defeat**     | all player characters run out of health and are defeated                               | `N_DEFEAT`     |
| **Flight**     | the players leave the dungeon via the dungeon entrance/exit                            | `N_FLIGHT`     |
| **Completion** | the [Lich](Characters.md#enemy-characters) (the final boss of the dungeon) is defeated | `N_COMPLETION` |

The game ends immediately when one of these conditions is fulfilled, no matter which phase it is currently in. No further output
from the current phase is printed (e.g. further damage messages, printing the current room).

Which of these outcomes occurs will lead to a different final story message being printed.
The corresponding story keys are listed in the table. 

</details>

<details>
<summary><h3> Scoring </h3></summary>

Regardless of how the game ends, the final score overview should be printed after the final story message.

#### Players
First, print the following header:
```
-- Players --------------------------------------\n
```

Then, in the following format, for each player that was **not defeated**, this message should be printed sorted in player
order by id:
```
  <PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>" survived.\n
```

After the surviving players, the **defeated** players should be listed in the following format sorted in player order by 
id:
```
  <PLAYER_TYPE_NAME> [<PLAYER_TYPE_ABBREVIATION>] "<PLAYER_NAME>", rest in peace.\n
```

#### Statistics
After the player list, print a statistics overview in the following format:
```
\n
-- Statistics -----------------------------------\n
  <NUMBER_OF_COMPLETED_ROOMS> rooms completed\n
  <TOTAL_ACTIONS> performed actions\n
\n
```
- `<NUMBER_OF_COMPLETED_ROOMS>` represents the number of completed rooms.
- `<TOTAL_ACTIONS>` represents the number of performed actions.

</details>

<details>
<summary><h3> Save to File</h3></summary>

The final scoring should now be saved to a file. For this the player is asked to input a filepath with the 
story key `N_SCORING_FILE` followed by the command prompt. The user can now input the filepath of the file the scoring should
be written to. 

If a file with that path ... 
* ... **doesn't exist:** create a new file with the given path
* ... **is not writable:** print the message with the story key `E_SCORING_FILE_NOT_WRITABLE` and print the 
      filename prompt again, until the input is valid

Now the Scoring (Players and Stats) should be written to the file in the same format as described
above. This should always overwrite the existing file content.

_Example file content_:
```
-- Players --------------------------------------\n
  Wizard [W] "Lisa" survived.\n
  Rogue [R] "John", rest in peace.\n
\n
-- Statistics -----------------------------------\n
  5 rooms completed\n
  57 performed actions\n
\n
```

>**Attention**: `quit` and `EOF` are also valid inputs here.

</details>

<details>
<summary><h3> Example Output</h3></summary>

```
\n
** Unfortunately, this adventure has come to an early end!\n
** The adventurers decided to flee the dangers of the dungeon\n
** rather than fight through it!\n
\n
-- Players --------------------------------------\n
  Wizard [W] "Lisa" survived.\n
  Rogue [R] "John", rest in peace.\n
\n
-- Statistics -----------------------------------\n
  5 rooms completed\n
  57 performed actions\n
\n
** Enter the file path of a writable file or\n
** a path for a new file (including the file format e.g. `.txt`).\n
> 
```
</details>

### Program End

After writing the end result to the file, the program should terminate with the return value `0`. All resources should be
freed before termination.

## Full Example Game

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
> B

Player 2 what do you wish to be called? (max length 10 characters)
> Alex
Alex, please choose a player type:
  [W] Wizard     0/1
  [B] Barbarian  1/1
  [R] Rogue      0/1
> W

-- Players --------------------------------------
  Player 1: Barbarian [B] "David"
  Player 2: Wizard [W] "Alex"

** You have entered the dungeon of the evil Lich!
** Help the anxious souls of the nearby village by freeing
** them from the monsters that haunt it!

** Beware, there are zombies in this room!

-- ROOM 1 (0/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   |#D2|
   +---+---+---+
 2 |   | Z1| B |
   +---+---+---+
 3 |   | W |#D0|
   +---+---+---+
   Z1: 15/15

** What do you want to do?
> positions
Barbarian [B] "David" on (2,3)
Wizard [W] "Alex" on (3,2)
Zombie 1 [Z1] on (2,2)

** What do you want to do?
> player W
Wizard [W] "Alex" on (3,2)
  Armor Value:        0
  Current Health:    10
  Max Health:        10
  Strength:           1
  Vitality:           4

** What do you want to do?
> inventory B
Inventory "David"
  Equipped Armor: [-] None
  Equipped Weapon: [GAXE] Greataxe
  Weapons: [HAXE] Handaxe (2)

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (3,2).
[Dice Roll] 1 d12 resulting in a total value of 3.

Zombie 1 loses 6 health (7 * 100 % - 1).
Alex loses 7 health (7 * 100 % - 0).

-- ROOM 1 (0/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   |#D2|
   +---+---+---+
 2 |   | Z1| B |
   +---+---+---+
 3 |   | W |#D0|
   +---+---+---+
   Z1: 9/15

** What do you want to do?
> attack W 2,2
Wizard [W] "Alex" used "Quarterstaff of Force" on (2,2) affecting (2,2).
[Dice Roll] 1 d10 resulting in a total value of 5.

Zombie 1 loses 4 health (5 * 100 % - 1).

-- ROOM 1 (0/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   |#D2|
   +---+---+---+
 2 |   | Z1| B |
   +---+---+---+
 3 |   | W |#D0|
   +---+---+---+
   Z1: 5/15

Zombie 1 [Z1] used "Dagger" on (3,2) affecting (3,2).
[Dice Roll] 1 d4 resulting in a total value of 4.

Alex loses 3 health (4 * 100 % - 0).
Alex was defeated.

-- ROOM 1 (0/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   |#D2|
   +---+---+---+
 2 |   | Z1| B |
   +---+---+---+
 3 |   | X |#D0|
   +---+---+---+
   Z1: 5/15

** What do you want to do?
> attack W 2,2
!! Unavailable entity or item.
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (3,2).
[Dice Roll] 1 d12 resulting in a total value of 6.

Zombie 1 loses 5 health (10 * 100 % - 1).
Zombie 1 was defeated.

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   | D2|
   +---+---+---+
 2 |   | X | B |
   +---+---+---+
 3 |   | X | D0|
   +---+---+---+


** What do you want to do?
> loot B 2,2
** Looting successful! You have received the following items ...
  Weapons: [DAGG] Dagger (1)

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   | D2|
   +---+---+---+
 2 |   |   | B |
   +---+---+---+
 3 |   | X | D0|
   +---+---+---+


** What do you want to do?
> loot B 3,2
** Looting successful! You have received the following items ...
  Weapons: [DAGG] Dagger (1), [QACD] Quarterstaff of Acid (1), [QFRC] Quarterstaff of Force (1)

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T |   | D2|
   +---+---+---+
 2 |   |   | B |
   +---+---+---+
 3 |   |   | D0|
   +---+---+---+


** What do you want to do?
> move B 1,2
Barbarian [B] "David" moved to (1,2).

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |#T | B | D2|
   +---+---+---+
 2 |   |   |   |
   +---+---+---+
 3 |   |   | D0|
   +---+---+---+


** What do you want to do?
> loot B 1,1
** To unlock this chest you need to roll at least 4 to reach the 5 needed.
[Dice Roll] 1 d20 resulting in a total value of 13.

** Looting successful! You have received the following items ...
  Armor: [PARM] Plate Armor (1)
  Weapons: [QCLD] Quarterstaff of Cold (1)
  Consumables: [GHEP] Greater Health Potion (3)

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   | B | D2|
   +---+---+---+
 2 |   |   |   |
   +---+---+---+
 3 |   |   | D0|
   +---+---+---+


** What do you want to do?
> inventory B
Inventory "David"
  Equipped Armor: [-] None
  Equipped Weapon: [GAXE] Greataxe
  Armor: [PARM] Plate Armor (1)
  Weapons: [DAGG] Dagger (2), [HAXE] Handaxe (2), [QACD] Quarterstaff of Acid (1), [QCLD] Quarterstaff of Cold (1), [QFRC] Quarterstaff of Force (1)
  Consumables: [GHEP] Greater Health Potion (3)

** What do you want to do?
> use B PARM

-- ROOM 1 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   | B | D2|
   +---+---+---+
 2 |   |   |   |
   +---+---+---+
 3 |   |   | D0|
   +---+---+---+


** What do you want to do?
> player B
Barbarian [B] "David" on (1,2)
  Armor Value:        8
  Current Health:    13
  Max Health:        13
  Strength:           4
  Vitality:           1

** What do you want to do?
> move 1,3
!! Invalid AMOUNT OF PARAMETERS.
> move B 1,3
Barbarian [B] "David" moved to (1,3).
** Well done, you made it through the first room!
** Let's see if you can master this one as well!

** We have reached the end of the dungeon!
** Kill the Lich and you should all be celebrated as heroes of the dungeon.


-- ROOM 2 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+
   L1: 50/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 10.

Lich 1 loses 8 health (14 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+
   L1: 42/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 10.

Lich 1 loses 8 health (14 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+
   L1: 34/50

Lich 1 [L1] used "Quarterstaff of Force" on (1,3) affecting (1,3).
[Dice Roll] 1 d10 resulting in a total value of 9.

David loses 1 health (9 * 100 % - 8).

-- ROOM 2 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+
   L1: 34/50

** What do you want to do?
> map

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 4.

Lich 1 loses 2 health (8 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 32/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 2.

Lich 1 loses 0 health (6 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 32/50

Lich 1 [L1] used "Quarterstaff of Cold" on (1,3) affecting (1,3).
[Dice Roll] 2 d10 resulting in a total value of 11.

David loses 3 health (11 * 100 % - 8).

-- ROOM 2 (1/2 completed) --------------------

   L1: 32/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 10.

Lich 1 loses 8 health (14 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 24/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 12.

Lich 1 loses 10 health (16 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 14/50

Lich 1 [L1] used "Quarterstaff of Fire" on (1,3) affecting (1,2), (1,3), (2,3).
[Dice Roll] 3 d6 resulting in a total value of 10.

David loses 2 health (10 * 100 % - 8).

-- ROOM 2 (1/2 completed) --------------------

   L1: 14/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 1.

Lich 1 loses 0 health (5 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 14/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 7.

Lich 1 loses 5 health (11 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

   L1: 9/50

Lich 1 [L1] used "Quarterstaff of Cold" on (1,3) affecting (1,3).
[Dice Roll] 2 d10 resulting in a total value of 11.

David loses 3 health (11 * 100 % - 8).

-- ROOM 2 (1/2 completed) --------------------

   L1: 9/50

** What do you want to do?
> map
     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 9.

Lich 1 loses 7 health (13 * 100 % - 6).

-- ROOM 2 (1/2 completed) --------------------

     1   2   3  
   +---+---+---+
 1 |   |#D1| B |
   +---+---+---+
 2 |   | L1|   |
   +---+---+---+
 3 |   |   |   |
   +---+---+---+
   L1: 2/50

** What do you want to do?
> attack B 2,2
Barbarian [B] "David" used "Greataxe" on (2,2) affecting (1,2), (2,2), (2,3).
[Dice Roll] 1 d12 resulting in a total value of 10.

Lich 1 loses 2 health (14 * 100 % - 6).
Lich 1 was defeated.

** Wow, you are all heroes!! You defeated the Lich!
** Go to the village! The people there have organised
** a huge festival to celebrate you!

-- Players --------------------------------------
  Barbarian [B] "David" survived.
  Wizard [W] "Alex", rest in peace.

-- Statistics -----------------------------------
  2 rooms completed
  19 performed actions

** Enter the file path of a writable file or
** a path for a new file (including the file format e.g. `.txt`).
> saves/game_10.txt
```
</details>
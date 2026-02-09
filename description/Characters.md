# Character Overview

## Character Types
In the game two different character types occur.
* [Player Characters](#player-characters)
* [Enemy Characters](#enemy-characters)

All characters have a health value, an armor value, and stats that represent their abilities.

## Health
The current health value represents the amount of damage needed to defeat a character. Health *cannot* be negative, if the 
current health of a character reaches 0, the character is defeated (for details refer to [Milestone 2](Milestone_2.md)). A character's
current health *cannot* exceed its maximum (= initial) value, which is defined in the respective character's description.

Certain [potions](Items.md#potions) exist to replenish a character's health.

## Armor Value
**Abbreviation**: `AV`

This value represents the amount of damage (regardless of what damage type) a character can withstand without losing any health.

Certain [armor items](Items.md#armor-items) exist to increase a character's base `AV`.

## Stats
The following stats represent a character's abilities:

|        | Stat              | Description                                                 |
|--------|-------------------|-------------------------------------------------------------|
| `STR`  | Strength          | This represents the physical capability of the character.   |
| `VIT`  | Vitality          | This represents the health and agility of the character.    |

## Player Characters

### Player Types
At the start of the game, each player chooses a unique player type, 
which provides them with a specialized set of stats and starting equipment.

| Player Type     | Maximum Health | Armor Value | Stats                                | Initial Inventory                                                                                                                                                                                                                                                                  |
|-----------------|----------------|-------------|--------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Barbarian (`B`) | `12 + VIT`     | Base: **2** | Strength: **4**<br>Vitality: **1**   | 1 [Greataxe](Items.md#weapon-items) (equipped)<br>2 [Handaxes](Items.md#weapon-items)                                                                                                                                                               |
| Rogue (`R`)     | `8 + VIT`      | Base: **1** | Strength: **2**<br>Vitality: **3**   | 1 [Leather Armor](Items.md#armor-items) (equipped)<br>2 [Daggers](Items.md#weapon-items)<br>1 [Rapier](Items.md#weapon-items) (equipped)<br>1 [Shortbow](Items.md#weapon-items)<br>20 [Arrows](Items.md#ammunition) |
| Wizard (`W`)    | `6 + VIT`      | Base: **0** | Strength: **1**<br>Vitality: **4**   | 1 [Quarterstaff of Force](Items.md#weapon-items) (equipped)<br>1 [Quarterstaff of Acid](Items.md#weapon-items)<br>1 [Dagger](Items.md#weapon-items)                                                                                                 |

## Enemy Characters

On their turn in the `Enemy Phase`, all enemies will attack if possible, and otherwise move.

| Enemy Type     | Maximum Health | Armor Value | Stats                                | Special Properties                           | Weapons                                                                                               | Loot                                                                                                           |
|----------------|----------------|-------------|--------------------------------------|----------------------------------------------|-------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| Zombie (`Z`)   | **15**         | Base: **1** | Strength: **3**<br>Vitality: **0**   | *(no special properties)*                    | *Dagger*                                                                                              | 1 [Dagger](Items.md#weapon-items)                                                                              |
| Goblin (`G`)   | **10**         | Base: **4** | Strength: **3**<br>Vitality: **4**   | *(no special properties)*                    | *Handaxe*<br>*Shortbow*                                                                               | 1 [Handaxe](Items.md#weapon-items)<br>1 [Shortbow](Items.md#weapon-items)<br>3 [Arrows](Items.md#ammunition)   |
| Lich (`L`)     | **50**         | Base: **6** | Strength: **2**<br>Vitality: **8**   | The Lich is resistant to damage type `Cold`. | *Quarterstaff of Fire*<br>*Quarterstaff of Cold*<br>*Quarterstaff of Acid*<br>*Quarterstaff of Force* | *(no loot)*                                                                                                    |

> **Note**: Enemies have **infinite** ammunition. Any ammunition listed here is *only* used as loot.

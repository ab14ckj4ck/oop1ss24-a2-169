# Items

In game the following item types can occur:
* [Consumable Items](#consumable-items)
* [Armor Items](#armor-items)
* [Weapon Items](#weapon-items)

## Consumable Items
These items can only be **used once** and disappear from the game after use.

### Potions
Potions are used via the `use` command.

#### Health Potions
_Effect:_ Regenerates health of the user.

| Abbreviation | Potion                          | Regenerated Health |
|--------------|---------------------------------|--------------------|
| `NHEP`       | **Normal Health Potion**        | `2 d4`             |
| `GHEP`       | **Greater Health Potion**       | `4 d4`             |
| `SHEP`       | **Superior Health Potion**      | `8 d4`             |

#### Potions of Resistance
_Effect:_ Makes the user resistant to a specific damage type .\
_Duration:_ This effect ends when the players leave the room.

| Abbreviation | Potion                            | Damage type |
|--------------|-----------------------------------|-------------|
| `FIRS`       | **Potion of Fire Resistance**     | `Fire`      |
| `CORS`       | **Potion of Cold Resistance**     | `Cold`      |
| `FORS`       | **Potion of Force Resistance**    | `Force`     |
| `ACRS`       | **Potion of Acid Resistance**     | `Acid`      |


### Ammunition

Ammunition is used whenever a player uses the `attack` command and has one of the weapons listed below equipped.

| Abbreviation | Ammunition Type                | Weapons                                          |
|--------------|--------------------------------|--------------------------------------------------|
| `ARRW`       | **Arrow**                      | Shortbow (`SBOW`), Longbow (`LBOW`)              |
| `BOLT`       | **Bolt**                       | Light Crossbow (`LCRS`), Heavy Crossbow (`HCRS`) |


## Armor Items
Armor can be equipped via the `use` command. It is used to reduce the damage taken from an attack.

| Abbreviation | Armor Item        | Armor Value       |
|--------------|-------------------|-------------------|
| `LARM`       | **Leather Armor** | `1 + VIT`         |
| `BPLT`       | **Breastplate**   | `4 + min(VIT, 2)` |
| `CHML`       | **Chain Mail**    | `6`               |
| `PARM`       | **Plate Armor**   | `8`               |


## Weapon Items
Weapons can be equipped via the `use` command. They are used for attacks.

| Abbreviation | Weapon                         | Attack Type                                          | Damage Type                                            | Damage Pattern                                                                                         | Damage Amount                                               |
|--------------|--------------------------------|------------------------------------------------------|--------------------------------------------------------|--------------------------------------------------------------------------------------------------------|-------------------------------------------------------------|
| `DAGG`       | **Dagger**                     | Melee                                                | Physical                                               | [Hit](Damage_Patterns.md#hit)                                                                          | `1 d4 + VIT`                                                |
| `MACE`       | **Mace**                       | Melee                                                | Physical                                               | [Hit](Damage_Patterns.md#hit)                                                                          | `1 d6 + STR`                                                |
| `WARH`       | **Warhammer**                  | Melee                                                | Physical                                               | [Hit](Damage_Patterns.md#hit)                                                                          | `1 d10 + STR`                                               |
| `HAXE`       | **Handaxe**                    | Melee                                                | Physical                                               | [Hit](Damage_Patterns.md#hit)                                                                          | `1 d6 + STR`                                                |
| `GAXE`       | **Greataxe**                   | Melee                                                | Physical                                               | [Slash](Damage_Patterns.md#slash)                                                                      | `1 d12 + STR`                                               |
| `RAPI`       | **Rapier**                     | Melee                                                | Physical                                               | [Thrust](Damage_Patterns.md#thrust)                                                                    | `1 d8 + VIT`                                                |
| `SSWD`       | **Shortsword**                 | Melee                                                | Physical                                               | [Thrust](Damage_Patterns.md#thrust)                                                                    | `1 d6 + VIT`                                                |
| `LSWD`       | **Longsword**                  | Melee                                                | Physical                                               | [Slash](Damage_Patterns.md#slash)                                                                      | `1 d10 + STR`                                               |
| `GSWD`       | **Greatsword**                 | Melee                                                | Physical                                               | [Slash](Damage_Patterns.md#slash)                                                                      | `2 d6 + STR`                                                |
| `SBOW`       | **Shortbow**                   | Ranged                                               | Physical                                               | [Shot](Damage_Patterns.md#shot)                                                                        | `1 d6 + VIT`                                                |
| `LBOW`       | **Longbow**                    | Ranged                                               | Physical                                               | [Shot](Damage_Patterns.md#shot)                                                                        | `1 d8 + VIT`                                                |
| `LCRS`       | **Light Crossbow**             | Ranged                                               | Physical                                               | [Shot](Damage_Patterns.md#shot)                                                                        | `1 d8 + VIT`                                                |
| `HCRS`       | **Heavy Crossbow**             | Ranged                                               | Physical                                               | [Shot](Damage_Patterns.md#shot)                                                                        | `1 d10 + VIT`                                               |
| `QFIR`       | **Quarterstaff of Fire**       | Ranged (Wizard, Lich),<br/> Melee (other characters) | Fire (Wizard, Lich),<br/> Physical (other character)   | [Burst](Damage_Patterns.md#burst) (Wizard, Lich),<br/> [Hit](Damage_Patterns.md#hit) (other character) | `3 d6` (Wizard, Lich),<br/> `1 d6 + STR` (other character)  |
| `QCLD`       | **Quarterstaff of Cold**       | Melee (all characters)                               | Cold (Wizard, Lich),<br/> Physical (other character)   | [Line](Damage_Patterns.md#line) (Wizard, Lich),<br/> [Hit](Damage_Patterns.md#hit) (other character)   | `2 d10` (Wizard, Lich),<br/> `1 d6 + STR` (other character) |
| `QACD`       | **Quarterstaff of Acid**       | Ranged (Wizard, Lich),<br/> Melee (other characters) | Acid (Wizard, Lich),<br/> Physical (other character)   | [Shot](Damage_Patterns.md#shot) (Wizard, Lich),<br/> [Hit](Damage_Patterns.md#hit) (other character)   | `1 d10` (Wizard, Lich),<br/> `1 d6 + STR` (other character) |
| `QFRC`       | **Quarterstaff of Force**      | Melee (all characters)                               | Force (Wizard, Lich),<br/> Physical (other character)  | [Hit](Damage_Patterns.md#hit) (all characters)                                                         | `1 d10` (Wizard, Lich),<br/> `1 d6 + STR` (other character) |

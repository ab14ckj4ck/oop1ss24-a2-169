# oop1ss24-a2-169

# Assignment 2 - Dungeons & Classes

## Introduction
In Assignment 2 (A2) of the OOP1 design practicals (KU), you will implement a game which is loosely inspired by
the tabletop role-playing game [Dungeons & Dragons](https://en.wikipedia.org/wiki/Dungeons_%26_Dragons).
Please take care to follow the assignment description exactly, even if you know the original game, because OOP1 _Dungeons
& Classes_ uses different rules than Dungeons & Dragons.

This document describes the general principle of the game.

Assigment 2 consists of three parts. We recommend completing the parts in order and finishing one before
starting the next, because each part builds on the previous one.

- [Design Document - Description of Approach](description/Design_Document.md)
- [Milestone 1 - Basic Structure and Output](description/Milestone_1.md)
- [Milestone 2 - Game Logic](description/Milestone_2.md)

## Learning Goals
- Classes and Objects
- Inheritance
- const
- Strings and Streams

## Specifications
Some general specifications must be followed for every assignment in this course. A list of these specifications can be
found here: [Assessment of Assignments - Deductions]

Please follow these specifications to avoid point deductions.

Additionally, the following specifications must be followed for this assignment (A2) as well:

- The provided `Random` class must not be modified.

If this specification is not followed, this may result in a deduction of up to -25 %.

## Grading
Assignment 2 is worth **60 points**.

**Attention:** To receive a positive grade on the OOP1 KU **at least half of all test cases for A2 have to be
passed** (among other conditions).

**Attention:** To be able to do A3, **at least 66 % of all test cases for A2 have to be passed** by the deadline, because A3 builds on A2.
If your program does not fulfill this requirement, you will need to spend the time while other groups are working on A3 on
correcting your A2, and will not be allowed to do A3 (see also this [Overview Picture]

Information on how your assignment submission will be assessed can be found
here: [Assessment of Assignments]

Here you can find an overview of how your total grade for the KU will be
calculated: [Procedure and Grading]

To get points on A2, you will need to attend an assignment interview.

### Points for Effective C++

Some of the points for this assignment can only be achieved if you use certain C++ concepts in your implementation. 
Information about this can be found here: [Effective C++ Points]

## Submission

### Design Document (**mandatory**)
- Push to your team's repository **on the `main` branch**
- Deliverables: a pdf of your [Design Document](description/Design_Document.md)
- **Deadline: 24.04.2024, 23:59 (GMT+1)**
- There will be discussion rounds with your tutor and other teams to discuss your design. Participating in a discussion
  round is **mandatory** to receive a positive grade. Your tutor will provide timeslots for your team to sign up for a
  discussion round. All design discussions will take place between 26.04.2024 and 03.05.2024.

### Program (**mandatory**)

- Push to your team's repository **on a branch called`submission`**
- Deliverables: all .cpp and .hpp files needed to run your program
- **Deadline: 22.05.2024, 23:59 (GMT+1)**
- **Assignment 2 is group work!** Try to keep the division of labor as even as possible. If a team member does not contribute sufficiently
  to the project, they may get deductions. Every team member should make their own commits so that we are able to see who implemented what.

## Assignment Description

### General Notes
The notes given in this section are important for the entire assignment description.

- `\n` shouldn't be printed as two separate characters, instead they represent the newline character.
- Note the leading and trailing spaces in the text fields. You can make them visible by selecting the text field.
- Words in uppercase letters in angle brackets (like `<PLAYER_ID>`) should be replaced by calculated or given data.  
  They are *not* part of the output itself!
- Additional **whitespaces** should be ignored
  - before and after user input
  - between command and parameters
  - between parameters
- User inputs are **case-insensitive**. This means that `quit`, `Quit` and `QUIT` are all valid inputs.
- `>` (command prompt) in the output indicates that the program waits for a user input at this point. 
  **The command prompt is always followed by a single space.**
  Execution should only continue after the user has ended the input with `ENTER`.
- If the user's input is invalid, print the corresponding error message and print the command prompt again, so that the user can 
  enter a new input. Repeat this process as long as the input is invalid.
- If `quit` or `EOF` is entered (End of File, not the string "EOF"), the program should terminate with the return value 0.
  This should be possible any time the program waits for user input, except where noted otherwise.
- The program needs to be able to handle arbitrary input in any form (e.g. strings when integers are expected).

### The Game
The following is just a quick overview of the basic elements of the game, a more detailed description can be found in 
[Milestone 1 - Basic Structure and Output](description/Milestone_1.md) and [Milestone 2 - Game Logic](description/Milestone_2.md).

In this game, 1 to 3 players explore a dungeon that consists of interconnected rooms. Along the way they have to fight
enemies and can find helpful items.

>**Note**: The game will probably not be amazingly balanced, since this is a programming course and not a game design course.
> If we were making a real game, there would of course be playtesting and adjustments to make it more balanced.

#### Start of the Game
At the beginning of the game each player chooses one of the following three [player types](description/Characters.md#player-types). Depending on the chosen type 
each player has different stats and base equipment.

* Barbarian
* Rogue
* Wizard

#### Playing the Game
The game consists of multiple rounds of two alternating phases, the `Player Phase` and the `Enemy Phase`.

In the `Player Phase` the players can perform **actions**. When the total number of player actions in this phase 
has reached the number of players, the `Player Phase` ends. If the room the players are currently in contains enemies, 
this is followed by the `Enemy Phase` in which every enemy in the current room either **attacks** or **moves**.

Then the next round starts with the next `Player Phase`. This keeps repeating until the Dungeon Boss (Lich) is killed, the players leave the Dungeon via the
entrance/exit, or all players are defeated.

#### Goal of the Game

The main goal of the game is to defeat the [dungeon boss](description/Characters.md#enemy-characters) and to complete as 
many rooms as possible along your journey. A **room** counts as **completed** as soon as there are no enemies left in it.

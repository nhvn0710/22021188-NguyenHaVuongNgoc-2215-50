# 221550

# Honehoover

Honehoover is a Minesweeper-inspired game implemented in C++ using the SDL2 library. 
This unique take on the classic game offers multiple difficulty levels and a customizable game board.


## Features

- Graphical user interface with SDL2
- Multiple default difficulty levels: Easy, Medium, Hard, and Very Hard
- Tutorial mode
- Pity mode with extra lives
- Customizable difficulty with a slider
- High score tracking
- Timer and flag counter
- Background music and sound effects
- Two different texture packs with unique styles
- Adjustable setting


## Dependencies

Built and complied in Visual Studio using:
- SDL2
- SDL2_ttf
- SDL2_image
- SDL2_mixer
- and other standard C++ libraries

## Building the Game

To build the game, you'll need to have the SDL2 libraries installed on your system. 
Use your preferred C++ compiler to compile all the `.h` and `.cpp` files, then link against the SDL2 libraries.


## How to Play

1. Start the game and select your desired difficulty level using the slider or preset buttons.
2. Click "Start Game" to begin.
3. Left-click to reveal cells, right-click to place or remove flags.
4. Clear all non-mine cells to win the game.
5. If you click on a mine, the game ends.


## Game Controls

- Left Mouse Button: Reveal cell
- Right Mouse Button: Place/remove flag
- Back Button: Return to main menu


## File Structure

- `main.cpp`: Entry point of the game
- `game.cpp` / `game.h`: Main game logic and state management
- `board.cpp` / `board.h`: Game board implementation
- `cell.cpp` / `cell.h`: Individual cell logic
- `timer.cpp` / `timer.h`: Game timer implementation
- `render.cpp`: Rendering functions
- `scores.cpp`: High score management
- `getset.cpp`: Getter and setter functions


## Customization

You can customize the game by modifying the values of the game at `game.h`,
or change difficulty levels in the `Game::difficulties` array in `getset.cpp`.


## High Scores

High scores are saved in a file named `highscores.txt` in the same directory as the executable.


## Credits

This game was created using SDL2 and its extension libraries. 
Some of the game assets (images and sounds) used are from freesound.org and craftpix.net.

## Preview

![Screenshot](Honehoover/resources/image/Screenshot01.png)

![Screenshot](Honehoover/resources/image/Screenshot02.png)

![Screenshot](Honehoover/resources/image/Screenshot03.png)

![Screenshot](Honehoover/resources/image/Screenshot04.png)

![Screenshot](Honehoover/resources/image/Screenshot05.png)


## License

github.com/nhvn0710


*Enjoy playing Honehoover!*
# Minesweeper

A modern C++ implementation of the classic Minesweeper game. Test your logic by navigating a hidden minefield using a command-line interface with color support and robust gameplay mechanics.

## Features
- **Difficulty Levels:**
  - Beginner: 9x9 grid with 10 mines.
  - Intermediate: 16x16 grid with 40 mines.
  - Advanced: 24x24 grid with 99 mines.

- **Gameplay Mechanics:**
  - **First Move Safety:** Your first move is guaranteed to be safe; mines are placed after your first reveal.
  - **Flagging System:** Mark suspected mines to keep track of the board.
  - **Recursive Reveal:** Clearing an empty cell automatically reveals all adjacent safe cells.
  - **Visual Feedback:** ANSI colors highlight different numbers and mine locations for better readability.
  - **Play Again Loop:** Start a new game immediately after finishing a session.

## How to Play
1. **Compile the Game:**
   Use a C++11 compatible compiler:
   ```bash
   g++ -std=c++11 main.cpp -o minesweeper
   ```
2. **Select Difficulty:** Choose between Beginner (0), Intermediate (1), or Advanced (2).
3. **Enter Commands:**
   - To reveal a cell: `r row column` (e.g., `r 3 4`)
   - To flag/unflag a cell: `f row column` (e.g., `f 3 4`)
4. **Win or Lose:** Successfully reveal all safe cells to win, or hit a mine to lose.

Challenge yourself to navigate the minefield and win the game!

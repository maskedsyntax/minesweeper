#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

class Minesweeper {
private:
    int side;
    int mines_count;
    vector<vector<char>> real_board;
    vector<vector<char>> player_board;
    vector<pair<int, int>> mine_positions;
    int flags_used;

    bool is_valid(int row, int col) const {
        return (row >= 0) && (row < side) && (col >= 0) && (col < side);
    }

    bool is_mine(int row, int col) const {
        return real_board[row][col] == '*';
    }

    int count_adjacent_mines(int row, int col) const {
        int count = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                if (is_valid(row + dx, col + dy) && is_mine(row + dx, col + dy)) {
                    count++;
                }
            }
        }
        return count;
    }

    void reveal_mines() {
        for (const auto& pos : mine_positions) {
            player_board[pos.first][pos.second] = '*';
        }
    }

    bool play_util(int row, int col, int& moves_left) {
        if (player_board[row][col] != '-' && player_board[row][col] != 'F') return false;
        if (player_board[row][col] == 'F') return false; // Don't reveal flagged cells

        if (real_board[row][col] == '*') {
            reveal_mines();
            print_board();
            cout << RED << BOLD << "\nBOOM! You hit a mine. Game Over!" << RESET << endl;
            return true;
        }

        int count = count_adjacent_mines(row, col);
        moves_left--;
        player_board[row][col] = count + '0';

        if (count == 0) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    if (is_valid(row + dx, col + dy)) {
                        play_util(row + dx, col + dy, moves_left);
                    }
                }
            }
        }
        return false;
    }

    void place_mines(int first_x, int first_y) {
        vector<bool> mark(side * side, false);
        mark[first_x * side + first_y] = true; // Guarantee first move is safe

        for (int i = 0; i < mines_count; ) {
            int random = rand() % (side * side);
            int x = random / side;
            int y = random % side;

            if (!mark[random]) {
                mine_positions.push_back({x, y});
                real_board[x][y] = '*';
                mark[random] = true;
                i++;
            }
        }
    }

    string get_cell_color(char c) const {
        switch (c) {
            case '1': return BLUE;
            case '2': return GREEN;
            case '3': return RED;
            case '4': return MAGENTA;
            case '5': return YELLOW;
            case '6': return CYAN;
            case '7': return WHITE;
            case '8': return BOLD;
            case '*': return RED;
            case 'F': return YELLOW;
            default: return RESET;
        }
    }

public:
    Minesweeper(int s, int m) : side(s), mines_count(m), flags_used(0) {
        real_board.assign(side, vector<char>(side, '-'));
        player_board.assign(side, vector<char>(side, '-'));
        srand(time(nullptr));
    }

    void print_board() const {
        cout << "\n    ";
        for (int i = 0; i < side; i++) cout << setw(2) << i << " ";
        cout << "\n   " << string(side * 3 + 1, '-') << "\n";
        for (int i = 0; i < side; i++) {
            cout << setw(2) << i << " | ";
            for (int j = 0; j < side; j++) {
                char c = player_board[i][j];
                if (c == '0') cout << "  " << " ";
                else cout << get_cell_color(c) << c << RESET << "  ";
            }
            cout << "|\n";
        }
        cout << "   " << string(side * 3 + 1, '-') << "\n";
        cout << "Mines: " << mines_count << " | Flags: " << flags_used << endl;
    }

    void play() {
        int moves_left = side * side - mines_count;
        int current_move_index = 0;
        bool game_over = false;

        while (!game_over) {
            print_board();
            cout << "Enter command (r row col / f row col): ";
            char command;
            int x, y;
            if (!(cin >> command >> x >> y)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Format: r 1 2 (reveal) or f 1 2 (flag)\n";
                continue;
            }

            if (!is_valid(x, y)) {
                cout << "Out of bounds!\n";
                continue;
            }

            if (command == 'f' || command == 'F') {
                if (player_board[x][y] == '-') {
                    player_board[x][y] = 'F';
                    flags_used++;
                } else if (player_board[x][y] == 'F') {
                    player_board[x][y] = '-';
                    flags_used--;
                } else {
                    cout << "Cannot flag a revealed cell!\n";
                }
                continue;
            }

            if (command != 'r' && command != 'R') {
                cout << "Unknown command! Use 'r' to reveal or 'f' to flag.\n";
                continue;
            }

            if (player_board[x][y] == 'F') {
                cout << "Unflag the cell first!\n";
                continue;
            }

            if (player_board[x][y] != '-') {
                cout << "Cell already revealed!\n";
                continue;
            }

            if (current_move_index == 0) {
                place_mines(x, y);
            }

            current_move_index++;
            game_over = play_util(x, y, moves_left);

            if (!game_over && moves_left == 0) {
                cout << GREEN << BOLD << "\nCongratulations! You cleared the minefield!" << RESET << endl;
                reveal_mines();
                print_board();
                game_over = true;
            }
        }
    }
};

bool play_again() {
    char choice;
    cout << "\nWould you like to play again? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

void choose_difficulty(int& side, int& mines) {
    int level;
    cout << "\nChoose Difficulty Level:\n";
    cout << "0 - " << GREEN << "BEGINNER" << RESET << " (9x9, 10 Mines)\n";
    cout << "1 - " << YELLOW << "INTERMEDIATE" << RESET << " (16x16, 40 Mines)\n";
    cout << "2 - " << RED << "ADVANCED" << RESET << " (24x24, 99 Mines)\n";
    cout << "Selection: ";
    if (!(cin >> level)) {
        cin.clear();
        cin.ignore(1000, '\n');
        level = 0;
    }

    if (level == 0) { side = 9; mines = 10; }
    else if (level == 1) { side = 16; mines = 40; }
    else { side = 24; mines = 99; }
}

int main() {
    do {
        int side, mines;
        choose_difficulty(side, mines);
        Minesweeper game(side, mines);
        game.play();
    } while (play_again());

    cout << "\nThanks for playing!\n";
    return 0;
}

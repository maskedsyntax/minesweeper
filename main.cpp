#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

using namespace std;

enum class Difficulty {
    BEGINNER,
    INTERMEDIATE,
    ADVANCED
};

class Minesweeper {
private:
    int side;
    int mines_count;
    vector<vector<char>> real_board;
    vector<vector<char>> player_board;
    vector<pair<int, int>> mine_positions;

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
        if (player_board[row][col] != '-') return false;

        if (real_board[row][col] == '*') {
            reveal_mines();
            print_board();
            cout << "\nYou lost!\n";
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

    void place_mines() {
        vector<bool> mark(side * side, false);
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

    void replace_mine(int row, int col) {
        for (int i = 0; i < side; i++) {
            for (int j = 0; j < side; j++) {
                if (real_board[i][j] != '*') {
                    real_board[i][j] = '*';
                    real_board[row][col] = '-';
                    // Update mine_positions
                    for (auto& pos : mine_positions) {
                        if (pos.first == row && pos.second == col) {
                            pos = {i, j};
                            break;
                        }
                    }
                    return;
                }
            }
        }
    }

public:
    Minesweeper(int s, int m) : side(s), mines_count(m) {
        real_board.assign(side, vector<char>(side, '-'));
        player_board.assign(side, vector<char>(side, '-'));
        srand(time(nullptr));
    }

    void print_board() const {
        cout << "\n  ";
        for (int i = 0; i < side; i++) cout << setw(2) << i;
        cout << "\n";
        for (int i = 0; i < side; i++) {
            cout << setw(2) << i << " ";
            for (int j = 0; j < side; j++) {
                cout << player_board[i][j] << " ";
            }
            cout << "\n";
        }
    }

    void play() {
        place_mines();
        int moves_left = side * side - mines_count;
        int current_move_index = 0;
        bool game_over = false;

        while (!game_over) {
            cout << "\nCurrent Status of Board : \n";
            print_board();

            int x, y;
            cout << "Enter your move, (row, column) -> ";
            if (!(cin >> x >> y)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter numbers.\n";
                continue;
            }

            if (!is_valid(x, y)) {
                cout << "Invalid coordinates!\n";
                continue;
            }

            if (current_move_index == 0 && is_mine(x, y)) {
                replace_mine(x, y);
            }

            current_move_index++;
            game_over = play_util(x, y, moves_left);

            if (!game_over && moves_left == 0) {
                cout << "\nYou won!\n";
                reveal_mines();
                print_board();
                game_over = true;
            }
        }
    }
};

void choose_difficulty(int& side, int& mines) {
    int level;
    cout << "Enter the Difficulty Level\n";
    cout << "0 for BEGINNER (9x9, 10 Mines)\n";
    cout << "1 for INTERMEDIATE (16x16, 40 Mines)\n";
    cout << "2 for ADVANCED (24x24, 99 Mines)\n";
    cin >> level;

    if (level == 0) { side = 9; mines = 10; }
    else if (level == 1) { side = 16; mines = 40; }
    else { side = 24; mines = 99; }
}

int main() {
    int side, mines;
    choose_difficulty(side, mines);
    Minesweeper game(side, mines);
    game.play();
    return 0;
}

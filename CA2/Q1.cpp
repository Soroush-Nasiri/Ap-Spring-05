#include <iostream>
using namespace std;

bool hasInitialConflict(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        bool seen[10] = {false};
        for (int j = 0; j < 9; j++) {
            int val = board[i][j];
            if (val != 0) {
                if (seen[val]) return true;
                seen[val] = true;
            }
        }
    }

    for (int j = 0; j < 9; j++) {
        bool seen[10] = {false};
        for (int i = 0; i < 9; i++) {
            int val = board[i][j];
            if (val != 0) {
                if (seen[val]) return true;
                seen[val] = true;
            }
        }
    }

    for (int br = 0; br < 3; br++) {
        for (int bc = 0; bc < 3; bc++) {
            bool seen[10] = {false};
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int val = board[br*3+i][bc*3+j];
                    if (val != 0) {
                        if (seen[val]) return true;
                        seen[val] = true;
                    }
                }
            }
        }
    }
    return false;
}

void readBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            cin >> board[i][j];
}

void printBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j];
            if (j < 8) cout << ' ';
        }
        cout << '\n';
    }
}

bool isValid(int board[9][9], int row, int col, int num) {
    for (int j = 0; j < 9; j++)
        if (board[row][j] == num) return false;
    for (int i = 0; i < 9; i++)
        if (board[i][col] == num) return false;
    int sr = row / 3 * 3;
    int sc = col / 3 * 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[sr + i][sc + j] == num) return false;
    return true;
}

bool solve(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValid(board, i, j, num)) {
                        board[i][j] = num;
                        if (solve(board)) return true;
                        board[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

int main() {
    int board[9][9];
    readBoard(board);
    if (hasInitialConflict(board)) {
        cout << "No solution exists\n";
        return 0;
    }
    if (solve(board))
        printBoard(board);
    else
        cout << "No solution exists\n";
    return 0;
}
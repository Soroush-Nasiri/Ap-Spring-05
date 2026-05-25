import random
import os

def has_conflict(board):
    for i in range(9):
        seen = set()
        for j in range(9):
            val = board[i][j]
            if val != 0:
                if val in seen:
                    return True
                seen.add(val)
    for j in range(9):
        seen = set()
        for i in range(9):
            val = board[i][j]
            if val != 0:
                if val in seen:
                    return True
                seen.add(val)
    for br in range(3):
        for bc in range(3):
            seen = set()
            for i in range(3):
                for j in range(3):
                    val = board[br*3+i][bc*3+j]
                    if val != 0:
                        if val in seen:
                            return True
                        seen.add(val)
    return False

def generate_board_with_conflict():
    board = [[0]*9 for _ in range(9)]
    for _ in range(random.randint(20, 40)):
        r, c = random.randint(0,8), random.randint(0,8)
        num = random.randint(1,9)
        old = board[r][c]
        board[r][c] = num
        if has_conflict(board):
            board[r][c] = old

    conflict_type = random.choice(['row', 'col', 'box'])
    if conflict_type == 'row':
        row = random.randint(0,8)
        num = random.randint(1,9)
        col1 = random.randint(0,8)
        col2 = random.randint(0,8)
        while col2 == col1:
            col2 = random.randint(0,8)
        board[row][col1] = num
        board[row][col2] = num
    elif conflict_type == 'col':
        col = random.randint(0,8)
        num = random.randint(1,9)
        row1 = random.randint(0,8)
        row2 = random.randint(0,8)
        while row2 == row1:
            row2 = random.randint(0,8)
        board[row1][col] = num
        board[row2][col] = num
    else:  # box conflict
        br = random.randint(0,2)
        bc = random.randint(0,2)
        num = random.randint(1,9)
        cells = [(br*3 + i, bc*3 + j) for i in range(3) for j in range(3)]
        (r1,c1), (r2,c2) = random.sample(cells, 2)
        board[r1][c1] = num
        board[r2][c2] = num
    if not has_conflict(board):
        return generate_board_with_conflict()
    return board

def save_board(board, filename):
    with open(filename, 'w', encoding='utf-8') as f:
        for i in range(9):
            f.write(' '.join(str(board[i][j]) for j in range(9)) + '\n')

def main():
    out_dir = "sudoku_invalid_inputs"
    os.makedirs(out_dir, exist_ok=True)
    for i in range(1, 101):
        board = generate_board_with_conflict()
        filename = os.path.join(out_dir, f"input{i}.txt")
        save_board(board, filename)
        if i % 10 == 0:
            print(f"Generated {i} files...")
    print("\n✅ 100 test files with initial contradictions created in folder 'sudoku_invalid_inputs'")
    print("   All of them will cause 'No solution exists' from a standard Sudoku solver.")

if __name__ == "__main__":
    main()
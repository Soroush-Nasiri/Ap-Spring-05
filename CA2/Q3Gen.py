import random
import os

def write_board(board, filepath):
    with open(filepath, 'w') as f:
        for row in board:
            f.write(''.join(row) + '\n')

def random_board(obstacle_prob):
    board = []
    for _ in range(8):
        row = []
        for _ in range(8):
            if random.random() < obstacle_prob:
                row.append('*')
            else:
                row.append('.')
        if all(c == '*' for c in row):
            row[random.randint(0, 7)] = '.'
        board.append(row)
    return board

def classic_board():
    """No obstacles – classic 8‑Queens."""
    return [['.'] * 8 for _ in range(8)]

def full_obstacles_board():
    """All cells are obstacles → no solution."""
    return [['*'] * 8 for _ in range(8)]

def one_row_all_obstacles():
    """Row 4 (0‑based) completely blocked."""
    board = [['.'] * 8 for _ in range(8)]
    for c in range(8):
        board[4][c] = '*'
    return board

def one_column_all_obstacles():
    """Column 3 completely blocked."""
    board = [['.'] * 8 for _ in range(8)]
    for r in range(8):
        board[r][3] = '*'
    return board

def main_diagonal_all_obstacles():
    """Main diagonal (r == c) all obstacles."""
    board = [['.'] * 8 for _ in range(8)]
    for i in range(8):
        board[i][i] = '*'
    return board

def anti_diagonal_all_obstacles():
    """Anti‑diagonal (r + c == 7) all obstacles."""
    board = [['.'] * 8 for _ in range(8)]
    for r in range(8):
        c = 7 - r
        board[r][c] = '*'
    return board

def knights_move_obstacles():
    """Place obstacles on all knight‑move positions from (0,0) – interesting attack pattern."""
    board = [['.'] * 8 for _ in range(8)]
    moves = [(2,1), (1,2), (-1,2), (-2,1), (-2,-1), (-1,-2), (1,-2), (2,-1)]
    for dr, dc in moves:
        r, c = 0 + dr, 0 + dc
        if 0 <= r < 8 and 0 <= c < 8:
            board[r][c] = '*'
    return board

def unique_solution_board():
    """
    A board that forces exactly one solution (known from N‑Queens with obstacles).
    Here we construct one intentionally: put obstacles everywhere except on one path.
    """
    board = [['*'] * 8 for _ in range(8)]
    solution = [(0,0), (1,4), (2,7), (3,5), (4,2), (5,6), (6,1), (7,3)]
    for r, c in solution:
        board[r][c] = '.'
    extra = [(0,7), (7,0)]
    for r, c in extra:
        if board[r][c] == '*':
            board[r][c] = '.'
    return board

def no_solution_corners_blocked():
    """
    Block the four corners – still a solution exists usually (classic 8‑Queens has many without corners).
    To make it impossible, we block all cells that can ever hold a queen in any solution? 
    Instead, block entire first two rows except one column – that column then forces conflict.
    Simpler: block all cells in rows 0,1,2 except column 0 – then column 0 will have three queens.
    """
    board = [['.'] * 8 for _ in range(8)]
    for r in range(3):
        for c in range(1,8):
            board[r][c] = '*'
    return board

def generate_all_tests():
    if not os.path.exists('in'):
        os.makedirs('in')
    
    generators = [
        classic_board,
        full_obstacles_board,
        one_row_all_obstacles,
        one_column_all_obstacles,
        main_diagonal_all_obstacles,
        anti_diagonal_all_obstacles,
        knights_move_obstacles,
        unique_solution_board,
        no_solution_corners_blocked,
    ]
    
    # Add many random boards with varying obstacle probabilities
    random_probs = [0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5]
    for prob in random_probs:
        # For each probability, generate several random boards (e.g., 4-6)
        for _ in range(5):
            generators.append(lambda p=prob: random_board(p))
    
    tests = []
    
    # 1. Classic (no obstacles)
    tests.append(classic_board)
    
    # 2. Full obstacles
    tests.append(full_obstacles_board)
    
    # 3-8. Hand-crafted edge cases
    tests.append(one_row_all_obstacles)
    tests.append(one_column_all_obstacles)
    tests.append(main_diagonal_all_obstacles)
    tests.append(anti_diagonal_all_obstacles)
    tests.append(knights_move_obstacles)
    tests.append(unique_solution_board)
    tests.append(no_solution_corners_blocked)
    
    probs = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6]
    num_per_prob = (100 - len(tests)) // len(probs) + 1
    for p in probs:
        for _ in range(num_per_prob):
            # Use a lambda that captures p correctly
            tests.append(lambda prob=p: random_board(prob))
    
    # Trim to exactly 100
    tests = tests[:100]
    
    # Generate files
    for i, board_maker in enumerate(tests, start=1):
        board = board_maker()
        filename = f"input{i}.txt"
        filepath = os.path.join('in', filename)
        write_board(board, filepath)
        print(f"Generated {filename}")
    
    print(f"\nDone! Created {len(tests)} test files inside the 'in' folder.")

if __name__ == "__main__":
    # Set seed for reproducibility (optional)
    random.seed(42)
    generate_all_tests()
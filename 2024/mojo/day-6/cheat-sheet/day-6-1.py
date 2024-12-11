import numpy as np
import time

def parse_map(file_path):
    """
    Parses input map file and initializes grid, guard position, and guard direction.
    """
    with open(file_path, 'r') as file:
        lines = [line.strip() for line in file]

    rows = len(lines)
    cols = len(lines[0]) if rows > 0 else 0
    grid = np.array([list(line) for line in lines])

    # Locate guard starting position and direction
    for r in range(rows):
        for c in range(cols):
            if grid[r, c] in "^>v<":
                guard_position = (r, c)
                guard_direction = grid[r, c]
                grid[r, c] = '.'  # Replace with empty space
                return grid, guard_position, guard_direction

    raise ValueError("No guard position found on input map.")

def simulate_guard_path(grid, start_pos, start_dir):
    """
    Simulates guard's patrol path and returns number of distinct visited positions.
    """
    directions = {
        '^': (-1, 0),  # Up
        '>': (0, 1),   # Right
        'v': (1, 0),   # Down
        '<': (0, -1)   # Left
    }
    turn_right = {
        '^': '>',
        '>': 'v',
        'v': '<',
        '<': '^'
    }

    visited_positions = set()
    current_pos = start_pos
    current_dir = start_dir

    rows, cols = grid.shape

    while True:
        # Add current position to visited positions
        visited_positions.add(current_pos)

        # Calculate next position based on current direction
        dr, dc = directions[current_dir]
        next_pos = (current_pos[0] + dr, current_pos[1] + dc)

        # Stop if next position is out of bounds
        if not (0 <= next_pos[0] < rows and 0 <= next_pos[1] < cols):
            break

        # Check for an obstacle at next position
        if grid[next_pos[0], next_pos[1]] == '#':
            # Turn right if an obstacle
            current_dir = turn_right[current_dir]
        else:
            # Moves to next position if clear
            current_pos = next_pos

    return len(visited_positions)

if __name__ == "__main__":
    input_file = "input.txt"  # Path to input file

    start_time = time.time()

    # Parse input map
    grid, start_pos, start_dir = parse_map(input_file)

    # Simulate guard's patrol path
    distinct_positions_count = simulate_guard_path(grid, start_pos, start_dir)

    end_time = time.time()

    print(f"The number of distinct visited positions is: {distinct_positions_count}")
    print(f"Execution time: {end_time - start_time:.4f} seconds")


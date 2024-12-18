// Day 3
use crate::util::grid::*;
use crate::util::parse::*;
use crate::util::point::*;
use std::fs;

pub struct Input {
    grid: Grid<u8>,
    seen: Grid<usize>,
    parts: Vec<u32>,
}

/// Reads input from the file and parses it.
pub fn read_input(file_path: &str) -> Input {
    let input = fs::read_to_string(file_path)
        .expect("Failed to read input file");
    parse(&input)
}

/// Parses the input string into the `Input` structure.
pub fn parse(input: &str) -> Input {
    let grid = Grid::parse(input);
    
    let mut seen: Grid<usize> = grid.same_size_with(0);
    let mut parts = vec![0];
    let mut number = 0;

    for y in 0..grid.height {
        for x in 0..grid.width {
            let p = Point::new(x, y);
            let b = grid[p];

            if b.is_ascii_digit() {
                seen[p] = parts.len();
                number = 10 * number + (b.to_decimal() as u32);
            } else if number > 0 {
                parts.push(number);
                number = 0;
            }
        }
        if number > 0 {
            parts.push(number);
            number = 0;
        }
    }

    Input { grid, seen, parts }
}

/// Sum all part numbers adjacent to at least one symbol.
pub fn part1(input: &Input) -> u32 {
    let Input { grid, seen, parts } = input;
    let mut parts = parts.clone();
    let mut result = 0;

    for y in 0..grid.height {
        for x in 0..grid.width {
            let p = Point::new(x, y);
            let b = grid[p];

            if !b.is_ascii_digit() && b != b'.' {
                for next in DIAGONAL.iter().copied().map(|d| p + d) {
                    let index = seen[next];
                    if index != 0 {
                        result += parts[index];
                        parts[index] = 0;
                    }
                }
            }
        }
    }

    result
}

/// Sum all gears adjacent to exactly two part numbers.
pub fn part2(input: &Input) -> u32 {
    let Input { grid, seen, parts } = input;
    let mut result = 0;

    for y in 0..grid.height {
        for x in 0..grid.width {
            let p = Point::new(x, y);

            if grid[p] == b'*' {
                let mut previous = 0;
                let mut distinct = 0;
                let mut subtotal = 1;

                for next in DIAGONAL.iter().copied().map(|d| p + d) {
                    let index = seen[next];
                    if index != 0 && index != previous {
                        previous = index;
                        distinct += 1;
                        subtotal *= parts[index];
                    }
                }

                if distinct == 2 {
                    result += subtotal;
                }
            }
        }
    }

    result
}

/// Main function to run the solution.
pub fn run() {
    let input = read_input("data/input3.txt");
    
    let result_part1 = part1(&input);
    println!("Part 1: {}", result_part1);

    let result_part2 = part2(&input);
    println!("Part 2: {}", result_part2);
}


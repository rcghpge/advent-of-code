// DAy 17 Clumsy Crucible
use crate::util::grid::*;
use crate::util::parse::*;
use std::fs;

/// Parse input into 2D grid of `u8` then convert to `i32` for convenience.
pub fn parse(input: &str) -> Grid<i32> {
    let Grid { width, height, bytes } = Grid::parse(input);
    let bytes = bytes.iter().map(|b| b.to_decimal() as i32).collect();
    Grid { width, height, bytes }
}

/// Search with maximum of 3 steps in any direction.
pub fn part1(grid: &Grid<i32>) -> i32 {
    astar::<1, 3>(grid)
}

/// Search with minimum of 4 and maximum of 10 steps in any direction.
pub fn part2(grid: &Grid<i32>) -> i32 {
    astar::<4, 10>(grid)
}

/// Optimized A* search using const generics for loop unrolling and efficiency.
fn astar<const L: i32, const U: i32>(grid: &Grid<i32>) -> i32 {
    let size = grid.width;
    let stride = size as usize;
    let heat = &grid.bytes;

    let mut index = 0;
    let mut todo = (0..100).map(|_| Vec::with_capacity(1000)).collect::<Vec<_>>();
    let mut cost = vec![[i32::MAX; 2]; heat.len()];

    // Start from top-left corner checking vertical and horizontal directions.
    todo[0].push((0, 0, 0));
    todo[0].push((0, 0, 1));
    cost[0][0] = 0;
    cost[0][1] = 0;

    loop {
        while let Some((x, y, direction)) = todo[index % 100].pop() {
            let idx = (size * y + x) as usize;
            let steps = cost[idx][direction];

            let heuristic = |x: i32, y: i32, cost: i32| {
                let priority = (2 * size - x - y).min(size + size / 2);
                ((cost + priority) % 100) as usize
            };

            // Check if reached the end.
            if x == size - 1 && y == size - 1 {
                return steps;
            }

            if direction == 0 {
                // Vertical movement: check left and right directions
                let mut next = idx;
                let mut extra = steps;

                for i in 1..=U {
                    if x + i >= size {
                        break;
                    }
                    next += 1;
                    extra += heat[next];
                    if i >= L && extra < cost[next][1] {
                        todo[heuristic(x + i, y, extra)].push((x + i, y, 1));
                        cost[next][1] = extra;
                    }
                }

                next = idx;
                extra = steps;

                for i in 1..=U {
                    if i > x {
                        break;
                    }
                    next -= 1;
                    extra += heat[next];
                    if i >= L && extra < cost[next][1] {
                        todo[heuristic(x - i, y, extra)].push((x - i, y, 1));
                        cost[next][1] = extra;
                    }
                }
            } else {
                // Horizontal movement: check up and down directions
                let mut next = idx;
                let mut extra = steps;

                for i in 1..=U {
                    if y + i >= size {
                        break;
                    }
                    next += stride;
                    extra += heat[next];
                    if i >= L && extra < cost[next][0] {
                        todo[heuristic(x, y + i, extra)].push((x, y + i, 0));
                        cost[next][0] = extra;
                    }
                }

                next = idx;
                extra = steps;

                for i in 1..=U {
                    if i > y {
                        break;
                    }
                    next -= stride;
                    extra += heat[next];
                    if i >= L && extra < cost[next][0] {
                        todo[heuristic(x, y - i, extra)].push((x, y - i, 0));
                        cost[next][0] = extra;
                    }
                }
            }
        }
        index += 1;
    }
}

/// Main entry point for script
pub fn run() {
    // Load input data 
    let input_file = "data/input17.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse input grid
    let grid = parse(&input);

    // Part 1 and Part 2 results
    let part1_result = part1(&grid);
    let part2_result = part2(&grid);

    // Results
    println!("Part 1: {}", part1_result);
    println!("Part 2: {}", part2_result);
}


// Day 14 Parabolic Reflector Dish
use crate::util::utils::compute_hash;
use std::collections::HashMap;
use std::fs;

fn tilt(platform: &mut Vec<Vec<u8>>, dx: i32, dy: i32) {
    let (start_x, start_y) = (if dx == -1 { 1 } else { 0 }, if dy == -1 { 1 } else { 0 });
    let (end_x, end_y) = (
        platform[0].len() - if dx == 1 { 1 } else { 0 },
        platform.len() - if dy == 1 { 1 } else { 0 },
    );

    let mut changed = true;
    while changed {
        changed = false;

        for y in (start_y..end_y).rev() {
            for x in (start_x..end_x).rev() {
                if platform[y][x] == b'O' {
                    let (new_x, new_y) = ((x as i32 + dx) as usize, (y as i32 + dy) as usize);
                    if platform[new_y][new_x] == b'.' {
                        platform[y][x] = b'.';
                        platform[new_y][new_x] = b'O';
                        changed = true;
                    }
                }
            }
        }
    }
}

fn compute_total_load(platform: &[Vec<u8>]) -> u32 {
    platform
        .iter()
        .enumerate()
        .map(|(y, row)| {
            let rounded_rocks = row.iter().filter(|&&c| c == b'O').count();
            (rounded_rocks * (platform.len() - y)) as u32
        })
        .sum()
}

fn cycle(platform: &mut Vec<Vec<u8>>) {
    for (dx, dy) in [(0, -1), (-1, 0), (0, 1), (1, 0)] {
        tilt(platform, dx, dy);
    }
}

fn repeat_cycles(platform: &mut Vec<Vec<u8>>, num_cycles: u32) {
    let mut seen_hashes = HashMap::new();
    let mut index = 0;

    while index < num_cycles {
        let hash = compute_hash(platform);
        if let Some(&prev_index) = seen_hashes.get(&hash) {
            let cycle_length = index - prev_index;
            let remaining_cycles = (num_cycles - index) % cycle_length;
            for _ in 0..remaining_cycles {
                cycle(platform);
            }
            return;
        }

        seen_hashes.insert(hash, index);
        cycle(platform);
        index += 1;
    }
}

pub fn run() {
    // Load input data from file
    let input_file = "data/input14.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse the input into a grid
    let mut platform: Vec<Vec<u8>> = input.lines().map(|line| line.bytes().collect()).collect();

    // Part 1: Compute initial load after one tilt
    tilt(&mut platform, 0, -1);
    let total_load = compute_total_load(&platform);
    println!("Part 1: {}", total_load);

    // Part 2: Compute total load after 1 billion cycles
    repeat_cycles(&mut platform, 1_000_000_000);
    let total_load = compute_total_load(&platform);
    println!("Part 2: {}", total_load);
}


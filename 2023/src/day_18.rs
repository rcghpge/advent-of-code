// Day 18 Lavaduct Lagoon
use crate::util::iter::*;
use crate::util::parse::*;
use crate::util::point::*;
use std::fs;

type Move = (Point, i32);
type Input = (Vec<Move>, Vec<Move>);

pub fn parse(input: &str) -> Input {
    let mut first = Vec::with_capacity(1000);
    let mut second = Vec::with_capacity(1000);

    for [a, b, c] in input.split_ascii_whitespace().chunk::<3>() {
        // Parse Part 1 moves
        let direction = Point::from(a.as_bytes()[0]);
        let amount = b.signed();
        first.push((direction, amount));

        // Parse Part 2 moves
        let direction = match c.as_bytes()[7] {
            b'0' => RIGHT,
            b'1' => DOWN,
            b'2' => LEFT,
            b'3' => UP,
            _ => unreachable!(),
        };
        let hex = &c[2..c.len() - 2];
        let amount = i32::from_str_radix(hex, 16).unwrap();
        second.push((direction, amount));
    }

    (first, second)
}

pub fn part1(input: &Input) -> i64 {
    compute_lava_volume(&input.0)
}

pub fn part2(input: &Input) -> i64 {
    compute_lava_volume(&input.1)
}

/// Calculate lava volume using Pick's theorem.
fn compute_lava_volume(moves: &[Move]) -> i64 {
    let mut position = ORIGIN;
    let mut area = 0i64;
    let mut perimeter = 0i64;

    for &(direction, amount) in moves {
        let previous = position;
        position += direction * amount;
        area += determinant(previous, position);
        perimeter += amount as i64;
    }

    // Pick's theorem: area / 2 + perimeter / 2 + 1
    area / 2 + perimeter / 2 + 1
}

/// Compute determinant of two points to find signed area.
fn determinant(a: Point, b: Point) -> i64 {
    (a.x as i64) * (b.y as i64) - (a.y as i64) * (b.x as i64)
}

pub fn run() {
    // Load input data
    let input_file = "data/input18.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse input
    let parsed_input = parse(&input);

    // Compute results
    let part1_result = part1(&parsed_input);
    let part2_result = part2(&parsed_input);

    // Results
    println!("Part 1: {}", part1_result);
    println!("Part 2: {}", part2_result);
}


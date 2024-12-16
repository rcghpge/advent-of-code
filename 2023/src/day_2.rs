//! Day 2
//! # Cube Conundrum Solver
//! Optimized solution for parsing and solving part 1 and 2 of Cube Conundrum puzzle.

use crate::util::iter::*;
use crate::util::parse::*;
use std::cmp;
use std::fs;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Game {
    pub red: u32,
    pub green: u32,
    pub blue: u32,
}

/// Parse input data into a vector of `Game` instances, calculating maximum values of `red`, `green`, and `blue`.
pub fn parse(input: &str) -> Vec<Game> {
    input
        .lines()
        .map(|line| {
            let mut game = Game { red: 0, green: 0, blue: 0 };

            line.split_ascii_whitespace()
                .collect::<Vec<_>>()
                .chunks(2)
                .skip(1) // Skip metadata (first chunk)
                .for_each(|chunk| {
                    if chunk.len() < 2 {
                        eprintln!("Skipping invalid chunk: {:?}", chunk);
                        return;
                    }

                    // Parse amount
                    let amount = chunk[0].parse::<u32>().unwrap_or(0);

                    // Normalize color by trimming unwanted characters
                    let color = chunk[1].trim_matches(&[';', ',', '.'][..]);

                    match color {
                        "r" | "red" => game.red = cmp::max(game.red, amount),
                        "g" | "green" => game.green = cmp::max(game.green, amount),
                        "b" | "blue" => game.blue = cmp::max(game.blue, amount),
                        _ => eprintln!("Unexpected color: {:?}", chunk[1]),
                    }
                });

            game
        })
        .collect()
}

/// Solve Part 1 by summing IDs of valid games.
pub fn part1(games: &[Game]) -> usize {
    games
        .iter()
        .enumerate()
        .filter_map(|(id, game)| {
            if game.red <= 12 && game.green <= 13 && game.blue <= 14 {
                Some(id + 1) // IDs are 1-based
            } else {
                None
            }
        })
        .sum()
}

/// Solve Part 2 by summing product of maximum red, green, and blue values for all games.
pub fn part2(games: &[Game]) -> u32 {
    games
        .iter()
        .map(|game| game.red * game.green * game.blue)
        .sum()
}

/// Run Day 2 code:
pub fn run() {
    // Read input from `data/input2.txt`
    let input_path = "data/input2.txt";
    let input = fs::read_to_string(input_path)
        .expect("Failed to read input file");

    // Parse the input into games
    let games = parse(&input);

    // Solve Part 1
    let result1 = part1(&games);
    println!("Part 1: {}", result1);

    // Solve Part 2
    let result2 = part2(&games);
    println!("Part 2: {}", result2);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse() {
        let input = "1 5 r 10 green, 8 blue;\n2 12 red; 11 green 9 blue";
        let games = parse(input);
        assert_eq!(
            games,
            vec![
                Game { red: 5, green: 10, blue: 8 },
                Game { red: 12, green: 11, blue: 9 },
            ]
        );
    }

    #[test]
    fn test_part1() {
        let games = vec![
            Game { red: 5, green: 10, blue: 8 },
            Game { red: 12, green: 11, blue: 9 },
            Game { red: 15, green: 13, blue: 14 },
        ];
        assert_eq!(part1(&games), 3);
    }

    #[test]
    fn test_part2() {
        let games = vec![
            Game { red: 5, green: 10, blue: 8 },
            Game { red: 12, green: 11, blue: 9 },
            Game { red: 15, green: 13, blue: 14 },
        ];
        assert_eq!(part2(&games), 400 + 1188 + 2730);
    }

    #[test]
    fn test_parse_with_trailing_characters() {
        let input = "1 5 red; 10 green, 8 blue.\n2 12 red 11 green 9 blue";
        let games = parse(input);
        assert_eq!(
            games,
            vec![
                Game { red: 5, green: 10, blue: 8 },
                Game { red: 12, green: 11, blue: 9 },
            ]
        );
    }
}


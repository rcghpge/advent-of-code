// Day 19 Aplenty
use crate::util::hash::*;
use crate::util::iter::*;
use crate::util::parse::*;
use std::fs;

pub struct Rule<'a> {
    start: u32,
    end: u32,
    category: usize,
    next: &'a str,
}

pub struct Input<'a> {
    workflows: FastMap<&'a str, Vec<Rule<'a>>>,
    parts: &'a str,
}

/// Parse input into rules and parts.
pub fn parse(input: &str) -> Input<'_> {
    let (prefix, suffix) = input.split_once("\n\n").unwrap();
    let mut workflows = FastMap::with_capacity(1000);

    for line in prefix.lines() {
        let mut rules = Vec::with_capacity(5);
        let mut iter = line.split(['{', ':', ',', '}']);
        let key = iter.next().unwrap();

        for [first, second] in iter.chunk::<2>() {
            let rule = if second.is_empty() {
                Rule {
                    start: 1,
                    end: 4001,
                    category: 0,
                    next: first,
                }
            } else {
                let category = match first.as_bytes()[0] {
                    b'x' => 0,
                    b'm' => 1,
                    b'a' => 2,
                    b's' => 3,
                    _ => unreachable!(),
                };

                let value: u32 = (&first[2..]).unsigned();
                let next = second;

                match first.as_bytes()[1] {
                    b'<' => Rule {
                        start: 1,
                        end: value,
                        category,
                        next,
                    },
                    b'>' => Rule {
                        start: value + 1,
                        end: 4001,
                        category,
                        next,
                    },
                    _ => unreachable!(),
                }
            };

            rules.push(rule);
        }

        workflows.insert(key, rules);
    }

    Input {
        workflows,
        parts: suffix,
    }
}

/// Compute Part 1: Sum values in category "A".
pub fn part1(input: &Input<'_>) -> u32 {
    let Input { workflows, parts } = input;
    let mut result = 0;

    for part in parts.iter_unsigned::<u32>().chunk::<4>() {
        let mut key = "in";

        while key.len() > 1 {
            for &Rule {
                start,
                end,
                category,
                next,
            } in &workflows[key]
            {
                if start <= part[category] && part[category] < end {
                    key = next;
                    break;
                }
            }
        }

        if key == "A" {
            result += part.iter().sum::<u32>();
        }
    }

    result
}

/// Compute Part 2: Sum ranges in category "A" using range splitting.
pub fn part2(input: &Input<'_>) -> u64 {
    let Input { workflows, .. } = input;
    let mut result = 0;
    let mut todo = vec![("in", 0, [(1, 4001); 4])];

    while let Some((key, index, mut part)) = todo.pop() {
        if key.len() == 1 {
            if key == "A" {
                result += part.iter().map(|(s, e)| (e - s) as u64).product::<u64>();
            }
            continue;
        }

        let Rule {
            start: s2,
            end: e2,
            category,
            next,
        } = workflows[key][index];
        let (s1, e1) = part[category];

        let x1 = s1.max(s2);
        let x2 = e1.min(e2);

        if x1 >= x2 {
            todo.push((key, index + 1, part));
        } else {
            part[category] = (x1, x2);
            todo.push((next, 0, part));

            if s1 < x1 {
                part[category] = (s1, x1);
                todo.push((key, index + 1, part));
            }

            if x2 < e1 {
                part[category] = (x2, e1);
                todo.push((key, index + 1, part));
            }
        }
    }

    result
}

/// Main entry point for script
pub fn run() {
    // Load input data
    let input_file = "data/input19.txt";
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


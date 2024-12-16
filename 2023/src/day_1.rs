//! Day 1:
// Process input to extract numeric values and calculate results for Day 1.
// Input contains overlapping digits such as "twone". The script parses input and
// computes results by extracting starting and ending numeric digits.
use crate::util::parse::ParseOps;
use crate::util::parse::ParseByte;

/// Use the index of each digit as its implicit value.
const DIGITS: [&[u8]; 9] = [
    b"one", b"two", b"three", b"four", b"five", b"six", b"seven", b"eight", b"nine",
];

/// Parses input file into a vector of string slices.
pub fn parse(input: &str) -> Vec<&str> {
    input.lines().collect()
}

/// Solve part 1 by summing two-digit numbers created from first and last digits
/// of each line in input data.
pub fn part1(input: &[&str]) -> u32 {
    input
        .iter()
        .map(|line| {
            let first = line.bytes().find(u8::is_ascii_digit).unwrap().to_decimal();
            let last = line.bytes().rfind(u8::is_ascii_digit).unwrap().to_decimal();
            (10 * first + last) as u32
        })
        .sum()
}

/// Solve part 2 by parsing first and last numbers in each line, considering both
/// digits and digit names (e.g., "one", "two").
pub fn part2(input: &[&str]) -> usize {
    input
        .iter()
        .map(|line| {
            let mut line = line.as_bytes();

            let first = 'outer: loop {
                if line[0].is_ascii_digit() {
                    break line[0].to_decimal() as usize;
                }
                for (value, digit) in DIGITS.iter().enumerate() {
                    if line.starts_with(digit) {
                        break 'outer value + 1;
                    }
                }
                line = &line[1..];
            };

            let last = 'outer: loop {
                if line[line.len() - 1].is_ascii_digit() {
                    break line[line.len() - 1].to_decimal() as usize;
                }
                for (value, digit) in DIGITS.iter().enumerate() {
                    if line.ends_with(digit) {
                        break 'outer value + 1;
                    }
                }
                line = &line[..line.len() - 1];
            };

            10 * first + last
        })
        .sum()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse() {
        let input = "123\n456\n789";
        let result = parse(input);
        assert_eq!(result, vec!["123", "456", "789"]);
    }

    #[test]
    fn test_part1() {
        let input = vec!["123", "456", "789"];
        let result = part1(&input);
        assert_eq!(result, 12 + 45 + 78);
    }

    #[test]
    fn test_part2() {
        let input = vec!["one234five", "7eight9two"];
        let result = part2(&input);
        assert_eq!(result, (10 * 1 + 5) + (10 * 7 + 2)); // 15 + 72 = 87
    }
}
pub fn run() {
    let input = std::fs::read_to_string("data/input1.txt")
        .expect("Failed to read input file");

    let parsed = parse(&input);

    let result1 = part1(&parsed);
    println!("Part 1: {}", result1);

    let result2 = part2(&parsed);
    println!("Part 2: {}", result2);
}


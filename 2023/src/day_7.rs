// Day 7: Camel Cards 
use std::fs;
use crate::util::utils::argmax;

fn card_rank(card: char, with_jokers: bool) -> usize {
    let order = if with_jokers {
        "J23456789TQKA"
    } else {
        "23456789TJQKA"
    };
    order
        .chars()
        .position(|c| c == card)
        .expect(&format!("Invalid card character: {}", card))
}

fn hand_type(hand: &str, with_jokers: bool) -> i64 {
    let mut card_counts = [0i64; 13];
    for c in hand.chars() {
        card_counts[card_rank(c, with_jokers)] += 1;
    }
    let mut n_jokers = 0;
    if with_jokers {
        n_jokers = card_counts[0];
        card_counts[0] = 0;
    }
    let rank_of_most_frequent = argmax(&card_counts).expect("Failed to find the rank of the most frequent card");
    if with_jokers {
        card_counts[rank_of_most_frequent] += n_jokers;
    }
    let n_pairs = card_counts.iter().filter(|&&c| c == 2).count() as i64;
    match card_counts[rank_of_most_frequent] {
        5 => 6,
        4 => 5,
        3 => 3 + n_pairs,
        2 => n_pairs,
        1 => 0,
        _ => unreachable!(),
    }
}

fn compare_hands(h1: &str, h2: &str, with_jokers: bool) -> std::cmp::Ordering {
    let type_cmp = hand_type(h1, with_jokers).cmp(&hand_type(h2, with_jokers));
    if type_cmp != std::cmp::Ordering::Equal {
        return type_cmp;
    }
    for (c1, c2) in h1.chars().zip(h2.chars()) {
        if c1 != c2 {
            return card_rank(c1, with_jokers).cmp(&card_rank(c2, with_jokers));
        }
    }
    std::cmp::Ordering::Equal
}

fn sort_hands_and_get_total_winnings(hands: &mut [(String, i64)], with_jokers: bool) -> i64 {
    hands.sort_by(|(h1, _), (h2, _)| compare_hands(h1, h2, with_jokers));
    hands
        .iter()
        .enumerate()
        .map(|(rank, (_, bid))| (rank as i64 + 1) * bid)
        .sum()
}

pub fn run() {
    // Use the hardcoded path for the input file
    let input_path = "data/input7.txt";

    // Read input from the hardcoded file path
    let input = fs::read_to_string(input_path).expect("Failed to read input file");

    // Parse hands and calculate total winnings
    let mut hands: Vec<(String, i64)> = input
        .lines()
        .enumerate()
        .filter_map(|(line_num, line)| {
            let mut parts = line.split_ascii_whitespace();
            let hand = parts.next();
            let bid = parts.next();

            match (hand, bid) {
                (Some(hand), Some(bid)) => {
                    let parsed_bid = bid.parse::<i64>();
                    match parsed_bid {
                        Ok(bid) => Some((hand.to_string(), bid)),
                        Err(_) => {
                            eprintln!("Warning: Invalid bid on line {}: '{}'", line_num + 1, line);
                            None
                        }
                    }
                }
                _ => {
                    eprintln!("Warning: Malformed line {}: '{}'", line_num + 1, line);
                    None
                }
            }
        })
        .collect();

    // Part 1: Without jokers
    let total_winnings_part1 = sort_hands_and_get_total_winnings(&mut hands, false);
    println!("Part 1: {}", total_winnings_part1);

    // Part 2: With jokers
    let total_winnings_part2 = sort_hands_and_get_total_winnings(&mut hands, true);
    println!("Part 2: {}", total_winnings_part2);
}


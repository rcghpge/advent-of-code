// Day 11 Cosmic Expansion
use std::fs;
use hashbrown::HashSet;
use std::collections::BTreeSet;

fn expanded_distance(
    coord1: i32,
    coord2: i32,
    galaxy_coords: &BTreeSet<i32>,
    expansion_factor: u64,
) -> u64 {
    let min_coord = coord1.min(coord2);
    let max_coord = coord1.max(coord2);
    let distance = (max_coord - min_coord) as u64;

    if distance < 2 {
        return distance;
    }

    let n_galaxies_in_between = galaxy_coords
        .range((min_coord + 1)..max_coord)
        .count() as u64;

    (distance - 1 - n_galaxies_in_between) * expansion_factor + n_galaxies_in_between + 1
}

fn sum_of_shortest_distances(galaxies: &[(i32, i32)], expansion_factor: u64) -> u64 {
    let galaxy_cols: BTreeSet<i32> = galaxies.iter().map(|&(x, _)| x).collect();
    let galaxy_rows: BTreeSet<i32> = galaxies.iter().map(|&(_, y)| y).collect();

    galaxies
        .iter()
        .enumerate()
        .flat_map(|(i, &g1)| galaxies.iter().skip(i + 1).map(move |&g2| (g1, g2)))
        .map(|(g1, g2)| {
            expanded_distance(g1.0, g2.0, &galaxy_cols, expansion_factor)
                + expanded_distance(g1.1, g2.1, &galaxy_rows, expansion_factor)
        })
        .sum()
}

pub fn run() {
    // Read the input file
    let input_file = "data/input11.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse and process the input
    let galaxies: Vec<(i32, i32)> = input
        .lines()
        .enumerate()
        .flat_map(|(y, line)| {
            line.bytes()
                .enumerate()
                .filter(|&(_, c)| c == b'#')
                .map(move |(x, _)| (x as i32, y as i32))
        })
        .collect();

    // Calculate and print total distances for each expansion factor
    let part_1_distance = sum_of_shortest_distances(&galaxies, 2);
    println!("Part 1: {}", part_1_distance);

    let part_2_distance = sum_of_shortest_distances(&galaxies, 1_000_000);
    println!("Part 2: {}", part_2_distance);
}



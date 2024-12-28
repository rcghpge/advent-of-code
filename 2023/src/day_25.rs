// Day 25 Snowverload
use std::collections::VecDeque;
use std::fs;

/// Store graph as [adjacency list](https://en.wikipedia.org/wiki/Adjacency_list).
/// Each node has unique index in `nodes` vec.
/// Each directed edge has unique index in `edges` vec.
pub struct Input {
    edges: Vec<usize>,
    nodes: Vec<(usize, usize)>,
}

impl Input {
    /// Return an iterator of `(edge, node)` pairs.
    #[inline]
    fn neighbours(&self, node: usize) -> impl Iterator<Item = (usize, usize)> + '_ {
        let (start, end) = self.nodes[node];
        (start..end).map(move |edge| (edge, self.edges[edge]))
    }
}

pub fn parse(input: &str) -> Input {
    let mut lookup = vec![usize::MAX; 26 * 26 * 26];
    let mut neighbours = Vec::with_capacity(2_000);

    for line in input.lines().map(str::as_bytes) {
        let first = perfect_minimal_hash(&mut lookup, &mut neighbours, line);

        for chunk in line[5..].chunks(4) {
            let second = perfect_minimal_hash(&mut lookup, &mut neighbours, chunk);
            neighbours[first].push(second);
            neighbours[second].push(first);
        }
    }

    let mut edges = Vec::with_capacity(5_000);
    let mut nodes = Vec::with_capacity(neighbours.len());

    for list in neighbours {
        let start = edges.len();
        let end = edges.len() + list.len();
        edges.extend(list);
        nodes.push((start, end));
    }

    Input { edges, nodes }
}

pub fn part1(input: &Input) -> usize {
    let start = furthest(input, 0);
    let end = furthest(input, start);
    let size = flow(input, start, end);
    size * (input.nodes.len() - size)
}

pub fn part2(_input: &Input) -> &'static str {
    "N/A"
}

fn perfect_minimal_hash(lookup: &mut [usize], nodes: &mut Vec<Vec<usize>>, slice: &[u8]) -> usize {
    let hash = slice[..3].iter().fold(0, |acc, &b| 26 * acc + ((b - b'a') as usize));
    let mut index = lookup[hash];

    if index == usize::MAX {
        index = nodes.len();
        lookup[hash] = index;
        nodes.push(Vec::with_capacity(10));
    }

    index
}

fn furthest(input: &Input, start: usize) -> usize {
    let mut todo = VecDeque::new();
    todo.push_back(start);

    let mut seen = vec![false; input.nodes.len()];
    seen[start] = true;

    let mut result = start;

    while let Some(current) = todo.pop_front() {
        result = current;

        for (_, next) in input.neighbours(current) {
            if !seen[next] {
                todo.push_back(next);
                seen[next] = true;
            }
        }
    }

    result
}

fn flow(input: &Input, start: usize, end: usize) -> usize {
    let mut todo = VecDeque::new();
    let mut path = Vec::new();
    let mut used = vec![false; input.edges.len()];
    let mut result = 0;

    for _ in 0..4 {
        todo.push_back((start, usize::MAX));
        result = 0;

        let mut seen = vec![false; input.nodes.len()];
        seen[start] = true;

        while let Some((current, head)) = todo.pop_front() {
            result += 1;

            if current == end {
                let mut index = head;

                while index != usize::MAX {
                    let (edge, next) = path[index];
                    used[edge] = true;
                    index = next;
                }

                break;
            }

            for (edge, next) in input.neighbours(current) {
                if !used[edge] && !seen[next] {
                    seen[next] = true;
                    todo.push_back((next, path.len()));
                    path.push((edge, head));
                }
            }
        }

        todo.clear();
        path.clear();
    }

    result
}

pub fn run() {
    let input_str = fs::read_to_string("data/input25.txt").expect("Failed to read input file");
    let parsed_input = parse(&input_str);

    let result_part1 = part1(&parsed_input);
    let result_part2 = part2(&parsed_input);

    println!("Part 1: {}", result_part1);
    println!("Part 2: {}", result_part2);
}


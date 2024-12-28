// Day 24 Never Tell Me The Odds
use crate::util::iter::*;
use crate::util::math::*;
use crate::util::parse::*;
use std::ops::RangeInclusive;
use std::fs;

const RANGE: RangeInclusive<i64> = 200_000_000_000_000..=400_000_000_000_000;

#[derive(Clone, Copy)]
struct Vector {
    x: i128,
    y: i128,
    z: i128,
}

impl Vector {
    fn add(self, other: Self) -> Self {
        Vector {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z,
        }
    }

    fn sub(self, other: Self) -> Self {
        Vector {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z,
        }
    }

    fn cross(self, other: Self) -> Self {
        Vector {
            x: self.y * other.z - self.z * other.y,
            y: self.z * other.x - self.x * other.z,
            z: self.x * other.y - self.y * other.x,
        }
    }

    fn gcd(self) -> Self {
        let gcd = self.x.gcd(self.y).gcd(self.z);
        Vector {
            x: self.x / gcd,
            y: self.y / gcd,
            z: self.z / gcd,
        }
    }

    fn sum(self) -> i128 {
        self.x + self.y + self.z
    }
}

pub fn parse(input: &str) -> Vec<[i64; 6]> {
    input.iter_signed().chunk::<6>().collect()
}

pub fn part1(input: &[[i64; 6]]) -> u32 {
    let mut result = 0;

    for (index, &[a, b, _, c, d, _]) in input.iter().enumerate() {
        for &[e, f, _, g, h, _] in &input[..index] {
            let determinant = d * g - c * h;
            if determinant == 0 {
                continue;
            }

            let t = (g * (f - b) - h * (e - a)) / determinant;
            let u = (c * (f - b) - d * (e - a)) / determinant;

            let x = a + t * c;
            let y = b + t * d;

            if t >= 0 && u >= 0 && RANGE.contains(&x) && RANGE.contains(&y) {
                result += 1;
            }
        }
    }

    result
}

pub fn part2(input: &[[i64; 6]]) -> i128 {
    let widen = |i: usize| {
        let [px, py, pz, vx, vy, vz] = input[i].map(|n| n as i128);
        (Vector { x: px, y: py, z: pz }, Vector { x: vx, y: vy, z: vz })
    };

    let (p0, v0) = widen(0);
    let (p1, v1) = widen(1);
    let (p2, v2) = widen(2);

    let p3 = p1.sub(p0);
    let p4 = p2.sub(p0);
    let v3 = v1.sub(v0);
    let v4 = v2.sub(v0);

    let q = v3.cross(p3).gcd();
    let r = v4.cross(p4).gcd();
    let s = q.cross(r).gcd();

    let t = (p3.y * s.x - p3.x * s.y) / (v3.x * s.y - v3.y * s.x);
    let u = (p4.y * s.x - p4.x * s.y) / (v4.x * s.y - v4.y * s.x);
    assert!(t != u);

    let a = p0.add(p3).sum();
    let b = p0.add(p4).sum();
    let c = v3.sub(v4).sum();
    (u * a - t * b + u * t * c) / (u - t)
}

pub fn run() {
    let input_str = fs::read_to_string("data/input24.txt").expect("Failed to read input file");
    let parsed_input = parse(&input_str);

    let result_part1 = part1(&parsed_input);
    let result_part2 = part2(&parsed_input);

    println!("Part 1: {}", result_part1);
    println!("Part 2: {}", result_part2);
}


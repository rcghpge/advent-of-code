mod util;
mod day_1;
mod day_2;
mod day_3;
mod day_4;
mod day_5;
mod day_6;
mod day_7;
mod day_8;
mod day_9;
mod day_10;
mod day_11;
mod day_12;
mod day_13;
mod day_14;
mod day_15;
mod day_16;
mod day_17;
mod day_18;
mod day_19;
mod day_20;
mod day_21;
mod day_22;
mod day_23;
mod day_24;
mod day_25;

use std::time::Instant;

fn main() {
    println!("Hello World! This is my first line of Rust code!");
    println!("--12/16/2024 hoping for a solid 2025 less geddit--");
    println!("Running Advent of Code 2023 solutions..........");

    // Helper function to time and run a day's solution
    fn time_and_run<F: FnOnce()>(day_name: &str, run_day: F) {
        println!("{}", day_name);
        let start = Instant::now();
        run_day();
        let duration = start.elapsed();
        println!("Execution time: {:?}\n", duration);
    }

    // Day 1
    time_and_run("Day 1: Trebuchet?!", || day_1::run());

    // Day 2
    time_and_run("Day 2: Cube Conundrum", || day_2::run());

    // Day 3
    time_and_run("Day 3: Gear Ratios", || day_3::run());

    // Day 4
    time_and_run("Day 4: Scratchcards", || day_4::run());

    // Day 5
    time_and_run("Day 5: If You Give A Seed A Fertilizer", || day_5::run());

    // Day 6
    time_and_run("Day 6: Wait For It", || day_6::run());

    // Day 7
    time_and_run("Day 7: Camel Cards", || day_7::run());

    // Day 8
    time_and_run("Day 8: Haunted Wasteland", || day_8::run());

    // Day 9
    time_and_run("Day 9: Mirage Maintenance", || day_9::run());

    // Day 10
    time_and_run("Day 10: Pipe Maze", || day_10::run());

    // Day 11
    time_and_run("Day 11: Cosmic Expansion", || day_11::run());

    // Day 12
    time_and_run("Day 12: Hot Springs", || day_12::run());

    // Day 13
    time_and_run("Day 13: Point of Incidence", || day_13::run());

    // Day 14
    time_and_run("Day 14: Parabolic Reflector Dish", || day_14::run());

    // Day 15
    time_and_run("Day 15: Lens Library", || day_15::run());

    // Day 16
    time_and_run("Day 16: The Floor Will Be Lava", || day_16::run());

    // Day 17
    time_and_run("Day 17: Clumsy Crucible", || day_17::run());

    // Day 18
    time_and_run("Day 18: Lavaduct Lagoon", || day_18::run());

    // Day 19
    time_and_run("Day 19: Aplenty", || day_19::run());

    // Day 20
    time_and_run("Day 20: Pulse Propagation", || day_20::run());

    // Day 21
    time_and_run("Day 21: Step Counter", || day_21::run());

    // Day 22
    time_and_run("Day 22: Sand Slabs", || day_22::run());

    // Day 23
    time_and_run("Day 23: A Long Walk", || day_23::run());

    // Day 24
    time_and_run("Day 24: Never Tell Me The Odds", || day_24::run());

    // Day 25
    time_and_run("Day 25: Snowverload", || day_25::run());
}


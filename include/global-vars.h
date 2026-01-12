#ifndef BOID_VARS_H
#define BOID_VARS_H

/**
 * global configuration variables used throughout the simulation
 * some preprocessor abuse was performed to avoid excessive duplication
 * between graphical and benchmark versions of the program
 */

extern int num_threads; // number of omp threads used
extern int num_boids;   // number of boids we're gonna spawn

extern int window_width;  // dimensions of raylib window
extern int window_height;
#ifdef BOID_GRAPHICS
extern int window_fps;    // fps of raylib window
#endif

#ifdef BOID_BENCHMARK
extern int num_warmup_iterations; // number of warmup iterations before measuring
extern int num_iterations;        // number of iterations in benchmark
extern char* output_file;         // file where benchmark results will be written
#endif

extern float turnback_margin; // distance from window border at which boids starts turning back
extern float turnback_factor; // strength of turning back

extern float neighbour_radius;   // min distance for two boids to be neighbours
extern float centering_factor;   // strength of boid centering potential
extern float speed_match_factor; // strength of boid speed matching potential

extern float too_close_radius; // distance at which boids start avoiding each other
extern float dont_slam_factor; // strength of boid collision avoidance potential

extern float speed_upper_bound; // upper bound on speed boids can have
extern float speed_lower_bound; // lower bound on speed boids can have

extern float acceleration_upper_bound;
// upper bound on how hard can you accelerate in one frame

#endif

#include"global-vars.h"

// https://stackoverflow.com/questions/69908418/multiple-definition-of-first-defined-here-on-gcc-10-2-1-but-not-gcc-8-3-0
// can't have variable definitions in a header like that

int num_boids = 100;

int window_width = 1000;
int window_height = 1000;
#ifdef BOID_GRAPHICS
int window_fps = 60;
#endif

#ifdef BOID_BENCHMARK
int num_warmup_iterations = 100;
int num_iterations = 200;
char* output_file = "./results/benchmark.csv";
#endif

float turnback_margin = 100.0;
float turnback_factor = 1.0;

float neighbour_radius = 75.0;
float centering_factor = 0.005;
float speed_match_factor = 0.05;

float too_close_radius = 20.0;
float dont_slam_factor = 0.05;

float speed_upper_bound = 50.00;
float speed_lower_bound = 5.00;

float acceleration_upper_bound = 2.0;

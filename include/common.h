#ifndef BOIDS_COMMON_H
#define BOIDS_COMMON_H

#include "argparse.h"

// somewhat brutal reimplementation of https://github.com/beneater/boids
// program parameters are global variables to facilitate fiddling
// I apologize for any heartaches caused by global variables

int num_boids = 100; // number of boids we're gonna spawn

int window_width = 1000;  // dimensions of raylib window
int window_height = 1000;
int window_fps = 60;      // fps of raylib window

float turnback_margin = 100.0; // distance from window border at which boids turn back
float turnback_factor = 1.0;   // strength of turning back

float neighbour_radius = 75.0;   // min distance for two boids to be neighbours
float centering_factor = 0.005;  // strength of boid centering potential
float speed_match_factor = 0.05; // strength of boid speed matching potential

float too_close_radius = 20.0; // distance at which boids start avoiding each other
float dont_slam_factor = 0.05; // strength of boid collision avoidance potential

float speed_upper_bound = 50.00;  // upper bound on speed boids can have
float speed_lower_bound = 5.00;   // lower bound on speed boids can have

float acceleration_upper_bound = 2.0; // upper bound on how hard can you accelerate in one frame

void parse_args(int argc, char** argv) {
	int quiet = 0;

	argparse_bind_int("--quiet", &quiet,
						"don't print any sort of help message when starting, set to 1 to have no help message");
	argparse_bind_int("--num-boids", &num_boids,
						"number of boids that will be spawned for the simulation");

	argparse_bind_int("--window-width", &window_width,
					  "initial width of window, which coincides with simulation area");
	argparse_bind_int("--window-height", &window_height,
					  "initial width of window, which coincides with simulation area");
	argparse_bind_int("--window-fps", &window_fps,
					  "fps with which the window and the simulation will be updated"
					  "\n(one and ony frame will be rendered for each simulation tick)");

	argparse_bind_float("--turnback-margin", &turnback_margin,
						"distance (in pixels) from window margin before which boids will start to turn back"); 
	argparse_bind_float("--turnback-factor", &turnback_factor,
						"acceleration (in pixels/frame^2) that boids will receive to move away from window margin if they're too close"); 

	argparse_bind_float("--neighbour-radius", &neighbour_radius,
						"minimum distance (in pixels) that must occur between two boids for them to be considered neighbours"
						"\naffects centering and speed matching behaviour");
	argparse_bind_float("--centering-factor", &centering_factor,
						"factor determining the strength with which boids will want to move towards the center of their respective neighbourhood");
	argparse_bind_float("--speed-match-factor", &speed_match_factor,
						"factor determining the strength with which boids will want to move at the same speed as that of neighbours");

	argparse_bind_float("--too-close-radius", &too_close_radius,
						"minimum distance (in pixels) that must ocur between two boids for them to start moving away from each other to avoid colliding"
						"\n(simulation does not currently account for the eventuality of boid collision)");
	argparse_bind_float("--dont-slam-factor", &dont_slam_factor,
						"factor determining the strength with which boids will wanto to move away from each other to avoid colliding");

	argparse_bind_float("--speed-upper-bound", &speed_upper_bound,
						"upper bound (in pixels/frame) on boid speed, boids cannot go faster than this");
	argparse_bind_float("--speed-lower-bound", &speed_lower_bound,
						"lower bound (in pixels/frame) on boid speed, boids cannot go slower than this");
	argparse_bind_float("--acceleration-upper-bound", &acceleration_upper_bound,
						"maximum module of acceleration (in pixels/frame^2) that a boid can have between two frames");

	argparse_assign_values(argc, argv);

	if(!quiet) {
		argparse_print_flag_help_name("--quiet");
		puts("\nsimulation options");
		argparse_print_flag_help_name("--num-boids");
		puts("\nwindow options");
		argparse_print_flag_help_name("--window-width");
		argparse_print_flag_help_name("--window-height");
		argparse_print_flag_help_name("--window-fps");
		puts("\nturnback options");
		argparse_print_flag_help_name("--turnback-margin");
		argparse_print_flag_help_name("--turnback-factor");
		puts("\nneghbourhood matching options");
		argparse_print_flag_help_name("--neighbour-radius");
		argparse_print_flag_help_name("--centering-factor");
		argparse_print_flag_help_name("--speed-match-factor");
		puts("\ncollision avoidance options");
		argparse_print_flag_help_name("--too-close-radius");
		argparse_print_flag_help_name("--dont-slam-factor");
		puts("\nspeed limit options");
		argparse_print_flag_help_name("--speed-upper-bound");
		argparse_print_flag_help_name("--speed-lower-bound");
		argparse_print_flag_help_name("--acceleration-upper-bound");
		puts("");
		
	}

	argparse_free();
}

float random_uniform(float min, float max) {
    float random_normalized = (float)random() / (float)INT_MAX;
    return (random_normalized * (max - min)) + min;
}

#endif

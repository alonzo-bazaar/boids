#include "argparse.h"
#include "global-vars.h"

void bind_boid_vars() {
	argparse_bind_int("--num-boids", &num_boids,
						"number of boids that will be spawned for the simulation");

#ifdef BOID_GRAPHICS
#define SPACE_UNITS "pixels"
#define TIME_UNIT "frame"
	argparse_bind_int("--window-width", &window_width,
					  "initial width of window (in " SPACE_UNITS "), which coincides with simulation area");
	argparse_bind_int("--window-height", &window_height,
					  "initial width of window (in " SPACE_UNITS "), which coincides with simulation area");
	argparse_bind_int("--window-fps", &window_fps,
					  "fps with which the window and the simulation will be updated"
					  "\n(one and ony frame will be rendered for each simulation tick)");
#endif

#ifdef BOID_BENCHMARK
#define SPACE_UNITS "units"
#define TIME_UNIT "simulation tick"
	// mildly ugly variable, but having the simulation area params use the window width/height variables
	// allows us to reuse the update code we had for the graphics version without having to duplicate anything
	// and we have a more simulation-y name for the command line argument
	argparse_bind_int("--area-width", &window_width, "width (in " SPACE_UNITS ") of simulation area");
	argparse_bind_int("--area_height", &window_height, "height (in " SPACE_UNITS ") of simulation area");

	argparse_bind_int("--num-warmup-iterations", &num_warmup_iterations,
					  "number of warmup iterations to be done before we start the benchmark");
	argparse_bind_int("--num-iterations", &num_iterations,
					  "number of iterations done as measurement for the benchmark");
	argparse_bind_string("--output-file", &output_file,
						 "csv file where benchmark results will be saved once the benchmark is done");
#endif

	argparse_bind_float("--turnback-margin", &turnback_margin,
						"distance (in " SPACE_UNITS ") from window margin before which boids will start to turn back"); 
	argparse_bind_float("--turnback-factor", &turnback_factor,
						"acceleration (in " SPACE_UNITS "/" TIME_UNIT "^2) that boids will receive to move away from window margin if they're too close"); 

	argparse_bind_float("--neighbour-radius", &neighbour_radius,
						"minimum distance (in " SPACE_UNITS ") that must occur between two boids for them to be considered neighbours"
						"\naffects centering and speed matching behaviour");
	argparse_bind_float("--centering-factor", &centering_factor,
						"factor determining the strength with which boids will want to move towards the center of their respective neighbourhood");
	argparse_bind_float("--speed-match-factor", &speed_match_factor,
						"factor determining the strength with which boids will want to move at the same speed as that of neighbours");

	argparse_bind_float("--too-close-radius", &too_close_radius,
						"minimum distance (in " SPACE_UNITS ") that must ocur between two boids for them to start moving away from each other to avoid colliding"
						"\n(simulation does not currently account for the eventuality of boid collision)");
	argparse_bind_float("--dont-slam-factor", &dont_slam_factor,
						"factor determining the strength with which boids will wanto to move away from each other to avoid colliding");

	argparse_bind_float("--speed-upper-bound", &speed_upper_bound,
						"upper bound (in " SPACE_UNITS "/" TIME_UNIT ") on boid speed, boids cannot go faster than this");
	argparse_bind_float("--speed-lower-bound", &speed_lower_bound,
						"lower bound (" SPACE_UNITS "/" TIME_UNIT ") on boid speed, boids cannot go slower than this");
	argparse_bind_float("--acceleration-upper-bound", &acceleration_upper_bound,
						"maximum module of acceleration (" SPACE_UNITS "/" TIME_UNIT "^2) that a boid can have between two frames");

#undef SPACE_UNITS
#undef TIME_UNIT
}

void print_help() {
	argparse_print_flag_help_name("--quiet");
	puts("\nsimulation options");
	argparse_print_flag_help_name("--num-boids");
#ifdef BOID_BENCHMARK
	argparse_print_flag_help_name("--area-width");
	argparse_print_flag_help_name("--area-height");
#endif


#ifdef BOID_GRAPHICS
	puts("\nwindow options");
	argparse_print_flag_help_name("--window-width");
	argparse_print_flag_help_name("--window-height");
	argparse_print_flag_help_name("--window-fps");
#endif

#ifdef BOID_BENCHMARK
	puts("\nbenchmark options options");
	argparse_print_flag_help_name("--num-warmup-iterations");
	argparse_print_flag_help_name("--num-iterations");
	argparse_print_flag_help_name("--output-file");
#endif

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

void handle_argv_vars(int argc, char** argv) {
	int quiet = 0;
	argparse_bind_int("--quiet", &quiet,
						"don't print any sort of help message when starting, set to 1 to have no help message");

	bind_boid_vars();
	argparse_assign_values(argc, argv);
	if(!quiet) {
		print_help();
	}

	argparse_free();
}

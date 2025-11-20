#ifndef BOIDS_ARGS_H
#define BOIDS_ARGS_H

/**
 * uses argparse.h to set configuration global variables
 * specific to boid simulation according to passed command line arguments
 */

#include "argparse.h"
#include "global-vars.h"

void bind_boid_vars();
void print_help();
void handle_argv_vars(int argc, char** argv);

#endif

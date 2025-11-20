#ifndef BOID_SOA_H
#define BOID_SOA_H

/**
 * simulation logic for struct of arrays versions of the program
 */

#include<math.h>         // for sqrt
#include"common.h"       // random uniform
#include"global-vars.h"  // simulation configuration global variables

void random_uniform_array(size_t n_elems, float arr [n_elems],
						  float min, float max);

void randomize_boids(const size_t n_boids,
					 float x[n_boids], float y[n_boids],
					 float dx[n_boids], float dy[n_boids]);

void boids_update(const size_t n_boids,
				  const float curr_x[n_boids], const float curr_y[n_boids],
				  const float curr_dx[n_boids], const float curr_dy[n_boids],
				  float next_x[n_boids], float next_y[n_boids],
				  float next_dx[n_boids], float next_dy[n_boids]);

#endif

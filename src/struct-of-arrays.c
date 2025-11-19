#include<stdio.h>   // print debugging
#include<stdlib.h>  // to create random numbers
#include<limits.h>  // to scale the random number in question
#include<math.h>    // for sqrt

#include"raylib.h"  // for the graphics
#include"common.h"

// randomize initial state of boids
// (better not to try my luck with parallelizing C random)
void random_uniform_array(size_t n_elems, float arr [n_elems],
						  float min, float max) {
	for(size_t i = 0; i<n_elems; ++i) {
		arr[i] = random_uniform(min, max);
	}
}

// we don't read next_(d)(x|y) at init stage
// we can leave them uninitialized for now
void randomize_boids(const size_t n_boids,
					 float x[n_boids], float y[n_boids],
					 float dx[n_boids], float dy[n_boids]) {
	random_uniform_array(n_boids, x,
					turnback_margin, window_width - turnback_margin);
	random_uniform_array(n_boids, y,
					turnback_margin, window_height - turnback_margin);

	// speeds will be sampled from a square and not from a circle
	// big whoops
	float speed_axis_bound = speed_upper_bound / sqrt(2);
	random_uniform_array(n_boids, dx,
						 -speed_axis_bound, speed_axis_bound);
	random_uniform_array(n_boids, dy,
						 -speed_axis_bound, speed_axis_bound);
	
}

void boids_update(const size_t n_boids,
				  const float curr_x[n_boids], const float curr_y[n_boids],
				  const float curr_dx[n_boids], const float curr_dy[n_boids],
				  float next_x[n_boids], float next_y[n_boids],
				  float next_dx[n_boids], float next_dy[n_boids]) {
	// compute next positions
#pragma omp parallel
	{
#pragma omp for
		for(size_t i = 0; i<n_boids; ++i) {
			next_x[i] = curr_x[i] + curr_dx[i];
			next_y[i] = curr_y[i] + curr_dy[i];
		}

		// compute next velocities
		// data to be computed only after having scanned all neighbours
		// neighbourhood centroid
		float center_x[n_boids];
		float center_y[n_boids];

		// average neighbourhood speed
		float avg_neigh_dx[n_boids];
		float avg_neigh_dy[n_boids];

		int n_neighbours[n_boids];

		// replusive force to apply to boid to avoid it crashing on its neighbours
		float repulse_x[n_boids];
		float repulse_y[n_boids];

#pragma omp for
		for(size_t i = 0; i<n_boids; ++i) {
			center_x[i] = 0.0f;
			center_y[i] = 0.0f;

			avg_neigh_dx[i] = 0.0f;
			avg_neigh_dy[i] = 0.0f;

			n_neighbours[i] = 0;

			repulse_x[i] = 0.0f;
			repulse_y[i] = 0.0f;
		}

		// data collection loop
#pragma omp for
		for(size_t i = 0; i<n_boids-1; ++i) {
			for(size_t j = i+1; j<n_boids; ++j) {
				float dx = curr_x[i] - curr_x[j];
				float dy = curr_y[i] - curr_y[j];
				float dist = sqrt((dx*dx) + (dy*dy));

				if(dist < too_close_radius) {
					repulse_x[i] += dx;
					repulse_y[i] += dy;

					repulse_x[j] -= dx;
					repulse_y[j] -= dy;
				}

				if(dist < neighbour_radius) {
					avg_neigh_dx[i] += curr_dx[j];
					avg_neigh_dy[i] += curr_dy[j];

					avg_neigh_dx[j] += curr_dx[i];
					avg_neigh_dy[j] += curr_dy[i];

					center_x[i] += curr_x[j];
					center_y[i] += curr_y[j];

					center_x[j] += curr_x[i];
					center_y[j] += curr_y[i];

					n_neighbours[i]++;
					n_neighbours[j]++;
				}
			}

			if(n_neighbours[i] > 0) {
				center_x[i]/=n_neighbours[i];
				center_y[i]/=n_neighbours[i];
				avg_neigh_dx[i]/=n_neighbours[i];
				avg_neigh_dy[i]/=n_neighbours[i];
			}
		}

		// speed update
		// the following operations will all act on the boid's speed alone
		// "make boid do this" here means
		// "update boid speed in a way that will make it go towards doing this"

#pragma omp for
		for(size_t i = 0; i<n_boids; ++i) {
			// velocity starting point
			next_dx[i] = curr_dx[i];
			next_dy[i] = curr_dy[i];

			// make boid center itself with respect to its neighbours
			if(n_neighbours[i] > 0) {
				next_dx[i] += (center_x[i] - curr_x[i]) * centering_factor;
				next_dy[i] += (center_y[i] - curr_y[i]) * centering_factor;
			}

			// make boid avoid collisions
			next_dx[i] += repulse_x[i] * dont_slam_factor;
			next_dy[i] += repulse_y[i] * dont_slam_factor;

			// make boid go at the same speed as its neighbours
			if(n_neighbours[i] > 0) {
				next_dx[i] += (avg_neigh_dx[i] - curr_dx[i])\
					* speed_match_factor;
				next_dy[i] += (avg_neigh_dy[i] - curr_dy[i])\
					* speed_match_factor;
			}

			// make boid stay within window bounds
			if(curr_x[i] > window_width - turnback_margin)
				next_dx[i] -= turnback_factor;
			if(curr_y[i] > window_height - turnback_margin)
				next_dy[i] -= turnback_factor;

			if(curr_x[i] < turnback_margin)
				next_dx[i] += turnback_factor;
			if(curr_y[i] < turnback_margin)
				next_dy[i] += turnback_factor;

			// clamp boid acceleration within an acceptable range
			float ax = next_dx[i] - curr_dx[i];
			float ay = next_dy[i] - curr_dy[i];
			float a = sqrt(ax*ax + ay*ay);
			if(a > acceleration_upper_bound) {
				ax*=acceleration_upper_bound/a;
				ay*=acceleration_upper_bound/a;

				next_dx[i] = curr_dx[i]+ax;
				next_dy[i] = curr_dy[i]+ay;
			}

			// clamp boid speed within an acceptable range
			float v = sqrt(next_dx[i]*next_dx[i] + 
						   next_dy[i]*next_dy[i]);
			if(v > speed_upper_bound) {
				next_dx[i] *= speed_upper_bound/v;
				next_dy[i] *= speed_upper_bound/v;
			}

			if(v < speed_lower_bound) {
				next_dx[i] *= speed_lower_bound/v;
				next_dy[i] *= speed_lower_bound/v;
			}
		}
	}
}

void draw_boid_coords(const float x, const float y,
					  const float dx, const float dy) {
	static const float triangle_length=10.5;
	static const float triangle_width=6.8;
 
	float speed_norm = sqrt((dx*dx) + (dy*dy));
	float normalized_dx = dx / speed_norm;
	float normalized_dy = dy / speed_norm;
 
	Vector2 tip = {
		.x = x + (normalized_dx * triangle_length),
		.y = y + (normalized_dy * triangle_length),
	};
	Vector2 right = {
		.x = x - (normalized_dy * triangle_width / 2),
		.y = y + (normalized_dx * triangle_width / 2),
	};
	Vector2 left = {
		.x = x + (normalized_dy * triangle_width / 2),
		.y = y - (normalized_dx * triangle_width / 2),
	};
 
	// vertices must be specified in counterclockwise order
	DrawTriangle(tip, left, right, WHITE);
}
 
void boids_draw_all(size_t n_boids,
					const float x[n_boids], const float y[n_boids],
					const float dx[n_boids], const float dy[n_boids]) {
	for(size_t i = 0; i<n_boids; ++i) {
		draw_boid_coords(x[i], y[i], dx[i], dy[i]);
	}
}

void swap_buffers(float** a, float** b) {
	float* tmp = *a;
	*a = *b;
	*b = tmp;
}
 
int main(int argc, char** argv) {
	// sets a bunch of global configuration variables
	// used throughout the program
	parse_args(argc, argv);

	float boids_a_x[num_boids];
	float boids_a_y[num_boids];
	float boids_a_dx[num_boids];
	float boids_a_dy[num_boids];

	float boids_b_x[num_boids];
	float boids_b_y[num_boids];
	float boids_b_dx[num_boids];
	float boids_b_dy[num_boids];

	randomize_boids(num_boids,
					boids_a_x, boids_a_y,
					boids_a_dx, boids_a_dy);
 
 	InitWindow(window_width, window_height, "nomen fenetrae");
 	SetTargetFPS(window_fps);
 
 	while(!WindowShouldClose()) {
		// swapping buffers is a pain 
		// just run it twice
 		BeginDrawing();
		ClearBackground(BLACK);

		boids_draw_all(num_boids,
					   boids_a_x,  boids_a_y,
					   boids_a_dx, boids_a_dy);

		boids_update(num_boids,
					 boids_a_x, boids_a_y,
					 boids_a_dx, boids_a_dy,
					 boids_b_x, boids_b_y,
					 boids_b_dx, boids_b_dy);

		window_width = GetScreenWidth();
		window_height = GetScreenHeight();
		EndDrawing();


 		BeginDrawing();
		ClearBackground(BLACK);

		boids_draw_all(num_boids,
					   boids_b_x,  boids_b_y,
					   boids_b_dx, boids_b_dy);

		boids_update(num_boids,
					 boids_b_x, boids_b_y,
					 boids_b_dx, boids_b_dy,
					 boids_a_x, boids_a_y,
					 boids_a_dx, boids_a_dy);

		window_width = GetScreenWidth();
		window_height = GetScreenHeight();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

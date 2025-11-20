#include"args.h"            // some global variables and argv parsing
#include"graphics-common.h" // raylib and some boid draw logic

#include"soa.h"
 
void boids_draw_all(size_t n_boids,
					const float x[n_boids], const float y[n_boids],
					const float dx[n_boids], const float dy[n_boids]) {
	for(size_t i = 0; i<n_boids; ++i) {
		draw_boid_coords(x[i], y[i], dx[i], dy[i]);
	}
}
 
int main(int argc, char** argv) {
	// sets a bunch of global configuration variables
	// used throughout the program
	handle_argv_vars(argc, argv);

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

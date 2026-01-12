#include"args.h"            // some global config variables, and parse_args
#include"graphics-common.h" // raylib, and some drawing logic

#include"aos.h"

void draw_boid_struct(const boid b) {
	draw_boid_coords(b.curr_x, b.curr_y, b.curr_dx, b.curr_dy);
}
 
void boids_draw_all(size_t nboids, boid boids[nboids]) {
	for(size_t i = 0; i<nboids; ++i) {
		draw_boid_struct(boids[i]);
	}
}
 
int main(int argc, char** argv) {
	// sets a bunch of global configuration variables
	// used throughout the program
	handle_argv_vars(argc, argv);

	boid boids[num_boids];
	randomize_boids(num_boids, boids);
 
 	InitWindow(window_width, window_height, "nomen fenetrae");
 	SetTargetFPS(window_fps);
 
 	while(!WindowShouldClose()) {
 		BeginDrawing();
		ClearBackground(BLACK);

		boids_draw_all(num_boids, boids);
		boids_update_all(num_boids, boids);

		// make boid simulation adapt to window resizing
		// updating here avoids weird race conditions
		window_width = GetScreenWidth();
		window_height = GetScreenHeight();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

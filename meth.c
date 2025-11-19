/* trying to get omp to compile with various bits of parallel part
 * throws around different functions seems to be causing some problems
 * this is a simplified boid-y thing I'm doing to
 * incrementally fuck around with openmp until I can get it to do
 * what I'm trying to do on the boids program, it's just the boids program
 * has a bit too many moving parts to work as a testing ground
 */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

#include<omp.h>

#include"raylib.h"

typedef struct {
    float x;
    float y;
} point;

float distance_squared(const point p1, const point p2) {
    float dx = p1.x - p2.x; 
    float dy = p1.y - p2.y; 
    return (dx*dx) + (dy*dy);
}
float distance(const point p1, const point p2) {
    return sqrt(distance_squared(p1, p2));
}

void update(size_t i, size_t npoints, point points[npoints]) {
    float center_x = 0;
    float center_y = 0;
    int n_neighbours = 0;
    printf("hello I am %d outside nested parallel\n",
           omp_get_thread_num());

#pragma omp for reduction(+:center_x, center_y, n_neighbours)
    for(size_t j = 0; j<npoints; ++j) {
        printf("hello I am %d inside nested parallel\n",
               omp_get_thread_num());
        if(i!=j && distance(points[i], points[j]) < 200) {
            center_x += points[j].x;
            center_y += points[j].y;
            n_neighbours++;
        }
    }

    printf("stats: %f, %f, %d\n", center_x, center_y, n_neighbours);

    if(n_neighbours != 0) {
        // move towards center but keep speed within bounds
        static const float max_v = 1.0;
        static const float min_v = 0.3;
        center_x /= n_neighbours;
        center_y /= n_neighbours;

        // towards center
        float dx = center_x - points[i].x;
        float dy = center_y - points[i].y;

        float v = sqrt((dx*dx)+(dy*dy));
        if(v>max_v) {
            dx*=max_v/v;
            dy*=max_v/v;
        }
        if(v<min_v){
            dx*=min_v/v;
            dy*=min_v/v;
        }

        points[i].x += dx;
        points[i].y += dy;
    }
}

float random_uniform(float min, float max) {
    float random_normalized = (float)random() / (float)INT_MAX;
    return (random_normalized * (max - min)) + min;
}

void init_points(size_t npoints, point points[npoints],
                 float minx, float maxx, float miny, float maxy) {
    for(size_t i = 0; i<npoints; ++i) {
        points[i].x = random_uniform(minx, maxx);
        points[i].y = random_uniform(miny, maxy);
    }
}

void display_all(size_t npoints, point points[npoints]) {
    for(size_t i = 0; i<npoints; ++i) {
        DrawCircleV((Vector2){.x = points[i].x, .y=points[i].y}, 1, WHITE);
    }
}

void update_all(size_t npoints, point points[npoints]) {
#pragma omp for
    for(int i = 0; i<npoints; ++i) {
        printf("hello I am %d\n", omp_get_thread_num());
        update(i, npoints, points);
    }
}

int main() {
    InitWindow(800, 800, "test");
    SetTargetFPS(24);
    static const size_t npoints = 10;
    point ps[npoints];
    init_points(npoints, ps, 0, 800, 0, 800);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        display_all(npoints, ps);

#pragma omp parallel
        update_all(npoints, ps);
        
        EndDrawing();
    }
    CloseWindow();
}

#include "benchmark-common.h"
#include "global-vars.h"

long long current_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

void file_create_header(FILE* f) {
    fputs("\"milliseconds\","
          "\"nboids\","
          "\"nthreads\","
          "\"warmup iterations\","
          "\"timed iterations\","
          "\"neighbourhood radius\","
          "\"safety distance\"\n",
          f);
}

void file_log_bench_results(FILE* f, long long time_taken) {
    fprintf(f,"%lld,%d,%d,%d,%d,%f,%f\n",
            time_taken,
            num_boids,
            num_threads,
            num_warmup_iterations,
            num_iterations,
            neighbour_radius,
            too_close_radius);
}

void log_to_file(const char* pathname, long long time_taken) {
    if(access(pathname, F_OK)) {
        // file did not previously exist
        FILE* f = fopen(pathname, "a");
        file_create_header(f);
        file_log_bench_results(f, time_taken);
        fclose(f);
    }
    else {
        // file did previously exist
        FILE* f = fopen(pathname, "a");
        file_log_bench_results(f, time_taken);
        fclose(f);
    }
}

#ifndef BOID_BENCHMARK_COMMON_H
#define BOID_BENCHMARK_COMMON_H

#include<stdio.h>     // printf&Co.
#include<unistd.h>    // access

#include <sys/time.h> // gettimeofday

long long current_millis();
void file_create_header(FILE* f);
void file_log_bench_results(FILE* f, long long time_taken);
void log_to_file(const char* pathname, long long time_taken);

#endif

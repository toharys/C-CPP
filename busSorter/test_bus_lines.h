#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif //EX2_REPO_TESTBUSLINES_H
/**
 * TODO add documentation
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);
/**
 * a function that checks if the bubble sort function succeed
 * return 1 if yes else 0
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);
/**
 * a function that receives two BusLine arrays and returns 1 if they
 * contains the same values else returns 0
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif //EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H

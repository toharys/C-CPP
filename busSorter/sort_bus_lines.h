#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
/**
 * a struct that resemble bus line object
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * function that receives pointers to the end and the start of BusLines
 * dynamic array and sorted it in bubble sort by their distance
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
* function that receives pointers to the end and the start of BusLines
 * dynamic array and sorted it in quick sort by their duration time
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * TODO add documentation
 */
BusLine *partition (BusLine *start, BusLine *end);
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H

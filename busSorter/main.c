#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUBBLE "bubble"
#define QUICK "quick"
#define TEST "test"
#define LINENUM_ERROR "ERROR: Number of lines should be a positive integer\n"
#define LINES_REQ "Enter number of lines. Then enter\n"
#define INFO_REQ "Enter line info. Then enter\n"
#define LINE_ERROR "ERROR: Line number should be an integer" \
                            " between 1 and 999 (includes)\n"
#define DISTANCE_ERROR "ERROR: distance should be an integer" \
                            " between 0 and 100 (includes)\n"
#define DURATION_ERROR "ERROR: Duration should be an integer "\
                            "between 10 and 100 (includes)\n"
#define MIN_LINE 1
#define MAX_LINE 999
#define MIN_DISTANCE 0
#define MAX_DISTANCE 1000
#define MIN_DURATION 10
#define MAX_DURATION 100
#define BUFF_LNGTH 60
#define TEN_BASE 10
#define NEXT -10
#define USAGE_ERROR "USAGE: plz insert a not_valid command" \
                         " (test, bubble, quick)"
#define B_SUCCESS "TEST 1 PASSED: bubble sort passed successfully"
#define B_FAIL "TEST 1 FAILED: bubble sort failed"
#define EQUAL_AFTERB "TEST 2 PASSED: the original and sorted arrays equals"
#define NEQUAL_AFTERB "TEST 2 FAILED: original and sorted arrays arnt equals"
#define Q_SUCCESS "TEST 3 PASSED: quick sort passed successfully"
#define Q_FAIL "TEST 3 FAILED: quick sort failed"
#define EQUAL_AFTERQ "TEST 4 PASSED: the original and sorted arrays equals"
#define NEQUAL_AFTERQ "TEST 4 FAILED: original and sorted arrays arnt equals"
# define A_LNGTH 2
# define MODE_INDEX 1

/**
 * a function that receives an input arr, checks it and returns the
 * requested command
 * @param argv  the input array
 */
char* run_mode(char *argv[])
{
    if (strcmp(argv[MODE_INDEX],BUBBLE)==0)
    {
        return BUBBLE;  // if the requested command is bubble sort
    }
    else if (strcmp(argv[MODE_INDEX],QUICK)==0)
    {
        return QUICK;   // if the requested command is quick sort
    }
    else if (strcmp(argv[MODE_INDEX],TEST)==0)
    {
        return TEST;    // if the requested command is test
    }
    // if the argument isn't not_valid prints error msg and return error value
    fprintf(stdout,USAGE_ERROR);
    return USAGE_ERROR;
}

/**
 * a function that makes validation checks on line info
 * @return 0 if pass else prints an error msg and return 1
 */
int not_valid(int line_number, int distance, int duration)
{
    if (line_number<MIN_LINE || line_number>MAX_LINE)
    {   //check if the line number in the right span
        fprintf(stdout,LINE_ERROR);
        return EXIT_FAILURE;
    }
    else if (distance<MIN_DISTANCE || distance>MAX_DISTANCE)
    {   //check if the distance is in the right span
        fprintf(stdout,DISTANCE_ERROR);
        return EXIT_FAILURE;
    }
    else if (duration<MIN_DURATION || duration>MAX_DURATION)
    {   //check if the duration is in the right span
        fprintf(stdout,DURATION_ERROR);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * a function that as input the number of lines and return it
 * @return the lines number as int
 */
int get_lines_num()
{
    char buffer[BUFF_LNGTH];
    fprintf(stdout, LINES_REQ); // prints request msg
    fgets(buffer, BUFF_LNGTH, stdin); //gets the lines num as input
    long lines_num = strtol(buffer, NULL, TEN_BASE);
    return (int)lines_num;  //converts it to int type and return it
}

/**
 * a function that receives the data on each bus line and convert it to int
 * variables
 * @param line_number
 * @param distance
 * @param duration
 * @return 0 if success to convert the info types to int else 1
 */
int get_line_info(int* line_number, int* distance, int* duration)
    {
        char buffer[BUFF_LNGTH];
        char* temp = NULL;
        fprintf(stdout,INFO_REQ);
        fgets(buffer,BUFF_LNGTH,stdin); // gets the line number as input
        *line_number = (int)strtol(buffer,&temp,TEN_BASE);
        *distance = (int)strtol(temp+1,&temp,TEN_BASE);
        *duration = (int)strtol(temp+1,NULL,TEN_BASE);
        if (not_valid(*line_number, *distance, *duration))
        {   // if the input's value not_valid
            return NEXT;
        }
        return EXIT_SUCCESS;
    }

/**
 * a function that runs tests in case that the run mode is test and prints
 * the test's results
 * @param start
 * @param arr_lngth
 * @return EXIT FAILURE if didnt success to make a copy to the
 * array else EXIT SUCCESS
 */
int test_mode(BusLine* busLine, int arr_lngth)
{   // initialize a copy array to the original one
    BusLine* dest = calloc(arr_lngth,
                           sizeof(*dest)*arr_lngth);
    if (dest==NULL)
    {   // if the allocation failed
        free(dest);
        return EXIT_FAILURE;
    }
    memcpy(dest, busLine, arr_lngth*sizeof(BusLine));
    BusLine* original_end = dest+arr_lngth-1;
    BusLine* end = busLine+arr_lngth-1;
    bubble_sort(busLine,end-1); // make sort for tests 1 and 2
    if (is_sorted_by_distance(busLine,end))   //test 1
    {
        fprintf(stdout,"%s\n",B_SUCCESS);
    }
    else
    {
        fprintf(stdout,"%s\n",B_FAIL);
    }
    if (is_equal(busLine, end,dest,original_end))   //test 2
    {
        fprintf(stdout,"%s\n",EQUAL_AFTERB);
    }
    else
    {
        fprintf(stdout,"%s\n",NEQUAL_AFTERB);
    }
    quick_sort(busLine,end-1); //make sort for tests 3 and 4
    if(is_sorted_by_duration(busLine,end))    // test 3
    {
        fprintf(stdout,"%s\n",Q_SUCCESS);
    }
    else
    {
        fprintf(stdout,"%s\n",Q_FAIL);
    }
    if (is_equal(busLine, end,dest,original_end))   //test 4
        {
        fprintf(stdout,"%s",EQUAL_AFTERQ);
        }
    else
    {
        fprintf(stdout,"%s",NEQUAL_AFTERQ);
    }
    free(dest);
    return EXIT_SUCCESS;
}

/**
 * a function that receives an array of BusLines and prints it in the
 * requested format
 * @param start
 * @param arr_lngth
 */
void print_bus_arr(BusLine* start, int arr_lngth)
{
    int i =0;
    for (; i<arr_lngth;i++)
    {
        BusLine curr = *(start+i);
        fprintf(stdout,"%d,%d,%d\n",
                curr.line_number,curr.distance,curr.duration);
    }
}

/**
 * a function that runs the program in bubble mode
 * @param start
 * @param arr_lngth
 */
void bubble_mode(BusLine* start, int arr_lngth)
{
    BusLine* end = (start+arr_lngth-1);
    bubble_sort(start,end);
    print_bus_arr(start,arr_lngth);
}

/**
 * a function that runs the program in quick mode
 * @param start
 * @param arr_lngth
 */
void quick_mode(BusLine* start, int arr_lngth)
{
    BusLine* end = (start+arr_lngth -1);
    quick_sort(start,end);
    print_bus_arr(start,arr_lngth);
}

int main (int argc, char *argv[])
{
    if (argv==NULL || argc!=A_LNGTH)
    {
        return EXIT_FAILURE;
    }
    char* run_command = run_mode(argv); // gets the requested command
    if (strcmp(run_command, USAGE_ERROR) == 0)
    {   // if the command isn't not_valid exit the program
        return EXIT_FAILURE;
    }
    int lines_num = (int)get_lines_num();
    while (lines_num<=0)   //gets new line num while its invalid
    {
        fprintf(stdout, LINENUM_ERROR);
        lines_num = (int)get_lines_num();
    }
    BusLine* arr_buses = malloc(sizeof (BusLine)*lines_num);
    if(arr_buses==NULL)
    {
        free(arr_buses);
        return EXIT_FAILURE;
    }
    for(int i=0;i<lines_num;i++)
    {
        int line_number = 0, distance = 0, duration = 0;
        int flag = get_line_info(&line_number, &distance, &duration);
        if (flag==EXIT_SUCCESS)
        {
            arr_buses[i].line_number=line_number;
            arr_buses[i].distance=distance;
            arr_buses[i].duration=duration;
        }
        if (flag==NEXT)
        {
            i--;
        }
    }
    if (strcmp(run_command,TEST)==0)
    {
        int res = test_mode(arr_buses,lines_num);
        return res;
    }
    else if (strcmp(run_command, BUBBLE)==0)
    {
        bubble_mode(arr_buses,lines_num);
    }
    else if (strcmp(run_command,QUICK)==0)
    {
        quick_mode(arr_buses,lines_num);
    }
    free(arr_buses);
    return EXIT_SUCCESS;
}

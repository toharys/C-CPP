#include "test_bus_lines.h"
#define TRUE 1
#define FALSE 0

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
    int arr_lngth =(int)(end-start);//calculate length of given arr
    for (int i=0; i<arr_lngth; i++)   // moves on each line in the array
    {
        int temp = (start+i)->distance;//the curr line distance
        for (int j=i+1; j<arr_lngth;j++)
        {   //moves on each end array index that after the curr index
            if (((start+j)->distance)<temp)
                {   //if smaller distance found return 0
                    return FALSE;
                }
            }
    }
    return TRUE;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
    int arr_lngth =(int)(end-start);//calculate length of given arr
    for (int i=0; i<arr_lngth; i++)   // moves on each line in the array
        {
        int temp = (start+i)->duration;//the curr line duration
        for (int j=i+1; j<arr_lngth;j++)
        {//moves on each end array index that after the curr index
            if (((start+j)->duration)<temp)
            {   //if smaller duration found return 0
                return FALSE;
            }
        }
        }
    return TRUE;
}

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{   // the next two lines calculates the dynamic arrays length
    int sorted_lngth = (int)(end_sorted-start_sorted);
    int original_lngth = (int)(end_original-start_original);
    if (sorted_lngth != original_lngth)
    {
        return FALSE;
    }
    for (int i=0;i<original_lngth;i++)
    {   //for each BusLine variable in the original array
        int curr = (start_original+i)->line_number;// the curr line number
        int flag = FALSE;
        for (int j=0;j<sorted_lngth;j++)
        {   // move on all the sorted array
          int temp = (start_sorted+j)->line_number;
          if (temp==curr)   // if the same line found
          {
              flag = TRUE;
          }
        }
        if (!flag)//if there curr line (from the original) isn't in the sorted
        {
            return flag;
        }
    }
    return TRUE;
}
#include "sort_bus_lines.h"

void swap(BusLine* pntr_1, BusLine* pntr_2)
{
    BusLine temp = *pntr_1;
    *pntr_1 = *pntr_2;
    *pntr_2 = temp;
}

void bubble_sort (BusLine *start, BusLine *end)
{
    int i = 0;
    for (BusLine* curr = start; curr<end; curr++)
    {
        // Last i elements are already
        // in place
        for (BusLine* j=start; j<(end-i); j++)
        {
            if (j->distance > (j+1)->distance)
            {
                swap(j,(j+1));
            }
        }
        i++;
    }
}

BusLine* partition (BusLine *start, BusLine *end)
{
    BusLine* pivot = end;    // choose the most right var in the array
    int i= -1;
    for (BusLine *k=start; k<end;k++)
    {
        if ((k->duration)<=(pivot->duration))
        {
            i++;
            swap(start+i,k);
        }
    }
    swap(start+i+1,end);
    return start+i+1;
}


void quick_sort (BusLine* start, BusLine* end)
{
    if(start<end)
    {
        BusLine* pivot = partition(start, end);  // finding the pivot index
        quick_sort(start,pivot-1);
        quick_sort(pivot+1,end);
    }
}




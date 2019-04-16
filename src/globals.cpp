#include "globals.h"


bool is_in_array (int number, int arr[], int how_many)      // is the delivered number found also in the given array?
{
     for (int i = 1; i < how_many; i++)                     // one by one, we check all the numbers in the array (but arr[0])
     {
         if (arr[i] == number)
            return true;
     }

     return false;
}


void copy_array (int arr_from[], int arr_into[], int size)  // we copy one array into another (i.e. this way we save an order of the unsorted drawn and hit numbers)
{
    for (int i = 1; i < size; i++)
        arr_into[i] = arr_from[i];
}


void sort_numbers (int arr[], int begin, int end)           // sorting with the Quicksort algorithm
{
    int i, j, center, help;

    i = begin;                                              // "pointer" to beginning
    j = end;                                                // "pointer" to end
    center = arr[(begin + end) / 2];                        // we find the center element by an integer division: (x+y) / 2

    do {
        while (arr[i] < center)                             // as long as the "i-th" element of the array is smaller than the center element, keep moving "i" to the right
            i++;
        while (center < arr[j])                             // as long as the "j-th" element of the array is bigger than the center element, keep moving "j" to the left
            j--;

        if (i <= j)                                         // if "i" is on a position smaller or equal to "j"
        {
            help = arr[i];
            arr[i] = arr[j];                                // we swap the values of the "i-th" and the "j-th" element
            arr[j] = help;

            i++;                                            // we move "i" to the right
            j--;                                            // we move "j" to the left
        }
    } while (i <= j);                                       // we keep doing it as long as "i" is to the left or at the same place as "j"

    if (begin < j)
        sort_numbers (arr, begin, j);                       // if "j" hasn't gone to the starting position, we sort from the beginning to "j"
    if (i < end)
        sort_numbers (arr, i, end);                         // if "i" hasn't gone to the final position, we sort from "i" to the end
}

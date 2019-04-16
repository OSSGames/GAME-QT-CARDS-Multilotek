#ifndef GLOBALS_H
#define GLOBALS_H


const float STAKE_MULTI_LOTEK = 2.5;                    // base stake for Multi Lotek
const int STAKE_MULTI_LOTEK_PLUS = 5;                   // base stake for Multi Lotek Plus
const int RANGE = 80;                                   // how many balls are there
const int HOW_MANY_DRAWN = 20;                          // how many balls are drawn
const int HOW_MANY_CHOSEN_MAX = 10;                     // what is the maximum quantity of balls that can be chosen


/**************************************************************************/


// array with prize values for hits in Multi Lotek
const int prizes_table[HOW_MANY_CHOSEN_MAX+1][HOW_MANY_CHOSEN_MAX+1] =
{
    // the table is build upon a pattern of [quantity_of_chosen_numbers][quantity_of_hit_numbers]
    // all prizes are given for a multiple of stake equaled 1
    // for instance - we chose 6 numbers, we hit 5 numbers, we won 120 zł ("złoty" or PLN is a Polish currency)
    // cells which are equal to zero mean "no prize"

    {0, 0,  0,   0,    0,   0,    0,    0,     0,     0,      0},       // [0][0-10]
    {0, 4,  0,   0,    0,   0,    0,    0,     0,     0,      0},       // [1][0-10] and so on
    {0, 0, 16,   0,    0,   0,    0,    0,     0,     0,      0},
    {0, 0,  2,  54,    0,   0,    0,    0,     0,     0,      0},
    {0, 0,  2,   8,   84,   0,    0,    0,     0,     0,      0},
    {0, 0,  0,   4,   20, 700,    0,    0,     0,     0,      0},
    {0, 0,  0,   2,    8, 120, 1300,    0,     0,     0,      0},
    {0, 0,  0,   2,    4,  20,  200, 6000,     0,     0,      0},
    {0, 0,  0,   0,    4,  20,   60,  600, 22000,     0,      0},
    {0, 0,  0,   0,    2,   8,   42,  300,  2000, 70000,      0},
    {0, 0,  0,   0,    2,   4,   12,  140,   520, 10000, 250000}

};


// prizes for hitting Plus in Multi Lotek Plus
const int prizes_plus_table[HOW_MANY_CHOSEN_MAX+1][HOW_MANY_CHOSEN_MAX+1] =
{
    {0, 0,    0,   0,   0,    0,    0,     0,      0,      0,       0},
    {0, 84,   0,   0,   0,    0,    0,     0,      0,      0,       0},
    {0, 24, 104,   0,   0,    0,    0,     0,      0,      0,       0},
    {0, 18,  26, 160,   0,    0,    0,     0,      0,      0,       0},
    {0, 16,  14,  40, 300,    0,    0,     0,      0,      0,       0},
    {0, 14,  10,  16,  60, 1100,    0,     0,      0,      0,       0},
    {0, 14,  10,  10,  12,  200, 3000,     0,      0,      0,       0},
    {0, 14,   8,   6,  10,   50,  500, 16000,      0,      0,       0},
    {0, 14,   4,   4,  10,   28,  120,  1200, 108000,      0,       0},
    {0, 14,   4,   4,   4,   14,   80,   600,   8000, 230000,       0},
    {0, 14,   4,   4,   4,    8,   24,   240,   1000,  40000, 2250000}
};


/**************************************************************************/


template <class type>
void clear_array (type* array, int size)                // template function for clearing any array
{
    for (int i = 0; i < size; i++)
        array[i] = 0;
}


template <class type>
void super_delete (type &object)                        // template function for deleting objects and clearing pointers
{
    delete object;
    object = 0;
}


/**************************************************************************/


bool is_in_array (int number, int arr[], int how_many);  // is the delivered number found also in the given array?
void copy_array (int arr_from[], int arr_into[], int size);  // we copy one array into another (i.e. this way we save an order of the unsorted drawn and hit numbers)
void sort_numbers (int arr[], int begin, int end);      // sorting with the Quicksort algorithm

#endif // GLOBALS_H

/*
    This file is part of Multilotek.
    Copyright (C) 2009 Arkadiusz Piekarz
    Contact: piekarzarkadiusz@gmail.com

    Multilotek is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Multilotek is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Multilotek.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QMessageBox>
#include "drawing.h"

/**********************************************************************/

// Constructor


drawing::drawing (mainwindow* main_window_arg)
{
    main_window = main_window_arg;

    clear_array (chosen, HOW_MANY_CHOSEN_MAX+1);
    how_many_chosen = 0;

    already_performed_drawing = false;
    already_performed_drawing_plus = false;
    do_sorting = false;
    already_drawn_sorted = false;
    already_hit_sorted = false;
    do_coloring = true;

    last_drawn_number = 0;
    how_many_hit = 0;
    prize_normal = 0;
    is_plus_hit = false;
    prize_plus = 0;

    clear_array (drawn, HOW_MANY_DRAWN+1);
    clear_array (drawn_sort, HOW_MANY_DRAWN+1);
    clear_array (hit, HOW_MANY_CHOSEN_MAX+1);
    clear_array (hit_sort, HOW_MANY_CHOSEN_MAX+1);
}


/**********************************************************************/

// Performing the lottery drawing


void drawing::perform_drawing (bool drawing_method)
{
    if (drawing_method == your_own)                                   // if we are drawing with our own numbers (typed by hand)
    {                                                                 // we copy the chosen numbers into an array
        how_many_chosen = main_window->ui->horizontalSlider_how_many_chosen->value();

        for (int i = 1; i <= how_many_chosen; i++)                    // we begin from 1 instead of 0 to easier understand
            chosen[i] = main_window->spinbox_arr[i]->value();         // which number we are working on (and we do so in the whole application)

        if (!check_chosen())                                          // if any numbers are repeated, we show an error and don't draw
            return;
    }

    // we draw 20 random numbers
    fill_with_random();


    // we check if we hit something and calculate the prizes
    // also, we check if we have to sort the numbers and color them
    check_hit_normal();
    check_prize_normal();

    check_if_sort(main_window->ui->action_sort_numbers->isChecked());

    if (main_window->playing_with_plus)
    {
        check_hit_plus();
        check_prize_plus();
    }

    check_if_color(main_window->ui->action_color_numbers->isChecked());


    // we print the results of drawing in the window
    print_all();


    // we send a signal to perform calculations of statistics
    emit calculate_statistics();                                      // at first I used a pointer to the "stat" object, but it was causing a crash :-/
}


void drawing::draw_hit_or_miss()
{
    how_many_chosen = (rand() % 10 + 1);                              // we draw how many numbers we will choose
    main_window->ui->horizontalSlider_how_many_chosen->setValue(how_many_chosen);

    int number;
    for (int i = 1; i <= how_many_chosen; i++)                        // we fill a proper quantity of spinboxes with random numbers
    {
        do {
            number = (rand() % RANGE + 1);                            // we take the number modulo RANGE, so it is in a proper range (and we add 1)
        } while (is_in_array(number, chosen, i));                     // we check if we have already drawn that number earlier

        chosen[i] = number;                                           // if not, we save it in the array
        main_window->spinbox_arr[i]->setValue(number);                // and we set that value in a spinbox
    }

    perform_drawing(hit_or_miss);                                     // we perform the rest of the drawing as usual (we send an argument that we're playing hit-or-miss)
}


bool drawing::check_chosen()                                          // are all chosen numbers different from each other?
{
    bool are_different = true;
    QString str;

    for (int i = 1; i <= how_many_chosen; i++)
    {

        if (is_in_array(chosen[i], chosen, i))                        // we call a separate global function to search through the array
        {
            are_different = false;                                    // if at least one number is repeated, we change that to "false"
            str += tr("Number %1 is repeated!\n").arg(chosen[i]);
        }
    }

    if (!are_different)                                               // if a number is repeated, we show an error
    {
        str += tr("\nThe numbers have been chosen incorrectly!");
        emit show_error (tr("Error"), str);
    }

    return are_different;
}


/**********************************************************************/

// Drawing the numbers


void drawing::fill_with_random()                            // we fill the drawn[] array with random numbers
{                                                           // we use HOW_MANY_DRAWN and RANGE constants
    for (int i = 1; i < HOW_MANY_DRAWN + 1; i++)
    {
        int number;

        do {
            number = (rand() % RANGE + 1);                  // we take the number modulo RANGE, so it is in a proper range (and we add 1)
        } while (is_in_array(number, drawn, i));            // we check if we have already drawn that number earlier

        drawn[i] = number;                                  // if not, we save it in the array
    }

    last_drawn_number = drawn[HOW_MANY_DRAWN];              // we save the last drawn number
    already_performed_drawing = true;

    if (main_window->playing_with_plus)
        already_performed_drawing_plus = true;              // it is needed for coloring and activating the Plus labels
    else
        already_performed_drawing_plus = false;

    already_drawn_sorted = false;
}


/**********************************************************************/

// Checking the hits


void drawing::check_hit_normal()                            // we check if we have hit the normal numbers
{
    /*
    we take in turns numbers from chosen[] and check the drawn[] array, whether that number is also found there
    if it is, we save it in the hit[] array and we increase a "result" variable
    */

    int result = 0;                                         // how many times have we hit
    int j = 1;                                              // numbering of an index of the hit[] array

    for (int i = 1; i <= how_many_chosen; i++)
    {
        if (is_in_array (chosen[i], drawn, HOW_MANY_DRAWN + 1))  // in turns - do the chosen numbers coincide with the drawn numbers?
        {
           hit[j] = chosen[i];                              // if they do, we save the given number in the hit[] array
           result++;
           j++;                                             // we increase the index of the hit numbers
        }
    }

    already_hit_sorted = false;
    how_many_hit = result;
}


void drawing::check_hit_plus()                              // we check if we have hit the Plus
{
    /*
    if the last drawn number has been also chosen by us, we hit the Plus - so we won!
    */

    if (is_in_array (last_drawn_number, hit, how_many_hit+1))
        is_plus_hit = true;
    else
        is_plus_hit = false;
}


/**********************************************************************/

// Sorting the numbers

void drawing::change_sorting()                              // we sort or unsort the drawn out numbers
{
    if (check_if_sort(main_window->ui->action_sort_numbers->isChecked()))
    {
        print_drawn();
        print_hit_normal();
    }
}

bool drawing::check_if_sort (bool sort_arg)                 // do we have to sort the drawn and hit numbers or to bring back the unsorted shape
{
    do_sorting = sort_arg;                                  // we save here whether we have to sort, or unsort (or do nothing)

    if (!already_performed_drawing)                         // if we haven't drawn even once yet, we return
        return false;                                       // the value of "do_sorting" has been already changed, so we're good

    if (do_sorting && !already_drawn_sorted)                // if we have to sort and haven't sorted the array yet
    {
        copy_array (drawn, drawn_sort, HOW_MANY_DRAWN + 1);
        sort_numbers (drawn_sort, 1, HOW_MANY_DRAWN);
        already_drawn_sorted = true;                        // we change the value of the variable here and not in the "sort_numbers() function", because the latter is recursive

        if (how_many_hit && !already_hit_sorted)            // if we have hit something and haven't sorted the array yet
        {
            copy_array (hit, hit_sort, how_many_chosen + 1);
            sort_numbers (hit_sort, 1, how_many_hit);
            already_hit_sorted = true;
        }

    }

    return true;
}


/**********************************************************************/

// Coloring the numbers


void drawing::change_coloring()                             // we turn on or off the coloring of numbers
{
    if (check_if_color(main_window->ui->action_color_numbers->isChecked()))
    {
        print_drawn();
        print_hit_normal();
        if (main_window->ui->radioButton_plus->isChecked())
        {
            print_hit_plus();
            print_prize_plus();
        }
    }
}


bool drawing::check_if_color (bool color_arg)               // do we have to turn on the coloring of the drawn and hit numbers
{
    do_coloring = color_arg;

    if (!already_performed_drawing)                         // if we haven't drawn yet
        return false;
    else
        return true;
}


/**********************************************************************/

// Calculating the prize


void drawing::check_prize_normal()                           // we calculate height of the prize for normal Multi Lotek
{
    prize_normal = (main_window->multiple * prizes_table[how_many_chosen][how_many_hit]);
}


void drawing::check_prize_plus()                             // we calculate height of the prize for Multi Lotek Plus
{
    if (is_plus_hit)
        prize_plus = (main_window->multiple * prizes_plus_table[how_many_chosen][how_many_hit]);
    else
        prize_plus = 0;
}


/**********************************************************************/

// Printing to the window


void drawing::print_all()                                           // we print to every label associated with drawing
{
    print_drawn();
    print_hit_normal();
    print_prize_normal();

    if (main_window->playing_with_plus)
    {
        print_hit_plus();
        print_prize_plus();
    }
}


void drawing::print_drawn()                                         // we print the drawn numbers
{
    QString text;
    int* draw_ptr;

    if (do_sorting)                                                 // if we have to sort, we will get numbers from an appropriate array
        draw_ptr = drawn_sort;
    else
        draw_ptr = drawn;

    if (do_coloring && main_window->playing_with_plus)              // if we have to color the numbers and we're playing with Plus
    {
        for (int i = 1; i < HOW_MANY_DRAWN + 1; i++)                // we make a loop for the drawn numbers
        {
            if (is_in_array (draw_ptr[i], hit, how_many_hit+1))     // we check which numbers from the drawn ones we have hit and we color them
            {
                if (draw_ptr[i] != last_drawn_number)               // if the number being checked is different from the last drawn number
                    text += "<font color='#14b520'><b>" + QString::number(draw_ptr[i]) + "</b></font>";  // we use a green color
                else
                    text += "<font color='#ff0000'><b>" + QString::number(draw_ptr[i]) + "</b></font>";  // if the number is the Plus, we color it with red
            }
            else                                                    // if we haven't hit a given number, we color it ordinarily with black
            {
                if (draw_ptr[i] != last_drawn_number)               // if the number is different from the last drawn number
                    text += QString::number(draw_ptr[i]);
                else                                                // if it's the Plus, we make it bold, even though we haven't hit it
                    text += "<b>" + QString::number(draw_ptr[i]) + "</b>";
            }

            if (i != HOW_MANY_DRAWN)
                    text += ", ";                                   // we put commas in appropriate places
        }
    }
    else if (!do_coloring && main_window->playing_with_plus)        // if we don't have to color the numbers, but we're playing with Plus
    {
        for (int i = 1; i < HOW_MANY_DRAWN + 1; i++)                // we make a loop for the drawn numbers
        {
            if (draw_ptr[i] != last_drawn_number)                   // if the number is different from the last drawn number
                text += QString::number(draw_ptr[i]);
            else
                text += "<b>" + QString::number(draw_ptr[i]) + "</b>";  // if it's the Plus, we make it bold, regardless of whether we have hit it or not

            if (i != HOW_MANY_DRAWN)
                text += ", ";                                       // we put commas in appropriate places
        }
    }
    else if (do_coloring && !main_window->playing_with_plus)        // if we have to color the numbers and we're playing normal Multi Lotek
    {
        for (int i = 1; i < HOW_MANY_DRAWN + 1; i++)                // we make a loop for the drawn numbers
        {
            if (is_in_array (draw_ptr[i], hit, how_many_hit+1))     // we check which numbers from the drawn ones we have hit and we color them
                text += "<font color='#14b520'><b>" + QString::number(draw_ptr[i]) + "</b></font>";
            else                                                    // if we haven't hit a given number, we color it ordinarily with black
                text += QString::number(draw_ptr[i]);

            if (i != HOW_MANY_DRAWN)
                    text += ", ";                                   // we put commas in appropriate places
        }
    }
    else                                                            // if we don't have to color and we're playing normal Multi Lotek
    {
        for (int i = 1; i < HOW_MANY_DRAWN + 1; i++)                // we make a loop for the drawn numbers
        {
            text += QString::number(draw_ptr[i]);                   // we print all drawn numbers ordinarily with black

            if (i != HOW_MANY_DRAWN)
                text += ", ";                                       // we put commas in appropriate places
        }
    }

    main_window->ui->label_drawn->setText(text);
}


void drawing::print_hit_normal()                                    // we print the hit numbers
{
    QString text;
    int* hit_ptr;

    if (do_sorting)                                                 // if we have to sort, we will get numbers from an appropriate array
        hit_ptr = hit_sort;
    else
        hit_ptr = hit;

    if (how_many_hit)                                               // if we have hit something
    {
        if (do_coloring)                                            // if we have to color the numbers
        {
            for (int i = 1; i <= how_many_hit; i++)                 // we print the results in turns from the hit[] array
            {
                text += "<font color='#14b520'><b>" + QString::number(hit_ptr[i]) + "</b></font>";  // we color the hit numbers with green

                if (i < how_many_hit)
                    text += ", ";                                   // we put commas in appropriate places
            }
        }
        else                                                        // if we don't have to color the numbers
        {
            for (int i = 1; i <= how_many_hit; i++)
            {
                text += QString::number(hit_ptr[i]);                // we print all hit numbers ordinarily with black

                if (i < how_many_hit)
                    text += ", ";
            }
        }
    }
    else
        text = tr("none");

    main_window->ui->label_hit_normal->setText(text);
}


void drawing::print_prize_normal()                                  // we print the height of the prize for normal Multi Lotek
{
    QString text =QString::number(prize_normal) + QString::fromUtf8(" zł");
    main_window->ui->label_prize_normal->setText(text);
}


void drawing::print_hit_plus()                                      // we print if we have hit the Plus
{
    if (!already_performed_drawing_plus)                            // have we already performed the drawing with the Plus (it is needed for coloring and for activating the Plus labels)
        return;

    QString text;

    if (is_plus_hit)                                                // if we have hit the Plus
    {
        if (do_coloring)
            text = "<font color='#ff0000'><b>" + QString::number(last_drawn_number) + "</b></font>";  // we color the Plus with red
        else
            text = "<b>" + QString::number(last_drawn_number) + "</b>";
    }
    else                                                            // if we haven't hit the Plus
        text = tr("none");

    main_window->ui->label_hit_plus->setText(text);
}


void drawing::print_prize_plus()                                    // we print the height of the prize for Multi Lotek Plus
{
    if (!already_performed_drawing_plus)                            // have we already performed the drawing with the Plus (it is needed for coloring and for activating the Plus labels)
        return;

    QString text = QString::number(prize_plus) + QString::fromUtf8(" zł");
    main_window->ui->label_prize_plus->setText(text);
}

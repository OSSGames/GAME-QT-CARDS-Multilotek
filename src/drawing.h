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


#ifndef DRAWING_H
#define DRAWING_H

#include <QObject>
#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class mainwindow;
class statistics;

class drawing: public QObject
{
    Q_OBJECT

private:
    mainwindow* main_window;                            // pointer to the "mainwindow" class

    int chosen[HOW_MANY_CHOSEN_MAX+1];                  // array with the numbers chosen by the user
    int how_many_chosen;                                // how many numbers user has chosen
    enum { your_own, hit_or_miss };

    bool already_performed_drawing;                     // have we already performed at least one drawing
    bool already_performed_drawing_plus;                // have we already performed drawing in Plus mode (needed for coloring and activating Plus labels
    bool do_sorting;                                    // do we have to do sorting of the drawn numbers
    bool already_drawn_sorted;                          // do we already have a ready array with sorted drawn numbers
    bool already_hit_sorted;                            // do we already have a ready array with sorted hit numbers
    bool do_coloring;                                   // do we have to color the drawn and hit numbers

    int last_drawn_number;                              // we save the last drawn number, important at sorting
    int how_many_hit;                                   // how many numbers have we hit
    int prize_normal;                                   // the height of the normal prize
    bool is_plus_hit;                                   // have we hit the Plus
    int prize_plus;                                     // the height of the prize for hitting Plus

    int drawn[HOW_MANY_DRAWN+1];                        // array for (default) 20 drawn numbers
    int drawn_sort[HOW_MANY_DRAWN+1];                   // array for (default) 20 drawn and sorted numbers
    int hit[HOW_MANY_CHOSEN_MAX+1];                     // array for the hit numbers
    int hit_sort[HOW_MANY_CHOSEN_MAX+1];                // array for hit and sorted numbers

    bool check_chosen();                                // are all chosen numbers different from each other?
    void fill_with_random();                            // we fill the drawn[] array with random numbers
    bool check_if_sort (bool sort_arg);                 // do we have to sort the drawn and hit numbers or to bring back the unsorted shape
    bool check_if_color (bool color_arg);               // do we have to turn on the coloring of the drawn and hit numbers

    void check_hit_normal();                            // we check if we have hit the normal numbers
    void check_prize_normal();                          // we calculate height of the prize for normal Multi Lotek
    void check_hit_plus();                              // we check if we have hit the Plus
    void check_prize_plus();                            // we calculate height of the prize for normal Multi Lotek Plus

    void print_all();                                   // we print to every label associated with drawing
    void print_drawn();                                 // we print the drawn numbers
    void print_hit_normal();                            // we print the hit numbers
    void print_prize_normal();                          // we print the height of the prize for normal Multi Lotek
    void print_hit_plus();                              // we print if we have hit the Plus
    void print_prize_plus();                            // we print the height of the prize for Multi Lotek Plus
    
public:
    drawing(mainwindow* main_window_arg);               // constructor

    friend class mainwindow;                            // we declare friendship with the class "mainwindow"
    friend class statistics;                            // we declare friendship with the class "statistics"
    
public slots:
    void perform_drawing (bool drawing_method = your_own);  // we launch the drawing after the button is pressed
    void draw_hit_or_miss();                            // we launch the drawing for the hit-or-miss mode
    void change_sorting();                              // we sort or unsort the drawn out numbers
    void change_coloring();                             // we turn on or off the coloring of numbers

signals:
    void show_error (QString title, QString text);      // we send a signal to show an error when some of the chosen numbers are the same
    void calculate_statistics();                        // we send a signal to "stat" object to calculate statistics
};


#endif // DRAWING_H

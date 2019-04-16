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


#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include "mainwindow.h"

class drawing;
class mainwindow;

class statistics : public QObject
{
    Q_OBJECT

private:
    mainwindow* main_window;                // pointer to the "mainwindow" class
    drawing* draw;                          // pointer to the "drawing" class

    long how_many_drawings;                 // how many drawings have been performed so far
    long how_many_drawings_normal;          // how many drawings for normal Multi Lotek have been performed so far
    long how_many_drawings_plus;            // how many drawings for Multi Lotek Plus have been performed so far
    long how_many_drawings_with_hits;       // how many drawings have been performed, where there was at least 1 number hit

    long won_sum;                           // how much money have we won in sum
    double spent_sum;                       // how much money have we spent in sum
    double balance;                         // the difference between the sum of won and the sum of spent money

    int max_prize_normal;                   // what is the biggest normal prize we have won
    int max_prize_plus;                     // what is the bigges prize we have won for hitting Plus
    int max_how_many_hit;                   // what is the biggest quantity of numbers we have hit

    long won_normal_sum;                    // how much money we have won from normal prizes in sum
    long won_plus_sum;                      // how much money we have won for hitting Plus
    double avg_prize_normal;                // what is the average of all normal prizes we have won
    double avg_prize_plus;                  // what is the average of all prizes for hitting Plus
    double avg_spent;                       // what is the acerage of all spent money

    int drawn_frequency[RANGE+1];           // a table with frequencies of appearence of the drawn numbers
    int most_frequently_drawn[RANGE+1];     // a table with the most frequently drawn numbers
    int top_freq_drawn_value;               // the highest value of frequency of drawn numbers

    int hit_frequency[RANGE+1];             // a table with frequencies of appearence of the hit numbers
    int most_frequently_hit[RANGE+1];       // a table with the most frequently hit numbers
    int top_freq_hit_value;                 // the highest value of frequency of drawn numbers


    void initial_calculations();            // we do some initial calculations
    void calculate_balance() { balance = won_sum - spent_sum; }  // we calculate balance

    // we check if any records have been beaten
    void check_max_prize_normal();
    void check_max_prize_plus();
    void check_max_how_many_hit();

    // we calculate some averages
    void calculate_avg_prize_normal() ;
    void calculate_avg_prize_plus();
    void calculate_avg_spent();

    // we calculate the most frequent numbers
    void calculate_most_freq_drawn();
    void calculate_most_freq_hit();


    /******************************************/

    // we print the results in the main window
    void print_spent_sum();
    void print_won_sum();
    void print_balance();

    void print_max_prize_normal();
    void print_max_prize_plus();
    void print_max_how_many_hit();

    void print_avg_prize_normal();
    void print_avg_prize_plus();
    void print_avg_spent();

    void print_most_freq_drawn();
    void print_most_freq_hit();

    /******************************************/


    void reset_statistics();                // we reset the statistics

public:
    statistics(mainwindow* main_window_arg, drawing* draw_arg);  // constructor
    void print_all_statistics();                        // we print all statistics

    friend class export_import;                         // we declare friendship with a class "export_import"
    friend class mainwindow;                            // and with a class "mainwindow"

public slots:
    void perform_all_calculations();                    // we conduct performing all calculations
    void reset_all_statistics();                        // we conduct resetting all statistics
};

#endif // STATISTICS_H

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


#include "statistics.h"

/**********************************************************************/

// Constructor


statistics::statistics(mainwindow* main_window_arg, drawing* draw_arg)
{
    main_window = main_window_arg;
    draw = draw_arg;

    how_many_drawings = 0;
    how_many_drawings_normal = 0;
    how_many_drawings_plus = 0;
    how_many_drawings_with_hits = 0;

    won_sum = 0;
    spent_sum = 0;
    won_normal_sum = 0;
    won_plus_sum = 0;
    balance = 0;

    max_prize_normal = 0;
    max_prize_plus = 0;
    max_how_many_hit = 0;

    avg_prize_normal = 0;
    avg_prize_plus = 0;
    avg_spent = 0;

    clear_array (drawn_frequency, RANGE+1);
    clear_array (most_frequently_drawn, RANGE+1);
    top_freq_drawn_value = 0;

    clear_array (hit_frequency, RANGE+1);
    clear_array (most_frequently_hit, RANGE+1);
    top_freq_hit_value = 0;
}


/**********************************************************************/

// Calculations of different statistics


void statistics::perform_all_calculations()
{
    initial_calculations();

    calculate_balance();

    check_max_prize_normal();
    if (main_window->playing_with_plus) check_max_prize_plus();
    check_max_how_many_hit();

    calculate_avg_prize_normal();
    if (main_window->playing_with_plus) calculate_avg_prize_plus();
    calculate_avg_spent();

    calculate_most_freq_drawn();
    calculate_most_freq_hit();


    // we print the statistics in the window
    print_spent_sum();
    print_won_sum();
    print_balance();

    print_max_prize_normal();
    if (main_window->playing_with_plus) print_max_prize_plus();
    print_max_how_many_hit();

    print_avg_prize_normal();
    if (main_window->playing_with_plus) print_avg_prize_plus();
    print_avg_spent();

    print_most_freq_drawn();
    print_most_freq_hit();
}


void statistics::initial_calculations()
{
    how_many_drawings++;

    if (main_window->playing_with_plus)
        how_many_drawings_plus++;
    else
        how_many_drawings_normal++;

    spent_sum += main_window->stake;
    won_sum += draw->prize_normal + draw->prize_plus;
    won_normal_sum += draw->prize_normal;
    won_plus_sum += draw->prize_plus;

    if (draw->how_many_hit)
        how_many_drawings_with_hits++;
}


void statistics::check_max_prize_normal() {
        if (max_prize_normal < draw->prize_normal) max_prize_normal = draw->prize_normal; }

void statistics::check_max_prize_plus() {
        if (max_prize_plus < draw->prize_plus) max_prize_plus = draw->prize_plus; }

void statistics::check_max_how_many_hit() {
        if (max_how_many_hit < draw->how_many_hit) max_how_many_hit = draw->how_many_hit; }


void statistics::calculate_avg_prize_normal() {
        avg_prize_normal = static_cast<double>(won_normal_sum) / how_many_drawings; }

void statistics::calculate_avg_prize_plus() {
        avg_prize_plus = static_cast<double>(won_plus_sum) / how_many_drawings_plus; }

void statistics::calculate_avg_spent() {
        avg_spent = static_cast<double>(spent_sum) / how_many_drawings; }


void statistics::calculate_most_freq_drawn()
{
    // we prepare the most_frequently_drawn[] array by resetting it
    clear_array (most_frequently_drawn, RANGE+1);

    // we have earlier copied an array with the drawn numbers
    // if number x has been drawn, we increase the value in drawn_frequency[x] cell
    for (int i = 1; i < HOW_MANY_DRAWN+1; i++)
        drawn_frequency[draw->drawn[i]] += 1;

    // we scan the drawn_frequency[] array for the highest value
    for (int i = 1; i < RANGE+1; i++)
    {
        if ( top_freq_drawn_value < drawn_frequency[i])
            top_freq_drawn_value = drawn_frequency[i];
    }

    // once we have found the highest value of frequency, we search which numbers have been drawn so many times
    int j = 1;
    for (int i = 1; i < RANGE+1; i++)
    {
        if (drawn_frequency[i] == top_freq_drawn_value)
        {
            most_frequently_drawn[j] = i;
            j++;
        }
    }
}


void statistics::calculate_most_freq_hit()
{
    // if no numbers have been hit, we don't do any calculations
    if (draw->how_many_hit == 0)
        return;

    // we prepare the most_frequently_hit[] array by resetting it
    clear_array (most_frequently_hit, RANGE+1);

    // we have earlier copied an array with the hit numbers
    // if number x has been hit, we increase the value in hit_frequency[x] cell
    for (int i = 1; i < draw->how_many_hit+1; i++)
        hit_frequency[draw->hit[i]] += 1;

    // we scan the hit_frequency[] array for the highest value
    for (int i = 1; i < RANGE+1; i++)
    {
        if ( top_freq_hit_value < hit_frequency[i])
            top_freq_hit_value = hit_frequency[i];
    }

    // once we have found the highest value of frequency, we search which numbers have been hit so many times
    int j = 1;
    for (int i = 1; i < RANGE+1; i++)
    {
        if (hit_frequency[i] == top_freq_hit_value)
        {
            most_frequently_hit[j] = i;
            j++;
        }
    }
}


/**********************************************************************/

// Printing the results in the window


void statistics::print_all_statistics()
{
    print_won_sum();
    print_spent_sum();
    print_balance();

    print_max_prize_normal();
    print_max_prize_plus();
    print_max_how_many_hit();

    print_avg_prize_normal();
    print_avg_prize_plus();
    print_avg_spent();

    print_most_freq_drawn();
    print_most_freq_hit();
}


void statistics::print_spent_sum()
{
    QString text = QString::number(spent_sum) + QString::fromUtf8(" zł");
    main_window->ui->label_spent_sum->setText(text);
}


void statistics::print_won_sum()
{
    QString text = QString::number(won_sum) + QString::fromUtf8(" zł");
    main_window->ui->label_won_sum->setText(text);
}


void statistics::print_balance()
{
    QString text;

    if (balance > 0)
        text = "<font color='#14b520'><b>" + QString::number(balance) + QString::fromUtf8("</b></font> zł");
    else if (balance == 0)
        text = "<font color='#0a84f6'><b>" + QString::number(balance) + QString::fromUtf8("</b></font> zł");
    else
        text = "<font color='#ff0000'><b>" + QString::number(balance) + QString::fromUtf8("</b></font> zł");

    main_window->ui->label_balance->setText(text);
}


void statistics::print_max_prize_normal()
{
    QString text = QString::number(max_prize_normal) + QString::fromUtf8(" zł");
    main_window->ui->label_max_prize_normal->setText(text);
}


void statistics::print_max_prize_plus()
{
    QString text = QString::number(max_prize_plus) + QString::fromUtf8(" zł");
    main_window->ui->label_max_prize_plus->setText(text);
}


void statistics::print_max_how_many_hit()
{
    QString text = QString::number(max_how_many_hit);
    main_window->ui->label_max_how_many_hit->setText(text);
}


/*
  below, for printing averages, we use an overloaded number() method with three arguments:
  the variable, an output format (x.xx in this case) and a precision (two digits after a decimal)
*/

void statistics::print_avg_prize_normal()
{
    QString text = QString::number(avg_prize_normal, 'f', 2) + QString::fromUtf8(" zł");
    main_window->ui->label_avg_prize_normal->setText(text);
}


void statistics::print_avg_prize_plus()
{
    QString text = QString::number(avg_prize_plus, 'f', 2) + QString::fromUtf8(" zł");
    main_window->ui->label_avg_prize_plus->setText(text);
}


void statistics::print_avg_spent()
{
    QString text = QString::number(avg_spent, 'f', 2) + QString::fromUtf8(" zł");
    main_window->ui->label_avg_spent->setText(text);
}


void statistics::print_most_freq_drawn()
{
    QString text;

    if (how_many_drawings == 0)
        text = tr("none");
    else if (how_many_drawings == 1)
        text = tr("draw at least 2 times");
    else
    {
        for (int i = 1; most_frequently_drawn[i] != 0; i++)
        {
            if (i%11 == 0)
                text += "\n";
            text += QString::number(most_frequently_drawn[i]);
            if (most_frequently_drawn[i+1] != 0)
                text += ", ";
        }
    }

    main_window->ui->label_most_freq_drawn->setText(text);
}


void statistics::print_most_freq_hit()
{
    QString text;

    if (how_many_drawings_with_hits == 0)
        text = tr("none");
    if (how_many_drawings_with_hits == 1)
        text = tr("hit at least 2 times");
    else
    {
        for (int i = 1; most_frequently_hit[i] != 0; i++)
        {
            if (i%11 == 0)
                text += "\n";
            text += QString::number(most_frequently_hit[i]);
            if (most_frequently_hit[i+1] != 0)
                text += ", ";
        }
    }

    main_window->ui->label_most_freq_hit->setText(text);
}


/**********************************************************************/

// Resetting the statistics

void statistics::reset_all_statistics()
{
    reset_statistics();
    print_all_statistics();
}


void statistics::reset_statistics()
{   
    how_many_drawings = 0;
    how_many_drawings_normal = 0;
    how_many_drawings_plus = 0;
    how_many_drawings_with_hits = 0;

    won_sum = 0;
    spent_sum = 0;
    won_normal_sum = 0;
    won_plus_sum = 0;
    balance = 0;

    max_prize_normal = 0;
    max_prize_plus = 0;
    max_how_many_hit = 0;

    avg_prize_normal = 0;
    avg_prize_plus = 0;
    avg_spent = 0;

    clear_array (drawn_frequency, RANGE+1);
    clear_array (most_frequently_drawn, RANGE+1);
    top_freq_drawn_value = 0;

    clear_array (hit_frequency, RANGE+1);
    clear_array (most_frequently_hit, RANGE+1);
    top_freq_hit_value = 0;
}

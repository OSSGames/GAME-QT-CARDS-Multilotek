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


#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "export_import.h"

/**********************************************************************/

// Constructor


export_import::export_import(mainwindow* main_window_arg, statistics* stat_arg)
{
    if (main_window_arg != 0)
        main_window = main_window_arg;
    if (stat_arg != 0)
        stat = stat_arg;

    test_int = 0;
    test_double = 0;
    test_long = 0;
}


/**********************************************************************/

// Export of the data to a file


void export_import::export_data()                                       // exporting the data
{
    QFile file("data.ini");                                             // we set the name of the file

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))             // if we fail to open the file, we show an error and return
    {
        QString error = tr("Creation or modification of the file data.ini has failed!");
        emit show_error (tr("Error"), error);

        return;
    }

    QTextStream exp(&file);                                             // we set a text stream into thw file

    // we write a name of the variable, then insert whitespaces, then its value
    exp << "current_locale\t" << main_window->current_locale << "\n";
    exp << "how_many_drawings\t" << stat->how_many_drawings << "\n";
    exp << "how_many_drawings_normal\t" << stat->how_many_drawings_normal << "\n";
    exp << "how_many_drawings_plus\t" << stat->how_many_drawings_plus << "\n";
    exp << "how_many_drawings_with_hits\t" << stat->how_many_drawings_with_hits << "\n";

    exp << "won_sum\t" << stat->won_sum << "\n";
    exp << "spent_sum\t" << stat->spent_sum << "\n";
    exp << "balance\t" << stat->balance << "\n";

    exp << "max_prize_normal\t" << stat->max_prize_normal << "\n";
    exp << "max_prize_plus\t" << stat->max_prize_plus << "\n";
    exp << "max_how_many_hit\t" << stat->max_how_many_hit << "\n";

    exp << "won_normal_sum\t" << stat->won_normal_sum << "\n";
    exp << "won_plus_sum\t" << stat->won_plus_sum << "\n";
    exp << "avg_prize_normal\t" << stat->avg_prize_normal << "\n";
    exp << "avg_prize_plus\t" << stat->avg_prize_plus << "\n";
    exp << "avg_spent\t" << stat->avg_spent << "\n";

    exp << "drawn_frequency\t";                                         // we export contents of an array
        for (int i = 0; i < RANGE+1; i++)
            exp << stat->drawn_frequency[i] << " ";
        exp << "\n";
    exp << "most_frequently_drawn\t";
        for (int i = 0; i < RANGE+1; i++)
            exp << " " << stat->most_frequently_drawn[i];
        exp << "\n";
    exp << "top_freq_drawn_value\t" << stat->top_freq_drawn_value << "\n";

    exp << "hit_frequency\t";
        for (int i = 0; i < RANGE+1; i++)
            exp << stat->hit_frequency[i] << " ";
        exp << "\n";
    exp << "most_frequently_hit\t";
        for (int i = 0; i < RANGE+1; i++)
            exp << " " << stat->most_frequently_hit[i];
        exp << "\n";
    exp << "top_freq_hit_value\t" << stat->top_freq_hit_value;

    file.close();
}


/**********************************************************************/

// Import of the data from a file


void export_import::import_data()
{
    if (import_data_test())
    {
        import_data_real();
        stat->print_all_statistics();
    }
}


bool export_import::import_data_test()                                  // testing the correctness of the data to be imported
{
    QFile file("data.ini");                                             // we set the name of the file

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))              // we try to open the text file with read-only attribute
    {
        QString error = tr("Opening of the file data.ini has failed!\nMake sure it exists.");
        emit show_error (tr("Error"), error);                           // if we failed, we show an error and return

        return false;
    }
    if (file.atEnd())                                                   // if the file is empty, we show an error and return
    {
        QString error = tr("The file data.ini is empty!");
        emit show_error (tr("Error"), error);

        file.close();
        return false;
    }

    QTextStream imp(&file);
    QString word = "";

    while (!imp.atEnd())                                                // for more information what we are doing here, look at import_data_real() function
    {
        imp >> word;

        if (word == "current_locale")
            imp >> test_string; else

        if (word == "how_many_drawings" || word == "how_many_drawings_normal" || word == "how_many_drawings_plus"
        || word == "how_many_drawings_with_hits" || word == "won_sum" || word == "won_normal_sum"
        || word == "won_plus_sum")
            imp >> test_long; else

        if (word == "spent_sum" || word == "balance" || word == "avg_prize_normal" || word == "avg_prize_plus"
        || word == "avg_spent")
            imp >> test_double; else


        if (word == "max_prize_normal" || word == "max_prize_plus" || word == "max_how_many_hit"
        || word == "top_freq_drawn_value" || word == "top_freq_hit_value")
            imp >> test_int; else


        if (word == "drawn_frequency" || word == "most_frequently_drawn" || word == "hit_frequency"
        || word == "most_frequently_hit")
            for (int i = 0; i < RANGE+1; i++) imp >> test_int;


        if (imp.status())                                               // if some operation of inserting a value into the variable failed, we show an error and return
        {
            QString error;

            if (word == "drawn_frequency" || word == "most_frequently_drawn" || word == "hit_frequency"
            || word == "most_frequently_hit")                           // if it is an array
            {
                error = tr("Loading some value for \"%1\" array has failed!\n\n"
                           "Import of the data has been aborted.").arg(word);
            }
            else                                                        // if it is a variable
            {
                error = tr("Loading a value for \"%1\" variable has failed!\n\n"
                           "Import of the data has been aborted.").arg(word);
            }

            emit show_error (tr("Error"), error);

            file.close();
            return false;
        }
    }

    file.close();
    return true;
}


void export_import::import_data_real()                                  // importing the data
{
    QFile file("data.ini");                                             // we set the name of the file

    file.open(QIODevice::ReadOnly | QIODevice::Text);                   // we try to open the text file with read-only attribute

    QTextStream imp(&file);                                             // we set a text stream from the file
    QString word = "";

    while (!imp.atEnd())                                                // we do the loop until we reach the end of the file
    {
        imp >> word;                                                    // we read one word which points us to a variable

        if (word == "current_locale")                                   // if we found the given name of the variable, we read another "word" (which contains a value) and write it into the variable
            imp >> main_window->new_locale; else
        if (word == "how_many_drawings")                                // (we ignore white spaces between the words)
            imp >> stat->how_many_drawings; else
        if (word == "how_many_drawings_normal")
            imp >> stat->how_many_drawings_normal; else
        if (word == "how_many_drawings_plus")
            imp >> stat->how_many_drawings_plus; else
        if (word == "how_many_drawings_with_hits")
            imp >> stat->how_many_drawings_with_hits; else

        if (word == "won_sum")
            imp >> stat->won_sum; else
        if (word == "spent_sum")
            imp >> stat->spent_sum; else
        if (word == "balance")
            imp >> stat->balance; else

        if (word == "max_prize_normal")
            imp >> stat->max_prize_normal; else
        if (word == "max_prize_plus")
            imp >> stat->max_prize_plus; else
        if (word == "max_how_many_hit")
            imp >> stat->max_how_many_hit; else

        if (word == "won_normal_sum")
            imp >> stat->won_normal_sum; else
        if (word == "won_plus_sum")
            imp >> stat->won_plus_sum; else
        if (word == "avg_prize_normal")
            imp >> stat->avg_prize_normal; else
        if (word == "avg_prize_plus")
            imp >> stat->avg_prize_plus; else
        if (word == "avg_spent")
            imp >> stat->avg_spent; else

        if (word == "drawn_frequency")                                  // we copy numbers into an array
        {
            for (int i = 0; i < RANGE+1; i++)
                imp >> stat->drawn_frequency[i];
        } else
        if (word == "most_frequently_drawn")
        {
            for (int i = 0; i < RANGE+1; i++)
                imp >> stat->most_frequently_drawn[i];
        } else
        if (word == "top_freq_drawn_value")
            imp >> stat->top_freq_drawn_value; else

        if (word == "hit_frequency")
        {
            for (int i = 0; i < RANGE+1; i++)
                imp >> stat->hit_frequency[i];
        } else
        if (word == "most_frequently_hit")
        {
            for (int i = 0; i < RANGE+1; i++)
                imp >> stat->most_frequently_hit[i];
        } else
        if (word == "top_freq_hit_value")
                imp >> stat->top_freq_hit_value;
    }

    file.close();
}


QString export_import::import_locale()                                  // importing only the language (locale) setting
{
    // we don't do much "error throwing" here, because we want this function to be rather silent for the user

    QFile file("data.ini");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "none";

    if (file.atEnd())
        return "none";

    QTextStream imp(&file);
    QString word;

    while (!imp.atEnd())
    {
        imp >> word;

        if (word == "current_locale")
            imp >> test_string;

        if (imp.status())
        {
            file.close();
            return "none";
        }
    }

    if (test_string == "")
    {
        file.close();
        return "none";
    }

    file.close();
    return test_string;                              // if the testing went fine, we can send the right string back
}

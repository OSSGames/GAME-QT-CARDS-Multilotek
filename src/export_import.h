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


#ifndef EXPORT_IMPORT_H
#define EXPORT_IMPORT_H

#include <QObject>
#include "globals.h"
#include "mainwindow.h"
#include "statistics.h"

class export_import : public QObject
{
    Q_OBJECT

private:
    mainwindow* main_window;                        // pointer to a class "mainwindow"
    statistics* stat;                               // pointer to a class "statistics"

    // variables for testing the type of a value from a file
    int test_int;
    double test_double;
    long test_long;
    QString test_string;

    bool import_data_test();                        // testing types of variables
    void import_data_real();                        // importing the data into real variables


public:
    export_import(mainwindow* main_window_arg = 0, statistics* stat_arg = 0);  // constructor
    QString import_locale();                        // we import only the language (locale) setting
    void export_data();                             // we conduct exporting of the data into a file
    void import_data();                             // we conduct importing of the data from a file

signals:
    void show_error (QString title, QString text);  // we send a signal to show an error when some operations on files fail
};

#endif // EXPORT_IMPORT_H

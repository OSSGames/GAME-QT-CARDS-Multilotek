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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QSpinBox>
#include <QTranslator>
#include "about.h"
#include "drawing.h"
#include "export_import.h"
#include "globals.h"
#include "statistics.h"
#include "ui_mainwindow.h"

class drawing;
class statistics;
class export_import;


namespace Ui
{
    class mainwindow;                   // the main window form
}

class mainwindow : public QMainWindow
{
    Q_OBJECT

private:
    about* about_form;                  // pointer to an "about author" form
    Ui::mainwindow* ui;                 // pointer to the window elements, i.e. buttons
    drawing* draw;                      // pointer to an object of a class "drawing"
    statistics* stat;                   // pointer to an object of a class "statistics"
    export_import* exp_imp;             // pointer to an object of a class "export_import"

    QTranslator* translator;            // pointer to an object of a class "QTranslator"
    QString current_locale;             // the current locale (language)
    QString new_locale;                 // proposition of a new locale to change the current one

    QSpinBox* spinbox_arr[11];          // array with the pointers to spinboxes
    bool playing_with_plus;             // are we playing a normal Multi Lotek or Multi Lotek Plus
    int multiple;                       // what multiple of stake are we playing with
    float stake;                        // how big is the stake (payment for a lottery ticket)
    QMessageBox message_box;            // object of a class "QMessageBox" (for showing the rules)

    void translation_on_startup();      // we automatically perform a translation of GUI when it launches
    void change_language();             // we change the language of GUI (i.e. after importing the data)
    void changeEvent (QEvent* event);   // redefined changeEvent() function for retranslating GUI
    void print_rules();                 // we print the rules in the "Learn the rules" dialog (needed for dynamic translation)

public:
    mainwindow (QWidget* parent = 0);   // constructor
    ~mainwindow();                      // destructor

    friend class drawing;               // we declare friendship with the class "drawing"
    friend class export_import;         // and with the class "export_import"
    friend class statistics;            // and with the class "statistics"

private slots:
    void activate_plus_labels();        // we activate or deactivate the labels showing if we have hit the Plus and how much have we won for Plus
    void set_multiple_text();           // we set how big is the multiple of stake, depending on the location of a slider nearby
    void set_stake_text();              // we calculate how big is the stake, depending on the mode of the game and the multiple and show it in the label
    void set_how_many_chosen_text();    // we set a quantity of numbers the user wants to choose, depending on the location of the second slider nearby
    void activate_spinboxes();          // we acivate or deactivate the spinboxes, depending on how many numbers the user wants to choose

    void export_data();                 // we export the data into a file
    void import_data();                 // we import the data from a file
    void change_language (QAction* action);  // we change the language of GUI (i.e. after choosing it in the menu)
    void show_author();                 // we show information about the author of the program

public slots:
    // showing different message boxes
    void show_rules();
    void show_error (QString title, QString text) { QMessageBox::critical(this, title, text); }
};

#endif // MAINWINDOW_H

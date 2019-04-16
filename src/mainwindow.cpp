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


#include <QDesktopWidget>
#include "mainwindow.h"

/**********************************************************************/

// Constructor, destructor and translator

mainwindow::mainwindow(QWidget *parent)                 // constructor
    : QMainWindow(parent), ui(new Ui::mainwindow), translator(0)
{
    ui->setupUi(this);
    translation_on_startup();


    // we set the initial sizes of the window, depending on the operating system
    #ifdef Q_WS_WIN
        QRect window_geometry(0, 0, 562, 648);
    #endif

    #ifdef Q_WS_X11
        QRect window_geometry(0, 0, 660, 648);
    #endif

    // we center the location of the main window
    QDesktopWidget *desktop = QApplication::desktop();

    window_geometry.moveTop((desktop->availableGeometry().height()/2) - (window_geometry.height()/2) + 20);
    window_geometry.moveLeft((desktop->availableGeometry().width()/2) - (window_geometry.width()/2));

    this->setGeometry(window_geometry);


    // we create objects of given classes and we save their addresses in appropriate pointers
    draw = new drawing(this);
    stat = new statistics(this, draw);


    // we save information about languages in menu actions
    ui->action_english->setData(QString("en_US"));
    ui->action_polish->setData(QString("pl_PL"));

    // we save pointers to spinboxes in a table
    spinbox_arr[1] = ui->spinBox1;
    spinbox_arr[2] = ui->spinBox2;
    spinbox_arr[3] = ui->spinBox3;
    spinbox_arr[4] = ui->spinBox4;
    spinbox_arr[5] = ui->spinBox5;
    spinbox_arr[6] = ui->spinBox6;
    spinbox_arr[7] = ui->spinBox7;
    spinbox_arr[8] = ui->spinBox8;
    spinbox_arr[9] = ui->spinBox9;
    spinbox_arr[10] = ui->spinBox10;

    // we set initial values of some variables
    playing_with_plus = false;
    multiple = 1;
    stake = 2.5;


    // handling of window elements such as buttons and sliders
    connect (ui->radioButton_plus, SIGNAL (toggled(bool)), this, SLOT (activate_plus_labels()));
    connect (ui->radioButton_plus, SIGNAL (toggled(bool)), this, SLOT (set_stake_text()));
    connect (ui->horizontalSlider_multiple, SIGNAL (valueChanged(int)), this, SLOT (set_multiple_text()));
    connect (ui->horizontalSlider_multiple, SIGNAL (valueChanged(int)), this, SLOT (set_stake_text()));
    connect (ui->horizontalSlider_how_many_chosen, SIGNAL (valueChanged(int)), this, SLOT (set_how_many_chosen_text()));
    connect (ui->horizontalSlider_how_many_chosen, SIGNAL (valueChanged(int)), this, SLOT (activate_spinboxes()));
    connect (ui->pushButton_draw, SIGNAL (clicked()), draw, SLOT (perform_drawing()));
    connect (ui->pushButton_draw_hit_or_miss, SIGNAL (clicked()), draw, SLOT (draw_hit_or_miss()));
    connect (ui->pushButton_rules, SIGNAL (clicked()), this, SLOT (show_rules()));

    // handling of menu, that is File, Edit and so on
    connect (ui->action_save_data, SIGNAL (triggered()), this, SLOT (export_data()));
    connect (ui->action_load_data, SIGNAL (triggered()), this, SLOT (import_data()));
    connect (ui->action_reset_statistics, SIGNAL (triggered()), stat, SLOT (reset_all_statistics()));
    connect (ui->action_close, SIGNAL (triggered()), this, SLOT (close()));
    connect (ui->action_sort_numbers, SIGNAL (triggered()), draw, SLOT (change_sorting()));
    connect (ui->action_color_numbers, SIGNAL (triggered()), draw, SLOT (change_coloring()));
    connect (ui->submenu_language, SIGNAL (triggered(QAction*)), this, SLOT (change_language(QAction*)));
    connect (ui->action_about_author, SIGNAL (triggered()), this, SLOT (show_author()));

    // we create a connection between "drawing" and "mainwindow" classes
    connect (draw, SIGNAL (show_error(QString, QString)), this, SLOT (show_error(QString, QString)));

    // we create a connection between "drawing" and "statistics" classes
    connect (draw, SIGNAL (calculate_statistics()), stat, SLOT (perform_all_calculations()));
}


mainwindow::~mainwindow()           // destructor
{
    delete stat;                    // we delete the object of a class "statistics"
    delete draw;                    // and the object of a class "drawing"
    delete ui;                      // and the object of a "mainwindow" form
    delete translator;              // and the object of a class "QTranslator", if it exists
}


void mainwindow::translation_on_startup()       // we automatically perform a translation of GUI when it launches
{
    /*
      we get the current system locale (language and country)
      firstly, we look into data.ini file; if that fails, we check the system locale
      we load a proper translation based on locale into the QTranslator object
      if no proper translation is found, we will fallback to using text from sources (English)
      we install the translation into a pool of translations used by the application itself
    */

    exp_imp = new export_import();

    new_locale = exp_imp->import_locale();
    if (new_locale == "none" && new_locale != "pl_PL" && new_locale != "en_US")
        new_locale = QLocale::system().name();

    if (new_locale == "pl_PL")
    {
        ui->action_polish->setChecked(true);
        translator = new QTranslator(this);
        translator->load(QString("multilotek_") + new_locale, QString(":/translations/"));
        qApp->installTranslator(translator);
        current_locale = new_locale;
    }
    else
    {
        ui->action_english->setChecked(true);
        current_locale = "en_US";
    }

    delete exp_imp;
}


/**********************************************************************/

// Handling of window elements


void mainwindow::activate_plus_labels()                                // we activate or deactivate the labels showing if we have hit the Plus and how much have we won for Plus
{
    if (ui->radioButton_plus->isChecked())
    {
        ui->label_hit_plus0->setEnabled(true);
        draw->print_hit_plus();
        ui->label_prize_plus0->setEnabled(true);
        draw->print_prize_plus();
    }
    else
    {
        ui->label_hit_plus->clear();
        ui->label_hit_plus0->setDisabled(true);
        ui->label_prize_plus->clear();
        ui->label_prize_plus0->setDisabled(true);
    }
}


void mainwindow::set_multiple_text()                                   // we set text how big the stake multiple should be
{
    multiple = ui->horizontalSlider_multiple->value();
    ui->label_multiple->setText(QString::number(multiple));
}


void mainwindow::set_stake_text()
{
    playing_with_plus = ui->radioButton_plus->isChecked();

    if (playing_with_plus)                                             // we calculate the height of stake (payment for the lottery ticket)
        stake = (STAKE_MULTI_LOTEK_PLUS * multiple);                   // if we are playing Multi Lotek Plus
    else
        stake = (STAKE_MULTI_LOTEK * multiple);                        // if we are playing normal Multi Lotek

    ui->label_stake->setText(QString::number(stake) + QString::fromUtf8(" zł"));
}


void mainwindow::set_how_many_chosen_text()                           // we set text how many numbers has to be chosen
{
    int value = ui->horizontalSlider_how_many_chosen->value();
    ui->label_how_many_chosen->setText(QString::number(value));
}


void mainwindow::activate_spinboxes()
{
    int how_many = ui->horizontalSlider_how_many_chosen->value();     // we check how many spinboxes have to be active

    for (int i = 1; i <= how_many; i++)
        spinbox_arr[i]->setEnabled(true);                             // we activate a proper quantity of spinboxes
    for (int i = how_many + 1; i <= 10; i++)
        spinbox_arr[i]->setEnabled(false);                            // we deactivate the rest
}


void mainwindow::show_rules()                       // we show a modeless dialog with the game rules
{
    if (message_box.isVisible())
        return;

    message_box.setParent(this, Qt::Dialog);
    message_box.setModal(false);
    message_box.setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMinimizeButtonHint);
    print_rules();

    message_box.show();

    #ifdef Q_WS_X11
        // only on Linux the dialog isn't automatically centered, so we do it manually
        QRect mainwindow_geometry(x(), y(), width(), height());
        QRect dialog_geometry(message_box.x() , message_box.y(), message_box.width(), message_box.height());

        // we set the center of dialog form to be the same as the center of the main window
        dialog_geometry.moveCenter(mainwindow_geometry.center());
        message_box.move(dialog_geometry.x(), dialog_geometry.y());
    #endif
}


void mainwindow::print_rules()                      // we print the rules in the "Learn the rules" dialog (needed for dynamic translation)
{
    QString text =
            tr("Multilotek is a game where you have to guess what numbers will be drawn.\n\n"
               "1. Each time 20 numbers are drawn from a pool of 80.\n\n"
               "2. You choose from 1 up to 10 numbers. It will affect the height of the prize.\n\n"
               "3. You can decide how much money you want to bet for the drawing. It will also affect the prize.\n\n"
               "4. Finally, you can choose which game mode you want to play - normal Multilotek or Multilotek Plus. "
               "The latter has an additional check if the last drawn number has been also chosen by you.\n"
               "If it has, you will win additional prize.\n\n"
               "Good luck and have fun!\n\n\n"
               "(By the way, while reading these rules, you still have access to the main window.)");

    message_box.setWindowTitle(tr("Learn the rules"));
    message_box.setText(text);
}

/**********************************************************************/

// Handling of the menu elements


void mainwindow::export_data()                      // we export the data into a file
{
    exp_imp = new export_import(this, stat);
    connect (exp_imp, SIGNAL (show_error(QString, QString)), this, SLOT (show_error(QString, QString)));

    exp_imp->export_data();

    disconnect (exp_imp, SIGNAL (show_error(QString, QString)), this, SLOT (show_error(QString, QString)));
    delete exp_imp;
}


void mainwindow::import_data()                      // we import the data from a file
{
    exp_imp = new export_import(this, stat);
    connect (exp_imp, SIGNAL (show_error(QString, QString)), this, SLOT (show_error(QString, QString)));

    exp_imp->import_data();
    change_language();

    disconnect (exp_imp, SIGNAL (show_error(QString, QString)), this, SLOT (show_error(QString, QString)));
    delete exp_imp;
}


void mainwindow::change_language()                  // we change the language of GUI (i.e. after importing the data)
{
    if (new_locale == current_locale)
        return;

    if (new_locale == "pl_PL")
    {
        ui->action_english->setChecked(false);
        ui->action_polish->setChecked(true);
        translator = new QTranslator(this);
        translator->load(QString("multilotek_") + new_locale, QString(":/translations/"));
        qApp->installTranslator(translator);
    }
    else if (new_locale == "en_US")
    {
        ui->action_polish->setChecked(false);
        ui->action_english->setChecked(true);
        qApp->removeTranslator(translator);
        super_delete(translator);
        current_locale = new_locale;
    }
}


void mainwindow::change_language(QAction* action)  // we change the language of GUI (i.e. after choosing it in the menu)
{
    new_locale = action->data().toString();

    if (new_locale == "pl_PL")
    {
        if (current_locale == new_locale)
        {
            ui->action_polish->setChecked(true);
            return;
        }

        ui->action_english->setChecked(false);
        translator = new QTranslator(this);
        translator->load(QString("multilotek_") + new_locale, QString(":/translations/"));
        qApp->installTranslator(translator);
        current_locale = new_locale;
    }
    else if (new_locale == "en_US")
    {
        if (current_locale == new_locale)
        {
            ui->action_english->setChecked(true);
            return;
        }

        ui->action_polish->setChecked(false);
        qApp->removeTranslator(translator);
        super_delete(translator);
        current_locale = new_locale;
    }
}


void mainwindow::changeEvent(QEvent* event)         // redefined changeEvent() function for retranslating GUI
{
    if (event->type() == QEvent::LanguageChange)
    {
        // all designed forms have retranslateUi() method
        ui->retranslateUi(this);

        // we print translated things that dynamically change their state
        if (draw->already_performed_drawing)
        {
            draw->print_hit_normal();
            draw->print_hit_plus();
            stat->print_most_freq_drawn();
            stat->print_most_freq_hit();
        }

        if (message_box.isVisible())
            print_rules();
    }
    QMainWindow::changeEvent(event);
}


void mainwindow::show_author()                      // we show information about the author of the program
{
    about_form = new about(this);
    about_form->exec();
    delete about_form;
}

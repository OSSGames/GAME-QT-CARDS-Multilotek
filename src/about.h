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


#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui/QDialog>

namespace Ui {
    class about;
}

class about : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(about)

private:
    Ui::about *ui;                          // pointer to the "About" form

public:
    explicit about(QWidget *parent = 0);    // constructor
    virtual ~about();                       // destructor

private slots:
    void on_pushButton_clicked()            // closing the application by pushing the "OK" button
    {
        close();
    }
};

#endif // ABOUT_H

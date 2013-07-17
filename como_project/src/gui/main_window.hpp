/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QOpenGLContext>
#include "../managers/scene.hpp"
#include "../managers/como_app.hpp"
#include <memory>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    private:
        Ui::MainWindow *ui;

        // App's singlenton instance.
        shared_ptr< ComoApp > comoApp;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();
};

#endif // MAIN_WINDOW_HPP

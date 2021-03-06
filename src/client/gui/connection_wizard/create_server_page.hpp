/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef CREATE_SERVER_PAGE_HPP
#define CREATE_SERVER_PAGE_HPP

#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QFileInfo>
#include <QPushButton>

#include <client/managers/scene/scene.hpp>
#include <memory>

#ifdef _WIN32
const char SERVER_PATH[] = "..\\server\\server";
#else
const char SERVER_PATH[] = "../server/server";
#endif

namespace como {

class CreateServerPage : public QWizardPage
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        CreateServerPage() = delete;
        CreateServerPage( const CreateServerPage& ) = delete;
        CreateServerPage( CreateServerPage&& ) = delete;
        CreateServerPage( ScenePtr& scene, LogPtr log );


        /***
         * 2. Destruction
         ***/
        ~CreateServerPage() = default;


        /***
         * 3. Getters
         ***/
        virtual int nextId() const;


        /***
         * 4. Operators
         ***/
        CreateServerPage& operator = ( const CreateServerPage& ) = delete;
        CreateServerPage& operator = ( CreateServerPage&& ) = delete;


    protected:
        /***
         * 5. Validators
         ***/
        virtual bool validatePage();


    private:
        ScenePtr& scene_;
        QLineEdit* sceneNameInput_;
        QLineEdit* portInput_;
        QSpinBox* maxUsersInput_;
        QPushButton* sceneFileInput_;
        QFileInfo sceneFileInfo_;

        // Log
        LogPtr log_;
};

} // namespace como

#endif // CREATE_SERVER_PAGE_HPP

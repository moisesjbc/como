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

#ifndef PRIMITIVE_IMPORT_DIALOG_HPP
#define PRIMITIVE_IMPORT_DIALOG_HPP

#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QMessageBox>
#include <client/managers/managers/primitives/client_primitives_manager.hpp>
#include <client/gui/resources/resource_selector.hpp>
#include <QFileInfo>

namespace como {

class PrimitiveImportDialog : public QDialog
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveImportDialog() = delete;
        PrimitiveImportDialog( ClientPrimitivesManager* primitivesManager );
        PrimitiveImportDialog( const PrimitiveImportDialog& ) = delete;
        PrimitiveImportDialog( PrimitiveImportDialog&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveImportDialog() = default;


        /***
         * 3. Operators
         ***/
        PrimitiveImportDialog& operator = ( const PrimitiveImportDialog& ) = delete;
        PrimitiveImportDialog& operator = ( PrimitiveImportDialog&& ) = delete;


    public slots:
        /***
         * 4. Data validation
         ***/
        void validate();


    private:
        ClientPrimitivesManager* primitivesManager_;

        QPushButton* fileInput_;
        QFileInfo fileInfo_;
        ResourceSelector* primitiveCategorySelector_;

        QPushButton* okButton_;
        QPushButton* cancelButton_;
};

} // namespace como

#endif // PRIMITIVE_IMPORT_DIALOG_HPP

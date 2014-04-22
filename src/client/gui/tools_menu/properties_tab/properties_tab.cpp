/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "properties_tab.hpp"
#include <QVBoxLayout>

namespace como {


/***
 * 1. Construction
 ***/

PropertiesTab::PropertiesTab( DrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this widget.
    QVBoxLayout* layout = new QVBoxLayout;

    // Construct the General Info menu.
    generalInfoMenu_ = new GeneralInfoMenu( userSelection_ );

    // Construct the Mesh Info menu.
    meshInfoMenu_ = new MeshInfoMenu( userSelection_ );

    // Add required widgets to the layout and set the latter as the current
    // layout.
    layout->addWidget( generalInfoMenu_ );
    layout->addWidget( meshInfoMenu_ );
    setLayout( layout );

    // Every time the user's selection changes, refres this properties tab.
    connect( userSelection_.get(), &DrawablesSelection::selectionChanged, this, &PropertiesTab::refresh );

    // Perform a first refreshing.
    refresh();
}


/***
 * 2. Initialization
 ***/


/***
 * 3. Refreshing
 ***/

void PropertiesTab::refresh()
{
    generalInfoMenu_->refresh();

    // If the user selection contains meshes, make the mesh info menu visible
    // to user and refresh it.
    if( userSelection_->contains( DrawableType::MESH ) ){
        meshInfoMenu_->setVisible( true );
        meshInfoMenu_->refresh();
    }else{
        meshInfoMenu_->setVisible( false );
    }
}

} // namespace como
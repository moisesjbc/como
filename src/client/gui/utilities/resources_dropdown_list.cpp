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

#include "resources_dropdown_list.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesDropdownList::ResourcesDropdownList( SelectableResourcesContainer& resourcesContainer ) :
    resourcesContainer_( &resourcesContainer )
{
    void (QComboBox::*srcSignal)( int ) = &QComboBox::currentIndexChanged;
    QObject::connect( this, srcSignal, [this]( int index ){
        if( index != -1 ){
            emit resourceSelected( indexToID_.at( index ) );
        }
    });

    // Is it OK to return a "this" pointer from constructor?
    resourcesContainer.addObserver( this );
}


/***
 * 3. Updating (observer pattern)
 ***/

void ResourcesDropdownList::update(ContainerAction lastContainerAction, ResourceID lastElementModified)
{
    switch( lastContainerAction ){
        case ContainerAction::ELEMENT_INSERTION:
            if( resourcesContainer_->isResourceSelectable( lastElementModified ) ){
                // TODO: Add elemenet.
            }
        break;
        case ContainerAction::ELEMENT_DELETION:
            // TODO: Remove element.
        break;
        default:
            if( resourcesContainer_->isResourceSelectable( lastElementModified ) ){
                // TODO: Add elemenet.
            }else{
                // TODO: Remove element.
            }
        break;
    }
}

} // namespace como

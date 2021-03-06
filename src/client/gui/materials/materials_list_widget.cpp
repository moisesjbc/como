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

#include "materials_list_widget.hpp"

namespace como {


/***
 * 1. Construction
 ***/

MaterialsListWidget::MaterialsListWidget( MaterialsManagerPtr materialsManager ) :
    materialsManager_( materialsManager )
{
    void (QComboBox::*srcSignal)( int ) = &QComboBox::currentIndexChanged;
    QObject::connect( this, srcSignal, [this]( int index ){
        if( index != -1 ){
            emit materialSelected( materialsManager_->selectMaterial( indexToID_.at( index ) ) );
        }
    });
}


/***
 * 4. Populating
 ***/

void MaterialsListWidget::populate()
{
    clear();
    indexToID_.clear();

    ResourceHeadersList materialsHeaders = materialsManager_->getLocalMaterialsHeaders();

    for( const ResourceHeader& materialHeader : materialsHeaders ){
        indexToID_[count()] = materialHeader.id;
        addItem( materialHeader.name.c_str() );
    }

    if( count() ){
        setCurrentIndex( 0 );
    }
}


} // namespace como

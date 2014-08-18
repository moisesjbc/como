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

#include "lights_selection.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightsSelection::LightsSelection( glm::vec4 borderColor ) :
    EntitiesSet( borderColor )
{}


/***
 * 3. Getters
 ***/

PackableColor LightsSelection::getLightColor() const
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::getLightColor() called on empty LightsSelection" );
    }
    return resources_.begin()->second->getLightColor();
}


float LightsSelection::getAmbientCoefficient() const
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::getAmbientCoefficient() called on empty LightsSelection" );
    }
    return resources_.begin()->second->getAmbientCoefficient();
}


/***
 * 4. Setters
 ***/

void LightsSelection::setLightColor( const PackableColor& color)
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::setLightColor() called on empty LightsSelection" );
    }
    return resources_.begin()->second->setLightColor( color );
}


void LightsSelection::setAmbientCoefficient( float coefficient )
{
    if( resources_.size() == 0 ){
        throw std::runtime_error( "LightsSelection::setAmbientCoefficient() called on empty LightsSelection" );
    }
    return resources_.begin()->second->setAmbientCoefficient( coefficient );
}

} // namespace como

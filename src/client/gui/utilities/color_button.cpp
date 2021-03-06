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

#include "color_button.hpp"

namespace como {

ColorButton::ColorButton( const QColor& color )
{
    setColor( color );

    // Signal / Slot connection. When pressed, this button opens a color
    // selection dialog.
    QObject::connect( this, &QPushButton::clicked, this, &ColorButton::execColorDialog );
}


/***
 * 3. Setters
 ***/

void ColorButton::setColor( const QColor& color )
{
    color_ = color;

    // Set the button's background to the color given as an argument.
    QString qss = QString( "background-color: %1" ).arg( color_.name() );
    setStyleSheet( qss );

    // Emit a signal indicating that the current color has changed.
    Color packableColor(
                static_cast< std::uint8_t >( color.red() ),
                static_cast< std::uint8_t >( color.green() ),
                static_cast< std::uint8_t >( color.blue() ),
                static_cast< std::uint8_t >( color.alpha() )
                );

    emit colorChanged( packableColor );
}


void ColorButton::setColor(const Color &color)
{
    color_.setRgb(
                color[0],
                color[1],
                color[2],
                color[3] );

    // Set the button's background to the color given as an argument.
    QString qss = QString( "background-color: %1" ).arg( color_.name() );
    setStyleSheet( qss );

    // Emit a signal indicating that the current color has changed.
    // TODO: Possible infinite loop?.
    //emit colorChanged( color );
}


/***
 * 6. Slots
 ***/

void ColorButton::execColorDialog()
{
    // Open a dialog for selecting a new color.
    QColor color = QColorDialog::getColor( color_ );

    // Set this button's color to the color selected by user.
    if( color.isValid() ){
        setColor( color );
    }
}

} // namespace como

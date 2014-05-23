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

#ifndef SELECTION_COMMAND_HPP
#define SELECTION_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class SelectionCommandType : std::uint8_t {
    FULL_DESELECTION = 0,
    SELECTION_RESPONSE,
    SELECTION_TRANSFORMATION,
    SELECTION_DELETION,
    PARAMETER_CHANGE
};
typedef PackableUint8< SelectionCommandType > PackableSelectionCommandType;


class SelectionCommand : public TypeCommand<SelectionCommandType>
{
    public:
        /***
         * 1. Construction
         ***/
        SelectionCommand( SelectionCommandType selectionCommmandType, UserID userID );
        SelectionCommand( const SelectionCommand& b );
        SelectionCommand( SelectionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectionCommand() = default;


        /***
         * 3. Operators
         ***/
        SelectionCommand& operator=( const SelectionCommand& ) = delete;
        SelectionCommand& operator=( SelectionCommand&& ) = delete;
};

typedef std::shared_ptr< const SelectionCommand > SelectionCommandConstPtr;

} // namespace como

#endif // SELECTION_COMMAND_HPP

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

#ifndef PRIMITIVE_COMMAND_HPP
#define PRIMITIVE_COMMAND_HPP

#include "../command.hpp"

namespace como {

/*!
 * \enum PrimitiveCommandType
 *
 * Enumeration with all the possible types of commands focused on 3D
 * primitives.
 */
enum class PrimitiveCommandType : std::uint8_t {
    PRIMITIVE_CREATION = 0
};

typedef PackableUint8< PrimitiveCommandType > PackablePrimitiveCommandType;
typedef std::uint8_t PrimitiveID;
typedef PackableUint8< PrimitiveID > PackablePrimitiveID;

class PrimitiveCommand : public Command
{
    private:
        /*! \brief Command type. */
        const PackablePrimitiveCommandType commandType_;

        /*! \brief Primitive ID. */
        PackablePrimitiveID primitiveID_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        PrimitiveCommand() = delete;

        /*!
         * \brief Constructor. Constructs a PrimitiveCommand from the given
         * data.
         * \param PrimitiveCommandType command type.
         * \param userID ID of the user who performed this command.
         * \param primitiveID ID of the primitive affected by this command.
         */
        PrimitiveCommand( PrimitiveCommandType primitiveCommandType, UserID userID, PrimitiveID primitiveID );

        /*! \brief Copy constructor. */
        PrimitiveCommand( const PrimitiveCommand& b );

        /*! \brief Move constructor. */
        PrimitiveCommand( PrimitiveCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor. */
        ~PrimitiveCommand() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Returns this Command's type. */
        PrimitiveCommandType getType() const;

        /*! \brief Get the ID of the primitive affected by this target. */
        PrimitiveID getPrimitiveID() const ;


        /***
         * 5. Buffer pre reading
         ***/

        /*!
         * \brief Unpacks / reads a PrimitiveCommandType from the given
         * buffer.
         */
        static PrimitiveCommandType getType( const void* buffer );


        /***
         * 6. Operators
         ***/

        /*! \brief Copy assignment operator. */
        PrimitiveCommand& operator=( const PrimitiveCommand& ) = delete;

        /*! \brief Move assignment operator. */
        PrimitiveCommand& operator=( PrimitiveCommand&& ) = delete;
};

typedef std::shared_ptr< const PrimitiveCommand > PrimitiveCommandConstPtr;

} // namespace como

#endif // PRIMITIVE_COMMAND_HPP
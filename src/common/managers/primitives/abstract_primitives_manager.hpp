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

#ifndef ABSTRACT_PRIMITIVES_MANAGER_HPP
#define ABSTRACT_PRIMITIVES_MANAGER_HPP

#include <string>
#include <map>
#include <common/utilities/log.hpp>
#include <common/ids/resource_id.hpp>
#include <common/ids/resource_id.hpp>
#include <common/managers/primitives/abstract_primitives_manager.hpp>
#include <list>

namespace como {

struct PrimitiveInfo
{
    std::string name;
    ResourceID category;

    std::string meshFileName;
    std::string materialFileName;
};

enum class PrimitiveComponent {
    MESH = 0,
    MATERIAL
};

// TODO: Move this
typedef std::list< std::pair< ResourceID, std::string > > ResourcesList;

class AbstractPrimitivesManager
{
    protected:
        std::string scenePrimitivesDir_;

        std::map< ResourceID, std::string > categoryNames_;
        std::map< ResourceID, PrimitiveInfo > primitiveInfo_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        AbstractPrimitivesManager() = delete;
        AbstractPrimitivesManager( std::string sceneDirPath, LogPtr log );
        AbstractPrimitivesManager( const AbstractPrimitivesManager& ) = delete;
        AbstractPrimitivesManager( AbstractPrimitivesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~AbstractPrimitivesManager();


        /***
         * 3. Getters
         ***/
        std::string getPrimitiveRelativePath( ResourceID id, PrimitiveComponent component ) const;
        std::string getPrimitiveAbsolutePath( ResourceID id, PrimitiveComponent component ) const;

    protected:
        std::string getCategoryRelativePath( ResourceID id ) const;
        std::string getCategoryAbsoluteePath( ResourceID id ) const;
    public:
        ResourcesList getCategoriesList() const;


        /***
         * 4. Categories management
         ***/
    protected:
        void registerCategory( ResourceID id, std::string name );
        void createCategory( ResourceID id, std::string name );


        /***
         * 5. Primitives management
         ***/
        void registerPrimitive( ResourceID id, ResourceID category, std::string meshFileName, std::string materialFileName );


        /***
         * 6. Auxiliar methods
         ***/
        bool categoryNameInUse( std::string categoryName ) const;


        /***
         * 7. Operators
         ***/
        AbstractPrimitivesManager& operator = ( const AbstractPrimitivesManager& ) = delete;
        AbstractPrimitivesManager& operator = ( AbstractPrimitivesManager&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_PRIMITIVES_MANAGER_HPP

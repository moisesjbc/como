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

#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <unordered_set>
#include "observer.hpp"

namespace como {

class Observable
{
    private:
        std::unordered_set< ObserverPtr > observers_;

    public:
        /***
         * 1. Construction
         ***/
        Observable();
        Observable( const Observable& ) = default;
        Observable( Observable& ) = default;


        /***
         * 2. Destruction
         ***/
        ~Observable() = default;


        /***
         * 3. Observers management
         ***/
        bool addObserver( ObserverPtr observer );
        void removeObserver( ObserverPtr observerToBeRemoved );


        /***
         * 4. Observers notification
         ***/
        void notifyObservers();


        /***
         * 5. Operators
         ***/
        Observable& operator = ( const Observable& ) = default;
        Observable& operator = ( Observable&& ) = default;
};

} // namespace como

#endif // OBSERVABLE_HPP

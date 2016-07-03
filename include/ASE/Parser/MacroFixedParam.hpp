//////////////////////////////////////////////////////////////////////////////////
//
//
//                    d88b         ad8888888ba   888888888888
//                   d8888b       d88"     "88b  888
//                  d88'`88b      Y88,           888
//                 d88'  `88b     `Y88888,       88888888
//                d88Y8888Y88b      `"""""88b,   888
//               d88""""""""88b           `88b   888
//              d88'        `88b  Y88a     a88P  888
//             d88'          `88b  "Y8888888P"   888888888888
//
//
// AwesomeScriptEditor: A script editor for GBA Pokémon games.
// Copyright (C) 2016 Pokedude, Diegoisawesome
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


#ifndef __ASE_MACROFIXEDPARAM_HPP__
#define __ASE_MACROFIXEDPARAM_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <ASE/Parser/MacroParam.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    MacroFixedParam.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Defines an unmodifyable, fixed parameter.
    ///
    ///////////////////////////////////////////////////////////
    class MacroFixedParam : public MacroParam {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::MacroFixedParam.
        ///
        ///////////////////////////////////////////////////////////
        MacroFixedParam();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the fixed value of this parameter.
        /// \returns the fixed value of this parameter.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 value() const;

        ///////////////////////////////////////////////////////////
        /// \brief Sets the fixed value of this parameter.
        /// \param value The fixed value of this parameter.
        ///
        ///////////////////////////////////////////////////////////
        void setValue(UInt32 value);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Value;
    };
}


#endif  // __ASE_MACROFIXEDPARAM_HPP__

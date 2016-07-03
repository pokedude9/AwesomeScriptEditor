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


#ifndef __ASE_SCRIPTBLOCK_HPP__
#define __ASE_SCRIPTBLOCK_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Defines
    //
    ///////////////////////////////////////////////////////////
    #define TERMINATOR_ITEM 0x0
    #define TERMINATOR_MOVE 0xFE


    ///////////////////////////////////////////////////////////
    /// \brief Defines the read priorities for script blocks.
    ///
    ///////////////////////////////////////////////////////////
    enum BlockPriority
    {
        BP_High,
        BP_Medium,
        BP_Low
    };


    ///////////////////////////////////////////////////////////
    /// \file    ScriptBlock.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Provides an abstract base for script data types.
    ///
    /// The type can be either a function (or sub-function), a
    /// Pokémon-encoded string, a list of items that belong to
    /// a pokemart command or a list of movements.
    ///
    ///////////////////////////////////////////////////////////
    class ScriptBlock {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Implicit constructor
        ///
        /// Implicitely constructs the object by setting the offset
        /// of the desired script data type.
        ///
        /// \param offset Offset of the data
        ///
        ///////////////////////////////////////////////////////////
        ScriptBlock(UInt32 offset);

        ///////////////////////////////////////////////////////////
        /// \brief Virtual destructor
        ///
        /// Must override in order to not leak memory.
        ///
        ///////////////////////////////////////////////////////////
        virtual ~ScriptBlock();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the current block offset.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset() const;


        ///////////////////////////////////////////////////////////
        /// \brief Reads the data located at this offset.
        ///
        /// The data beeing read depends on the devired type which
        /// implemented this virtual reading function.
        ///
        /// \param rom Currently loaded ROM file
        ///
        ///////////////////////////////////////////////////////////
        virtual const QString readData(const qboy::Rom &rom);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the data located at this offset.
        ///
        /// The data beeing written depends on the devired type
        /// which implemented this virtual writing function.
        ///
        /// \param rom Currently loaded ROM file
        ///
        ///////////////////////////////////////////////////////////
        virtual void writeData(const qboy::Rom &rom);


    protected:

        ///////////////////////////////////////////////////////////
        // Protected base class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;
    };
}


#endif  // __ASE_SCRIPTBLOCK_HPP__

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


#ifndef __ASE_SCRIPT_HPP__
#define __ASE_SCRIPT_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/Scripting/ScriptBlock.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ASEScript.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Holds symbols for a decompiled script.
    ///
    /// Supports reading all the data from a pre-parsed script.
    ///
    ///////////////////////////////////////////////////////////
    class ASEScript {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEScript.
        ///
        ///////////////////////////////////////////////////////////
        ASEScript();

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases the ownership-taken script-blocks.
        ///
        ///////////////////////////////////////////////////////////
        ~ASEScript();


        ///////////////////////////////////////////////////////////
        /// \brief Parses and reads the underlying data.
        ///
        /// In case this is a function, it does not read any sub
        /// functions because they will be read as extra script
        /// block.
        ///
        /// \param rom The currently opened ROM file
        ///
        ///////////////////////////////////////////////////////////
        const QString parse(const qboy::Rom &rom) const;

        ///////////////////////////////////////////////////////////
        /// \brief Adds the specified block to the list.
        ///
        /// The data located at the specified block will be read
        /// and parsed and appended to the script string depending
        /// on its priority. Takes ownership of the block.
        ///
        /// \param block Block to append to the list
        /// \param priority One of the BP enum flags
        ///
        ///////////////////////////////////////////////////////////
        void addBlock(ScriptBlock *block, BlockPriority priority);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QList<ScriptBlock *> m_Blocks;
        QList<BlockPriority> m_Priorities;
    };
}


#endif  // __ASE_SCRIPT_HPP__

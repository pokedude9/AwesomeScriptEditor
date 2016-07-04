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


#ifndef __ASE_PARAMETERTYPE_HPP__
#define __ASE_PARAMETERTYPE_HPP__


///////////////////////////////////////////////////////////
// Defines
//
///////////////////////////////////////////////////////////
#define PT_BYTE_SIZE        1
#define PT_HWORD_SIZE       2
#define PT_WORD_SIZE        4


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ParameterType.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Defines possible parameter types.
    ///
    /// Special types like item, text and movement pointers
    /// are treated in a specific way upon (de)compiling.
    ///
    ///////////////////////////////////////////////////////////
    enum ParameterType
    {
        PT_Byte,
        PT_HWord,
        PT_Word,
        PT_Pointer,
        PT_MovePointer,
        PT_ItemPointer,
        PT_StringPointer
    };

    ///////////////////////////////////////////////////////////
    /// \brief Array of type strings; for real-time validator.
    ///
    ///////////////////////////////////////////////////////////
    extern const char *ParamTypeStrings[7];


    namespace ParameterTypeHelper
    {
        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the size of the parameter.
        ///
        /// Retrieves the size of the parameter in order to know
        /// the location of the next parameter.
        ///
        /// \returns the size of the parameter, in bytes.
        ///
        ///////////////////////////////////////////////////////////
        inline unsigned int GetParamSize(ParameterType type)
        {
            if (type == PT_Byte)
                return PT_BYTE_SIZE;
            if (type == PT_HWord)
                return PT_HWORD_SIZE;

            return PT_WORD_SIZE;
        }

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the maximum value of the parameter.
        ///
        /// Retrieves the maximum value of the parameter in order
        /// to know how much bytes it occupies
        ///
        /// \returns the maximum value of the parameter.
        ///
        ///////////////////////////////////////////////////////////
        inline int GetMaxValue(ParameterType type)
        {
            if (type == PT_Byte)
                return 0xFF;
            if (type == PT_HWord)
                return 0xFFFF;

            return 0xFFFFFFFF;
        }
    }
}


#endif  // __ASE_PARAMETERTYPE_HPP__

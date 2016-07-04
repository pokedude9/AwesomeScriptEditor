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


#ifndef __ASE_RTERROR_HPP__
#define __ASE_RTERROR_HPP__


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Error messages
    //
    ///////////////////////////////////////////////////////////
    #define RT_INVALID_DEFINITION       "The definition '%s' is invalid."
    #define RT_INVALID_INCLUDE          "The include expression '%s' is invalid."
    #define RT_INVALID_MOVEMENT         "Movement '%s' does not exist."
    #define RT_INVALID_ITEM             "Item '%s' does not exist."
    #define RT_INVALID_BYTE             "Invalid byte identifier '%s'."
    #define RT_INVALID_HWORD            "Invalid halfword identifier '%s'."
    #define RT_INVALID_WORD             "Invalid word identifier '%s'."
    #define RT_OVERFLOW_BYTE            "Given value '%i' exceeds byte range."
    #define RT_OVERFLOW_HWORD           "Given value '%i' exceeds halfword range."
    #define RT_OVERFLOW_WORD            "Given value '%i' exceeds word range."
    #define RT_UNKNOWN_PREPROC          "The preprocessor directive '%s' does not exist."
    #define RT_INVALID_LABEL            "Erroneous label syntax '%s'."
    #define RT_INVALID_ASE_LABEL        "Erroneous ASE label syntax '%s'. Expected ':'."
    #define RT_UNKNOWN_IDENTIFIER       "Identifier '%s' is not a command, macro or preprocessor directive."
    #define RT_INVALID_ARGCOUNT         "Invalid argument count for command '%i'."
    #define RT_INVALID_PARAMETER_N      "Parameter '%i': Expected numeric value. Got '%s'."
    #define RT_INVALID_PARAMETER_O      "Parameter '%i': Numeric value overflow. Expected: <'%i'. Got: '%i'."
    #define RT_INVALID_PARAMETER_I      "Given parameter '%s' is not a macro or label. Ignore if label existing later in the code."
}


#endif  // __ASE_RTWORKER_HPP__

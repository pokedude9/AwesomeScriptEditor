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


#ifndef __ASE_PARAMETER_HPP__
#define __ASE_PARAMETER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/Parser/ParameterType.hpp>
#include <QString>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ParameterType.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Defines a parameter with type and description.
    ///
    /// Each parameter has a specific value type that
    /// determines how it is read and whether it loads
    /// dependencies, in case it is a data pointer. It
    /// also holds information for the intellisense system
    /// which tracks the parameters the user enters.
    ///
    ///////////////////////////////////////////////////////////
    class Parameter {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::Parameter.
        ///
        ///////////////////////////////////////////////////////////
        Parameter();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the parameter type for this parameter.
        ///
        /// The parameter type determines which size the argument
        /// has and therefore affects the (de)compiling process.
        ///
        /// \returns a readonly copy of the parameter type.
        ///
        ///////////////////////////////////////////////////////////
        ParameterType type() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the string identifier of this parameter.
        ///
        /// This is mostly for intellisense purposes. The parameter
        /// name should give a useful hint of what it does in-game.
        ///
        /// \returns a readonly reference of the parameter name.
        ///
        ///////////////////////////////////////////////////////////
        const QString &name() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the description of this parameter.
        ///
        /// This is only used for intellisense. It is shown
        /// underneath the parameter name, which is bold.
        ///
        /// \returns a readonly reference of the description.
        ///
        ///////////////////////////////////////////////////////////
        const QString &info() const;


        ///////////////////////////////////////////////////////////
        /// \brief Modifies the parameter type.
        ///
        /// This function should never be called by unresponsible
        /// code, else various bugs concerning the compiling occur.
        ///
        /// \param type ase::ParameterType enum value
        ///
        ///////////////////////////////////////////////////////////
        void setType(ParameterType type);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the parameter name.
        ///
        /// This function is exclusively called by the configuration
        /// reader, which parses the XML command file.
        ///
        /// \param name New name of the parameter
        ///
        ///////////////////////////////////////////////////////////
        void setName(const QString &name);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the parameter description.
        ///
        /// This function is exclusively called by the configuration
        /// reader, which parses the XML command file.
        ///
        /// \param info New description of the parameter
        ///
        ///////////////////////////////////////////////////////////
        void setInfo(const QString &info);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        ParameterType m_Type;
        QString m_Name;
        QString m_Info;
    };
}


#endif  // __ASE_PARAMETER_HPP__

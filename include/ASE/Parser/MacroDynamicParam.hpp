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


#ifndef __ASE_MACRODYNAMICPARAM_HPP__
#define __ASE_MACRODYNAMICPARAM_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <ASE/Parser/MacroParam.hpp>
#include <QString>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    MacroParam.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Defines a dynamic, editable parameter.
    ///
    ///////////////////////////////////////////////////////////
    class MacroDynamicParam : public MacroParam {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::MacroDynamicParam.
        ///
        ///////////////////////////////////////////////////////////
        MacroDynamicParam();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the name of the parameter.
        /// \returns the name of the parameter
        ///
        ///////////////////////////////////////////////////////////
        const QString &name() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the description of the parameter.
        /// \returns the info of the parameter
        ///
        ///////////////////////////////////////////////////////////
        const QString &info() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the index of the parameter.
        ///
        /// The index of the parameter simply is the index of
        /// it in the list of dynamic parameters.
        ///
        /// \returns the dynamic index of the parameter
        ///
        ///////////////////////////////////////////////////////////
        Int32 index() const;


        ///////////////////////////////////////////////////////////
        /// \brief Sets the name of the parameter.
        /// \param name The name of the parameter
        ///
        ///////////////////////////////////////////////////////////
        void setName(const QString &name);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the description of the parameter.
        /// \param info The description of the parameter
        ///
        ///////////////////////////////////////////////////////////
        void setInfo(const QString &info);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the index of the parameter.
        /// \param index The index of the parameter
        ///
        ///////////////////////////////////////////////////////////
        void setIndex(Int32 index);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QString     m_Name;
        QString     m_Info;
        Int32       m_Index;
    };
}


#endif  // __ASE_MACRODYNAMICPARAM_HPP__

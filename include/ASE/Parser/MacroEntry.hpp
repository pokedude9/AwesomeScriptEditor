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


#ifndef __ASE_MACROENTRY_HPP__
#define __ASE_MACROENTRY_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <ASE/Parser/MacroFixedParam.hpp>
#include <ASE/Parser/MacroDynamicParam.hpp>
#include <QList>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    MacroEntry.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Contains the command and all the parameters.
    ///
    /// A parameter can be either fixed or dynamic. A fixed
    /// parameter simply contains a value of specific type
    /// whereas a dynamic parameter can be changed in the
    /// script.
    ///
    ///////////////////////////////////////////////////////////
    class MacroEntry {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of pory::MacroEntry.
        ///
        ///////////////////////////////////////////////////////////
        MacroEntry();

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases the macro parameter structures because they
        /// are not released by the configuration loader.
        ///
        ///////////////////////////////////////////////////////////
        ~MacroEntry();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the underlying command's name.
        ///
        /// The name is used to identify the actual pointer to a
        /// valid command structure.
        ///
        /// \returns the name of the underlying command.
        ///
        ///////////////////////////////////////////////////////////
        const QString &command() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all the fixed parameters in a row.
        ///
        /// Some operations require the parameter types to be in
        /// a row, others require them to be in the order in which
        /// they exist in the ROM.
        ///
        /// \returns the fixed parameters only.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MacroFixedParam *> &fixedParams() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all the dynamic parameters in a row.
        ///
        /// Some operations require the parameter types to be in
        /// a row, others require them to be in the order in which
        /// they exist in the ROM.
        ///
        /// \returns the dynamic parameters only.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MacroDynamicParam *> &dynamicParams() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all parameters in "random" order.
        ///
        /// Some operations require the parameter types to be in
        /// a row, others require them to be in the order in which
        /// they exist in the ROM.
        ///
        /// \returns all the parameters.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MacroParam *> &params() const;


        ///////////////////////////////////////////////////////////
        /// \brief Sets the underlying command's name.
        /// \param command Name of the underlying command
        ///
        ///////////////////////////////////////////////////////////
        void setCommand(const QString &command);

        ///////////////////////////////////////////////////////////
        /// \brief Adds one parameter in "random" order.
        ///
        /// Automatically sorts out the fixed and dynamic params
        /// and stores them in separate lists.
        ///
        /// \param param Parameter structure
        /// \param type 0=fixed 1=dynamic
        ///
        ///////////////////////////////////////////////////////////
        void addParam(MacroParam *param);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QString                         m_Command;
        QList<MacroParam *>             m_Params;
        QList<MacroFixedParam *>        m_ParamsFixed;
        QList<MacroDynamicParam *>      m_ParamsDynamic;
    };
}


#endif  // __ASE_MACROENTRY_HPP__

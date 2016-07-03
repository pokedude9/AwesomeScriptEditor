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


#ifndef __ASE_MACRO_HPP__
#define __ASE_MACRO_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <ASE/Parser/MacroEntry.hpp>
#include <ASE/Parser/Command.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    Macro.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   A macro that contains multiple commands.
    ///
    /// Macroes can contain multiple parameters (dynamic) but
    /// also predefined ones (fixed).
    ///
    ///////////////////////////////////////////////////////////
    class Macro {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::Macro.
        ///
        ///////////////////////////////////////////////////////////
        Macro();

        ///////////////////////////////////////////////////////////
        /// \brief Default destructor
        ///
        /// Deletes every macro entry stored.
        ///
        ///////////////////////////////////////////////////////////
        ~Macro();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the entries for this macro.
        ///
        /// The entries contain the command names and both, fixed
        /// parameters and dynamic parameters.
        ///
        /// \returns a list of entries.
        ///
        ///////////////////////////////////////////////////////////
        const QList<MacroEntry *> &entries() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the commands for this macro.
        ///
        /// Contains all commands and parameters, including the
        /// predefined ones.
        ///
        /// \returns a readonly list of commands.
        ///
        ///////////////////////////////////////////////////////////
        const QList<const Command*> &commands() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the name of the macro.
        ///
        /// The macro name is essential to the (de)compiling
        /// process as it will be displayed instead of two or
        /// more commands.
        ///
        /// \returns the readonly macro name.
        ///
        ///////////////////////////////////////////////////////////
        const QString &name() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the description of the macro.
        ///
        /// The macro description is exclusively used for the
        /// intellisense system.
        ///
        /// \returns the readonly macro description.
        ///
        ///////////////////////////////////////////////////////////
        const QString &info() const;


        ///////////////////////////////////////////////////////////
        /// \brief Sets the entries for this macro.
        /// \param entries List of entries
        ///
        ///////////////////////////////////////////////////////////
        void setEntries(const QList<MacroEntry *> &entries);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the macro name.
        /// \param name Macro name
        ///
        ///////////////////////////////////////////////////////////
        void setName(const QString &name);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the macro description.
        /// \param info Macro description
        ///
        ///////////////////////////////////////////////////////////
        void setInfo(const QString &info);


        ///////////////////////////////////////////////////////////
        /// \brief Parses this macro's meta-data to commands.
        ///
        /// Retrieves all the entry's command name and tried to
        /// find it in the given commands-list. The list entry
        /// "nullptr" will be added in case it can not be found.
        ///
        ///////////////////////////////////////////////////////////
        void parse();

        //////////////////////////////////////////////////////////
        /// \brief Parses the macro into a string and returns it.
        ///
        /// Reads the data from the offset where the current ROM
        /// is seeked at and converts it to a readable string.
        ///
        //////////////////////////////////////////////////////////
        const QString read(const qboy::Rom &rom) const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QList<const Command *>  m_Commands;
        QList<MacroEntry *>     m_Entries;
        QString                 m_Name;
        QString                 m_Info;
    };
}


#endif  // __ASE_MACRO_HPP__

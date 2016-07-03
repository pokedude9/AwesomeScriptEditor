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


#ifndef __ASE_CONFIGURATION_HPP__
#define __ASE_CONFIGURATION_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/Parser/Command.hpp>
#include <ASE/Parser/Macro.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    MacroParam.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Loads the files containing the commands.
    ///
    /// Allows multiple files to be loaded and parsed. Lists
    /// of macroes and commands can be retrieved by calling
    /// the respective getter functions.
    ///
    ///////////////////////////////////////////////////////////
    class Configuration {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Loads all the commands and macroes.
        ///
        /// Opens every XML file that is located within the settings
        /// folder and starts with "Script.". Then attempts to load
        /// the commands and macroes out of each opened file.
        ///
        ///////////////////////////////////////////////////////////
        static void read();

        ///////////////////////////////////////////////////////////
        /// \brief Destroys all the commands and macroes.
        ///
        ///////////////////////////////////////////////////////////
        static void dispose();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the amount of found files.
        ///
        ///////////////////////////////////////////////////////////
        static Int32 getAmount();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the commands at the given index.
        ///
        /// The index corresponds to the order in which the files
        /// were parsed. The default script file is not guaranteed
        /// to be first (zero) one.
        ///
        /// \param index Index of the file to fetch commands from.
        ///
        ///////////////////////////////////////////////////////////
        static const QList<Command *> &getCommands(Int32 index);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the macroes at the given index.
        ///
        /// The index corresponds to the order in which the files
        /// were parsed. The default script file is not guaranteed
        /// to be first (zero) one.
        ///
        /// \param index Index of the file to fetch macroes from.
        ///
        ///////////////////////////////////////////////////////////
        static const QList<Macro *> &getMacroes(Int32 index);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the file names of the loaded files.
        ///
        /// The files names are listed in when they were loaded.
        ///
        ///////////////////////////////////////////////////////////
        static const QStringList &getFileNames();


        ///////////////////////////////////////////////////////////
        /// \brief Sets the active command and macro list.
        ///
        ///////////////////////////////////////////////////////////
        static void setActive(Int32 index);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the active command list.
        ///
        ///////////////////////////////////////////////////////////
        static const QList<Command *> &activeCommands();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the active macro list.
        ///
        ///////////////////////////////////////////////////////////
        static const QList<Macro *> &activeMacroes();


        ///////////////////////////////////////////////////////////
        /// \brief Finds a command structure by byte.
        ///
        /// Note: The active command file will be used for searching.
        /// Change the active index via "setActive" to change this.
        ///
        ///////////////////////////////////////////////////////////
        static const Command *findByByte(UInt8 byte);

        ///////////////////////////////////////////////////////////
        /// \brief Finds a command structure by name.
        ///
        /// Note: The active command file will be used for searching.
        /// Change the active index via "setActive" to change this.
        ///
        ///////////////////////////////////////////////////////////
        static const Command *findByName(const QString &name);


        ///////////////////////////////////////////////////////////
        /// \brief Finds all possible macroes by a given command.
        ///
        /// Note: The active command file will be used for searching.
        /// Change the active index via "setActive" to change this.
        ///
        ///////////////////////////////////////////////////////////
        static const QList<Macro *> findMacroByCmd(const Command *cmd);


    private:

        ///////////////////////////////////////////////////////////
        // Static class members
        //
        ///////////////////////////////////////////////////////////
        static QList<QList<Command *>>     m_Commands;
        static QList<QList<Macro *>>       m_Macroes;
        static QStringList                 m_FileNames;
        static Int32                       m_Count;
        static Int32                       m_Active;
    };
}


#endif  // __ASE_CONFIGURATION_HPP__

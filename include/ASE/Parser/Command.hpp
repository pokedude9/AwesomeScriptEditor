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


#ifndef __ASE_COMMAND_HPP__
#define __ASE_COMMAND_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <ASE/Parser/Parameter.hpp>
#include <QList>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    Command.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Defines a command with multiple parameters.
    ///
    /// Holds the command byte, the parameters and other things
    /// concerning the intellisense feature.
    ///
    ///////////////////////////////////////////////////////////
    class Command {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::Command.
        ///
        ///////////////////////////////////////////////////////////
        Command();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the numeric representation.
        ///
        /// Fetches the byte value that represents the command.
        ///
        /// \returns the readonly byte value of the command.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 byte() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the command's name.
        ///
        /// This property is essential to the compiling process
        /// because it will be compared to text in the editor but
        /// also to the decompiling process while generating the
        /// human-readable script file.
        ///
        /// \returns the readonly name of the command.
        ///
        ///////////////////////////////////////////////////////////
        const QString &name() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the command's description.
        ///
        /// This property is essential to the intellisense and the
        /// user manual, as it clearly states what the command does
        /// in the game.
        ///
        /// \returns the readonly description of the command.
        ///
        ///////////////////////////////////////////////////////////
        const QString &info() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves a list of the parameters.
        ///
        /// Essential property in the (de)compiling process. It
        /// determines how many bytes are fetched from the ROM
        /// per command and also in which format they are fetched.
        ///
        /// \returns the readonly list of parameters.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Parameter *> &params() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the size of one command.
        ///
        /// The size of the command is measured in the size of
        /// the command itself (1 byte) plus the size of each
        /// parameter that is required for the command.
        ///
        /// \returns the absolute size of the command.
        ///
        ///////////////////////////////////////////////////////////
        int size() const;


        ///////////////////////////////////////////////////////////
        /// \brief Modifies the numeric representation.
        ///
        /// This method is exclusively used in the configuration
        /// file parsing process and should not be used elsewhere.
        ///
        /// \param byte Numeric representation of the command
        ///
        ///////////////////////////////////////////////////////////
        void setByte(UInt8 byte);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the command's name.
        ///
        /// This method is exclusively used in the configuration
        /// file parsing process and should not be used elsewhere.
        ///
        /// \param name The name of the command
        ///
        ///////////////////////////////////////////////////////////
        void setName(const QString &name);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the command's description.
        ///
        /// This method is exclusively used in the configuration
        /// file parsing process and should not be used elsewhere.
        ///
        /// \param info The description of the command
        ///
        ///////////////////////////////////////////////////////////
        void setInfo(const QString &info);

        ///////////////////////////////////////////////////////////
        /// \brief Modifies the command's parameters.
        ///
        /// This method is exclusively used in the configuration
        /// file parsing process and should not be used elsewhere.
        ///
        /// \param params The parameters of the command
        ///
        ///////////////////////////////////////////////////////////
        void setParams(const QList<Parameter *> &params);


        ///////////////////////////////////////////////////////////
        /// \brief Reads the command's parameters and parses them.
        ///
        /// Parses the command's parameters to readable text.
        ///
        /// \param params A script-line with the specified command
        ///
        ///////////////////////////////////////////////////////////
        const QString read(const qboy::Rom &rom) const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt8 m_Byte;
        QString m_Name;
        QString m_Info;
        QList<Parameter*> m_Params;
    };
}


#endif  // __ASE_COMMAND_HPP__

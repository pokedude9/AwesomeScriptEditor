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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/Parser/Command.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    Command::Command()
        : m_Byte(0),
          m_Name(QString::null),
          m_Info(QString::null)
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    UInt8 Command::byte() const
    {
        return m_Byte;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString &Command::name() const
    {
        return m_Name;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString &Command::info() const
    {
        return m_Info;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Parameter *> &Command::params() const
    {
        return m_Params;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    int Command::size() const
    {
        int size = 1;
        int paramCount = m_Params.size();

        for (int i = 0; i < paramCount; i++)
            size += ParameterTypeHelper::GetParamSize(m_Params.at(i)->type());

        return size;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Command::setByte(UInt8 byte)
    {
        m_Byte = byte;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Command::setName(const QString &name)
    {
        m_Name = name;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Command::setInfo(const QString &info)
    {
        m_Info = info;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Command::setParams(const QList<Parameter *> &params)
    {
        m_Params = params;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString Command::read(const qboy::Rom &rom) const
    {
        // Appends the name, to begin with
        QString commandString(m_Name);
        Boolean isSpecial = m_Name.contains("{");

        // Then appends all the parameters
        for (int i = 0; i < m_Params.size(); i++)
        {
            UInt32 paramValue = 0;
            ParameterType type = m_Params.at(i)->type();

            // Determines the parameter type and reads data in dependance
            switch (type)
            {
                case PT_Byte:
                    paramValue = rom.readByte();
                    break;
                case PT_HWord:
                    paramValue = rom.readHWord();
                    break;
                case PT_Word:
                    paramValue = rom.readWord();
                    break;
                default:
                    paramValue = rom.readWord();
                    if (paramValue > 0x08000000)
                        paramValue -= 0x08000000;
                    break;
            }

            // Converts the value to a hexadecimal value
            if (!isSpecial)
                commandString.append(QString(" 0x") + QString::number(paramValue, 16).toUpper());
            else
                commandString.replace(
                    QString("{") + QString::number(i) + QString("}"),
                    QString("0x") + QString::number(paramValue, 16).toUpper()
                );
        }

        return commandString;
    }
}

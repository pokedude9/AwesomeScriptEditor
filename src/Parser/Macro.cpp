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
#include <ASE/System/Configuration.hpp>
#include <ASE/Parser/Macro.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    Macro::Macro()
        : m_Name(QString::null),
          m_Info(QString::null)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    Macro::~Macro()
    {
        foreach (MacroEntry *entry, m_Entries)
            delete entry;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString &Macro::name() const
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
    const QString &Macro::info() const
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
    const QList<MacroEntry *> &Macro::entries() const
    {
        return m_Entries;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<const Command *> &Macro::commands() const
    {
        return m_Commands;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Macro::setName(const QString &name)
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
    void Macro::setInfo(const QString &info)
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
    void Macro::setEntries(const QList<MacroEntry *> &entries)
    {
        m_Entries = entries;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Macro::parse()
    {
        // Finds the command for every macro entry
        foreach (const MacroEntry *entry, m_Entries)
        {
            const Command *cmd = Configuration::findByName(entry->command());
            if (cmd != NULL)
                m_Commands.push_back(cmd);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString Macro::read(const qboy::Rom &rom) const
    {
        // Defines the starting sequence of the macro
        QString macroString(m_Name);
        if (CONFIG(Language) == BL_ASE) // identation
            macroString.insert(0, "    ");

        // Goes through every command parameter and appends the dynamic ones
        int commandPosition = 0;
        foreach (const Command *cmd, m_Commands)
        {
            // Reads the command byte (ignored)
            rom.readByte();

            // Loops through every parameter
            int paramPosition = 0;
            foreach (const Parameter *param, cmd->params())
            {
                MacroParam *macro = m_Entries.at(commandPosition)->params().at(paramPosition++);

                // Determines whether the current parameter is fixed or dynamic
                if (macro->isFixed())
                {
                    // Skip the current parameter, as it is not needed in the decompiling process
                    rom.readBytes(ParameterTypeHelper::GetParamSize(param->type()));
                }
                else
                {
                    // Reads the value of the command
                    if (param->type() == PT_Byte)
                        macroString.append(QString(" 0x") + QString::number(rom.readByte(), 16).toUpper());
                    else if (param->type() == PT_HWord)
                        macroString.append(QString(" 0x") + QString::number(rom.readHWord(), 16).toUpper());
                    else if (param->type() == PT_Word)
                        macroString.append(QString(" 0x") + QString::number(rom.readWord(), 16).toUpper());
                    else
                    {
                        // Can be a ROM pointer, in that case remove the RAM prefix
                        UInt32 word = rom.readWord();
                        if (word > 0x08000000)
                            word -= 0x08000000;

                        macroString.append(QString(" 0x") + QString::number(word, 16).toUpper());
                    }
                }
            }

            commandPosition++;
        }

        // Returns the parsed macro string
        return macroString;
    }
}

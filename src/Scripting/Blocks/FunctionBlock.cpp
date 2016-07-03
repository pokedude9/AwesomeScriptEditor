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
//////////////////////////////////////////////////////////////////////////////////_


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/System/Configuration.hpp>
#include <ASE/Scripting/Blocks/FunctionBlock.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    FunctionBlock::FunctionBlock(UInt32 offset)
        : ScriptBlock(offset)
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString FunctionBlock::readData(const qboy::Rom &rom)
    {
        QString scriptString;
        if (CONFIG(Language) == BL_XSE)
            scriptString.append(QString("#org 0x") + QString::number(m_Offset, 16).toUpper());
        else
            scriptString.append(QString(".org 0x") + QString::number(m_Offset, 16).toUpper() + QString(":"));


        // Determines whether the data offset is in ROM range
        if (!rom.seek(m_Offset))
            goto error;


        // Loops until one of the function terminators were found
        while (true)
        {
            if (!rom.canRead(VT_Byte))
                goto error;


            // Reads the next command byte and finds it by name
            unsigned char commandByte = rom.readByte();
            const Command *cmd = Configuration::findByByte(commandByte);
            if (cmd == NULL)
            {
                // Command not existing, append raw byte data
                if (CONFIG(Language) == BL_XSE)
                    scriptString.append(QString("\n#byte 0x") + QString::number(commandByte, 16).toUpper());
                else
                    scriptString.append(QString("\n.byte 0x") + QString::number(commandByte, 16).toUpper());
                continue;
            }


            // Finds a possible macro definition. This is done by three main steps:
            // 1. Compares the first command of the macro with the current one.
            // 2. If successful, compares all the following commands of the macro.
            // 3. If successful, the macro replaces these multiple commands.
            Boolean foundMacro = false;
            UInt32 temporaryOffset = rom.offset();
            rom.seek(temporaryOffset + cmd->size()-1);

            const QList<Macro *> possibleMacroes = Configuration::findMacroByCmd(cmd);
            for (int i = 0; i < possibleMacroes.size(); i++)
            {
                // Compares the following commands of the macro
                int foundCommands = 1;
                const Macro *macro = possibleMacroes.at(i);
                for (int j = 1; j < macro->commands().size(); j++)
                {
                    // Retrieves the current command structure
                    unsigned char byte = rom.readByte();
                    const Command *nextCmd = Configuration::findByByte(byte);
                    if (nextCmd == NULL)
                        break;

                    // Seeks to the next command beforehand and pushes
                    // the current cmd back in case it equals the macro command
                    if (macro->commands().at(j) == nextCmd)
                        foundCommands++;
                }

                // Not a macro, break the FOR iteration and continue parsing
                if (foundCommands < macro->commands().size())
                    continue;

                // Is a macro, parses it to a readable string
                rom.seek(temporaryOffset-1);
                scriptString.append("\n");
                scriptString.append(macro->read(rom));
                foundMacro = true;
                break;
            }

            // Do not parse command if a macro was already found
            if (foundMacro)
                continue;

            // Assures that command can be read
            if (!rom.seek(temporaryOffset))
                goto error;

            // Reads the command
            scriptString.append("\n");
            scriptString.append(cmd->read(rom));


            // If any terminating command occured, abort further reading
            if (commandByte == 2 || commandByte == 3 || commandByte == 5)
                return scriptString;
        }

    error:

        QString prefix;
        if (CONFIG(Language) == BL_XSE)
            prefix = "#";
        else
            prefix = ".";

        // Generates an error if failing to read data within the block
        QString firstLine = QString("// " + prefix + "org 0x") + QString::number(m_Offset, 16).toUpper();
        QString secondLine = QString("\n// Error: Tried to read data beyond ROM bounds.");
        return firstLine + secondLine;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void FunctionBlock::writeData(const qboy::Rom &rom)
    {
        Q_UNUSED(rom);
    }
}

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
#include <ASE/Scripting/Decompiler.hpp>
#include <ASE/Scripting/Blocks/MoveBlock.hpp>
#include <ASE/Scripting/Blocks/ItemBlock.hpp>
#include <ASE/Scripting/Blocks/StringBlock.hpp>
#include <ASE/Scripting/Blocks/FunctionBlock.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString Decompiler::decompile(const qboy::Rom &rom, UInt32 offset)
    {
        ASEScript script;
        script.addBlock(new FunctionBlock(offset), BP_High);
        decompileSub(rom, offset, script);
        return script.parse(rom);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Decompiler::decompileSub(const qboy::Rom &rom, UInt32 offset, ASEScript &script)
    {
        UInt32 restore = rom.offset();
        UInt8 cmdByte = 0;

        // Seeks to the current function offset
        if (!rom.seek(offset))
            return;

        // Loops throught the current block until it is terminated
        while (cmdByte == 0x4 || cmdByte < 0x2 || cmdByte > 0x5)
        {
            if (!rom.canRead(VT_Byte))
                goto end;

            cmdByte = rom.readByte();


            // Is it one of goto- or call-type commands?
            if (cmdByte >= 0x4 && cmdByte <= 0x7)
            {
                if (cmdByte == 0x6 || cmdByte == 0x7)
                    rom.readByte(); // condition not necessary

                UInt32 tempPointer = rom.readPointer();

                // Checks whether the pointer is within the ROM
                if (rom.checkOffset(tempPointer))
                {
                    script.addBlock(new FunctionBlock(tempPointer), BP_High);
                    decompileSub(rom, tempPointer, script);
                }

                continue;
            }


            // Retrieves the command associated to the command byte
            // and checks whether it contains a parsable symbol
            const Command *cmdStruct = Configuration::findByByte(cmdByte);
            if (cmdStruct != NULL)
            {
                if (!rom.checkOffset(rom.offset() + cmdStruct->size()))
                    goto end;

                // Does it contain a sub-function or data?
                int paramSize = cmdStruct->params().size();
                for (int i = 0; i < paramSize; i++)
                {
                    ParameterType type = cmdStruct->params().at(i)->type();
                    if (type == PT_StringPointer)
                    {
                        UInt32 tempPointer = rom.readPointer();

                        // Checks whether the pointer is within the ROM
                        if (rom.checkOffset(tempPointer))
                            script.addBlock(new StringBlock(tempPointer), BP_Medium);
                    }
                    else if (type == PT_ItemPointer)
                    {
                        UInt32 tempPointer = rom.readPointer();

                        // Checks whether the pointer is within the ROM
                        if (rom.checkOffset(tempPointer))
                            script.addBlock(new ItemBlock(tempPointer), BP_Low);
                    }
                    else if (type == PT_MovePointer)
                    {
                        UInt32 tempPointer = rom.readPointer();

                        // Checks whether the pointer is within the ROM
                        if (rom.checkOffset(tempPointer))
                            script.addBlock(new MoveBlock(tempPointer), BP_Low);
                    }
                    else
                    {
                        // Just skip the bytes, they are not necessary now
                        rom.readBytes(ParameterTypeHelper::GetParamSize(type));
                    }
                }
            }
        }


        // Restores the offset for the parental function
    end:
        rom.seek(restore);
    }
}

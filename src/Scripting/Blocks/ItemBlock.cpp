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
#include <ASE/Scripting/Blocks/ItemBlock.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ItemBlock::ItemBlock(UInt32 offset)
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
    const QString ItemBlock::readData(const qboy::Rom &rom)
    {
        if (!rom.seek(m_Offset))
        {
            // Generates an error if failing to read data within the block
            QString firstLine = QString("// #org 0x") + QString::number(m_Offset, 16).toUpper();
            QString secondLine = QString("\n// Error: Tried to read data beyond ROM bounds.");
            return firstLine + secondLine;
        }


        QString itemString = QString("#org 0x") + QString::number(m_Offset, 16).toUpper();

        // Reads half-words until the terminator 0x0 was read
        while (true)
        {
            UInt16 item = rom.readHWord();
            itemString.append(QString("\n#item 0x") + QString::number(item, 16).toUpper());

            if (item == TERMINATOR_ITEM)
                break;
        }

        return itemString;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ItemBlock::writeData(const qboy::Rom &rom)
    {
        Q_UNUSED(rom);
    }
}

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
#include <ASE/Scripting/ASEScript.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEScript::ASEScript()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEScript::~ASEScript()
    {
        foreach (ScriptBlock *block, m_Blocks)
            delete block;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEScript::addBlock(ScriptBlock *block, BlockPriority priority)
    {
        // Checks whether the block already exists
        int size = m_Blocks.size();
        for (int i = 0; i < size; i++)
            if (m_Blocks.at(i)->offset() == block->offset())
                return;

        // Adds the block, if it does not
        m_Blocks.push_back(block);
        m_Priorities.push_back(priority);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QString ASEScript::parse(const qboy::Rom &rom) const
    {
        QStringList stringsHigh;
        QStringList stringsMed;
        QStringList stringsLow;

        int size = m_Blocks.size();
        for (int i = 0; i < size; i++)
        {
            if (m_Priorities.at(i) == BP_High)
                stringsHigh.push_back(m_Blocks.at(i)->readData(rom));
            else if (m_Priorities.at(i) == BP_Medium)
                stringsMed.push_back(m_Blocks.at(i)->readData(rom));
            else
                stringsLow.push_back(m_Blocks.at(i)->readData(rom));
        }

        QString full;
        if (!stringsHigh.isEmpty())
        {
            full += "////////////////////////////////////////\n// Functions (" + QString::number(stringsHigh.size()) + ")\n//\n////////////////////////////////////////\n";
            full += stringsHigh.join(QByteArray("\n\n")).toStdString().c_str();
        }
        if (!stringsMed.isEmpty())
        {
            full += "\n\n\n////////////////////////////////////////\n// String data (" + QString::number(stringsMed.size()) + ")\n//\n////////////////////////////////////////\n";
            full += stringsMed.join(QByteArray("\n\n")).toStdString().c_str();
        }
        if (!stringsLow.isEmpty())
        {
            full += "\n\n\n////////////////////////////////////////\n// Miscellaneous data (" + QString::number(stringsLow.size()) + ")\n//\n////////////////////////////////////////\n";
            full += stringsLow.join(QByteArray("\n\n")).toStdString().c_str();
        }

        return full;
   }
}

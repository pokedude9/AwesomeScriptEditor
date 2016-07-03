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
#include <QBoy/Config.hpp>
#include <ASE/Widgets/ASEEditor.hpp>
#include <ASE/Widgets/ASEEditorLineWidget.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    int ASEEditor::widgetLineWidth() const
    {
        // Computes the number of digits
        int digits = 1;
        int maxDig = qMax(digits, document()->blockCount());
        while (maxDig >= 10)
        {
            maxDig /= 10;
            digits++;
        }


        // Returns the approximate width of the widget
        return (fontMetrics().width(QLatin1Char('9')) * digits) + 32;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ////////////////////////////////////////////////////////////
    void ASEEditor::updateLineWidgetWidth(int lineCount)
    {
        Q_UNUSED(lineCount); // used by widgetLineWidth() anyways...
        setViewportMargins(widgetLineWidth(), 0, 0, 0);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::updateLineWidgetScroll(QRect r, int y)
    {
        if (y != 0)
            m_LineWidget->scroll(0, y);
        else
            m_LineWidget->update(0, r.y(), m_LineWidget->width(), r.height());

        if (r.contains(viewport()->rect()))
            updateLineWidgetWidth(0);
    }

    ///////////////////////////////////////////////////////////
    void ASEEditor::editorInsertCompletion(const QString &full)
    {
        QTextCursor current = textCursor();
        Int32 wordLength = full.length() - m_AutoComplete->completionPrefix().length();
        current.movePosition(QTextCursor::Left);
        current.movePosition(QTextCursor::EndOfWord);
        current.insertText(full.right(wordLength));
        setTextCursor(current);
    }
}

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
#include <ASE/Widgets/ASEHexBox.hpp>
#include <QBoy/Config.hpp>
#include <QPainter>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEHexBox::ASEHexBox(QWidget *parent)
        : QLineEdit(parent)
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEHexBox::keyPressEvent(QKeyEvent *event)
    {
        // Backspace and shortcuts are permitted
        if (event->key() == Qt::Key_Backspace ||
            ((event->modifiers() & Qt::ControlModifier) &&
             (event->key() == Qt::Key_C ||  // CTRL + C (copy)
              event->key() == Qt::Key_V ||  // CTRL + V (paste)
              event->key() == Qt::Key_X ||  // CTRL + X (cut)
              event->key() == Qt::Key_A)))  // CTRL + A (select)
        {
            // Special case: Abort if clipboard is not a hexadeximal number
            if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_V)
            {
                QString clipboardText = QApplication::clipboard()->text();
                bool success = false;

                clipboardText.toInt(&success, 16);
                if (success == false)
                {
                    // Cannot paste!
                    return;
                }

            }

            QLineEdit::keyPressEvent(event);
            return;
        }


        // Tests whether the character is hexadecimal
        bool success = false;
        event->text().toInt(&success, 16);
        if (success == false)
            return;

        // Processes the character and displays it
        QLineEdit::keyPressEvent(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEHexBox::paintEvent(QPaintEvent *event)
    {
        // Paints text, background and border
        QLineEdit::paintEvent(event);


        // Calculates the bounding factors
        Int32 x = contentsMargins().left() + 3;
        Int32 w = fontMetrics().width("0x");
        Int32 h = height() - 1;

        // Paints the hexadecimal prefix
        QPainter painter(this);
        painter.setPen(palette().text().color());
        painter.drawText(x, 0, w, h, Qt::AlignVCenter, "0x");
    }
}

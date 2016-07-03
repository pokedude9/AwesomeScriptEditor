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
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::paintEvent(QPaintEvent *event)
    {
        QPlainTextEdit::paintEvent(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::keyReleaseEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Tab)
            return;

        QPlainTextEdit::keyReleaseEvent(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::keyPressEvent(QKeyEvent *event)
    {
        // The keys in the switch will be forwarded
        if (m_AutoComplete->popup()->isVisible())
        {
            switch (event->key())
            {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Escape:
                case Qt::Key_Tab:
                case Qt::Key_Backtab:
                    event->ignore();
                    return;
                default:
                    break;
            }
        }


        if (event->key() == Qt::Key_Tab)
        {
            QTextCursor cursor = textCursor();
            cursor.insertText("    ");
            setTextCursor(cursor);
            return;
        }

        // Processes default key-press behaviour
        QPlainTextEdit::keyPressEvent(event);

        // If backspace was pressed, close box immediately
        if (event->key() == Qt::Key_Backspace)
        {
            m_AutoComplete->popup()->hide();
            return;
        }


        // Determines whether a modifier is pressed.
        // Aborts completing, if at least one was pressed.
        const bool hasMod = event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier);
        if (hasMod && event->text().isEmpty())
            return;

        // Aborts completion if line contains a preprocessor character
        if (textCursor().block().text().contains('#'))
        {
            m_AutoComplete->popup()->hide();
            return;
        }


        // Retrieves the currently typed word
        QString prefix;
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::WordUnderCursor);
        prefix = cursor.selectedText();

        // Determines whether this word is valid and hides
        // the auto-complete box under certain conditions.
        static QString invalidChars("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
        bool hasAnyMod = (event->modifiers() != Qt::NoModifier) && !hasMod;
        if (hasAnyMod || event->text().isEmpty() || prefix.length() < 3 || invalidChars.contains(event->text().right(1)))
        {
            m_AutoComplete->popup()->hide();
            return;
        }

        // Filters the keyword list by a specific regular expression
        m_AutoComplete->setCompletionPrefix(prefix);
        m_FilterModel->setFilterRegExp(QString("^") + prefix + ".*$");
        m_AutoComplete->popup()->setCurrentIndex(m_AutoComplete->completionModel()->index(0, 0));

        // Determines whether any match has been found
        if (m_AutoComplete->popup()->model()->rowCount() == 0)
        {
            m_AutoComplete->popup()->hide();
            return;
        }


        // Shows the auto-complete box, if not already
        if (!m_AutoComplete->popup()->isVisible())
        {
            // Modifies the position of the popup menu
            QRect rect = cursorRect();
            rect.moveTo(rect.x() + widgetLineWidth() - fontMetrics().width(prefix), rect.y() + 4);
            rect.setWidth(m_AutoComplete->popup()->sizeHintForColumn(0) + m_AutoComplete->popup()->verticalScrollBar()->sizeHint().width());

            // Shows the popup menu
            m_AutoComplete->complete(rect);
            // TODO: Show tip in custom popup
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::resizeEvent(QResizeEvent *event)
    {
        Q_UNUSED(event);

        // Makes space for the line widget
        QRect content = contentsRect();
        setViewportMargins(widgetLineWidth() + 2, 0, 0, 0);
        m_LineWidget->setGeometry(
            content.left(),
            content.top(),
            widgetLineWidth(),
            content.height()
        );
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::paintEventLineWidget(QPaintEvent *event)
    {
        QRect content = event->rect();
        QPainter painter(m_LineWidget);

        // Paints the background and the separator
        painter.fillRect(content, m_ClrLineBkgr);
        painter.setPen(m_ClrLineFrgr);
        painter.drawLine(
            content.width() - 1, 0,
            content.width() - 1, m_LineWidget->height() - 1
        );


        // Paints all the visible line numbers
        painter.setPen(m_ClrLineText);
        QTextBlock block = firstVisibleBlock();
        Int32 blockNumber = block.blockNumber();
        Int32 topPos = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
        Int32 btmPos = static_cast<int>(blockBoundingRect(block).height()) + topPos;

        while (block.isValid() && topPos <= content.bottom())
        {
            if (block.isVisible() && btmPos >= content.top())
            {
                painter.drawText(
                    0, topPos,
                    m_LineWidget->width() - 16, fontMetrics().height(),
                    Qt::AlignRight, QString::number(blockNumber + 1)
                );
            }

            block = block.next();
            topPos = btmPos;
            btmPos = topPos + static_cast<int>(blockBoundingRect(block).height());
            blockNumber++;
        }
    }
}

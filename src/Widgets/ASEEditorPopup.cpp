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
#include <ASE/Widgets/ASEEditorPopup.hpp>
#include <ASE/Widgets/ASEEditorPopupLabel.hpp>
#include <ASE/Widgets/Misc/StyleSheets.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditorPopup::ASEEditorPopup(QWidget *parent)
        : QListView(parent),
          m_Label(NULL)
    {
        // Loads all the style sheets for this popup
        m_Sheet = StyleSheets::loadSheet("ASEEditorPopup");
        m_Sheet2 = StyleSheets::loadSheet("ASEEditorPopup2");


        // Changes properties from the listview
        setMouseTracking(true);
        setFrameShape(QFrame::NoFrame);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setSelectionBehavior(QAbstractItemView::SelectItems);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        verticalScrollBar()->setFixedHeight(162);
        setFixedHeight(207);
        setFixedWidth(300);
        setStyleSheet(m_Sheet);


        // Constructs the child label plus layout
        m_Label = new ASEEditorPopupLabel(this);
        m_Label->setGeometry(0, 163, 300, 44);
        m_Label->setFixedHeight(44);
        m_Label->setFixedWidth(300);
        m_Label->setText("No description.");
        viewport()->setFixedHeight(163);

        // Retrieves the default height of a two-line-text
        m_DefaultHeight = fontMetrics().boundingRect(m_Label->contentsRect(), Qt::TextWordWrap, "T\nT").height()+6;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditorPopup::~ASEEditorPopup()
    {
        delete m_Label;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool ASEEditorPopup::viewportEvent(QEvent *event)
    {
        if (event->type() == QEvent::MouseMove)
        {
            // Selects the item at mouse cursor position.
            QModelIndex index = indexAt(static_cast<QMouseEvent*>(event)->pos());
            selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            // Hack: There is no pressed state in QListView's QSS
            if (!selectedIndexes().isEmpty())
                setStyleSheet(m_Sheet2);
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            setStyleSheet(m_Sheet);
        }

        // Processes the default event logic
        return QListView::viewportEvent(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditorPopup::updateInfo(const QModelIndex &index)
    {
        // Hack: We must override the default QItemDelegate in
        // order to style our model-items. This hack does not
        // seem to work in any other functions except this one.
        setItemDelegate(new QStyledItemDelegate);

        // Retrieves the info if an item was selected
        if (index.isValid())
        {
            // Takes the info data; guaranteed to be a string
            QString info = index.data(Qt::UserRole).toString();
            if (!info.isNull())
            {
                m_Label->setText(info);

                // Checks if the info text fits into two lines
                int theight = fontMetrics().boundingRect(m_Label->contentsRect(), Qt::TextWordWrap, info).height();
                if (theight > m_DefaultHeight)
                {
                    setFixedHeight(207 + theight-m_DefaultHeight);
                    m_Label->setFixedHeight(theight+m_DefaultHeight-20);
                }
                else if (m_Label->height() != 44)
                {
                    setFixedHeight(207);
                    m_Label->setFixedHeight(44);
                }
            }
            else
            {
                m_Label->setText("No description.");
            }
        }
        else
        {
            m_Label->setText("No description.");
        }
    }
}

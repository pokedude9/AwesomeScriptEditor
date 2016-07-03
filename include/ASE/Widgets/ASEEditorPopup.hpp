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


#ifndef __ASE_EDITORPOPUP_HPP__
#define __ASE_EDITORPOPUP_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QtWidgets>
#include <QtEvents>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Forward declarations
    //
    ///////////////////////////////////////////////////////////
    class ASEEditorPopupLabel;


    ///////////////////////////////////////////////////////////
    /// \file    ASEEditorPopup.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Extends an item view with custom implementations.
    ///
    /// When selecting a function, a label on the bottom
    /// should be displayed with an appropriate description.
    ///
    ////////////////////////////////////////////////////////////
    class ASEEditorPopup : public QListView {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEEditorPopup.
        ///
        ///////////////////////////////////////////////////////////
        ASEEditorPopup(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases the label widget used by ase::ASEEditorPopup.
        ///
        ///////////////////////////////////////////////////////////
        ~ASEEditorPopup();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Enables selection with the mouse pointer.
        ///
        /// Intercepts the mouse move event and selects the item
        /// that is currently underneath the mouse pointer.
        ///
        ///////////////////////////////////////////////////////////
        bool viewportEvent(QEvent *event) Q_DECL_OVERRIDE;


    private slots:

        ///////////////////////////////////////////////////////////
        /// \brief Changes the information.
        ///
        /// When the selected item is changed via the keyboard,
        /// the info attached to the function should be displayed.
        /// If a macro is selected, "No description" will be shown.
        ///
        /// \param selection Cut to a single-item selection
        ///
        ///////////////////////////////////////////////////////////
        void updateInfo(const QModelIndex &index);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        ASEEditorPopupLabel     *m_Label;
        QString                 m_Sheet;
        QString                 m_Sheet2;
        Int32                   m_DefaultHeight;
    };
}


#endif  // __ASE_EDITORPOPUP_HPP__

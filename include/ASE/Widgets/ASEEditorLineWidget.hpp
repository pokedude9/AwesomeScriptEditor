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


#ifndef __ASE_EDITORLINEWIDGET_HPP__
#define __ASE_EDITORLINEWIDGET_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtGui/QtGui>
#include <QWidget>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Forward declarations
    //
    ///////////////////////////////////////////////////////////
    class ASEEditor;


    ///////////////////////////////////////////////////////////
    /// \file    ASEEditorLineWidget.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Paints the line column and the line numbers.
    ///
    ////////////////////////////////////////////////////////////
    class ASEEditorLineWidget : public QWidget {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEEditorLineWidget.
        /// Sets the given ase::ASEEditor as parental editor.
        ///
        ///////////////////////////////////////////////////////////
        ASEEditorLineWidget(ASEEditor *parent);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources used by ase::ASEEditorLineWidget.
        ///
        ///////////////////////////////////////////////////////////
        ~ASEEditorLineWidget();


        ///////////////////////////////////////////////////////////
        /// \brief Manipulates the size hint.
        ///
        /// Automatically occupies some space in the ase::ASEEditor
        /// in order to correctly display the line numbers.
        ///
        ///////////////////////////////////////////////////////////
        QSize sizeHint() const;


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Paints the column and the line numbers.
        /// \param event Event containing the visible region
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        ASEEditor *m_Parent;
    };
}


#endif  // __ASE_EDITORLINEWIDGET_HPP__

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


#ifndef __ASE_EDITORPOPUPLABEL_HPP__
#define __ASE_EDITORPOPUPLABEL_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QLabel>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ASEEditorPopupLabel.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Returns a size hint to avoid using layouts.
    ///
    ////////////////////////////////////////////////////////////
    class ASEEditorPopupLabel : public QLabel {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEEditorPopupLabel.
        ///
        ///////////////////////////////////////////////////////////
        ASEEditorPopupLabel(QWidget *parent = NULL);


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Returns the recommended size for this label.
        ///
        ///////////////////////////////////////////////////////////
        QSize sizeHint() const;

        ///////////////////////////////////////////////////////////
        /// \brief Paints a small information icon on the right.
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    };
}


#endif  // __ASE_EDITORPOPUPLABEL_HPP__

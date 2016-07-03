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


#ifndef __ASE_HEXBOX_HPP__
#define __ASE_HEXBOX_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtWidgets>
#include <QtEvents>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ASEHexBox.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Prevents non-hexadecimal input from the user.
    ///
    /// If the user types any character that is not contained
    /// in the hexadecimal set, it will be simply ignored.
    ///
    ////////////////////////////////////////////////////////////
    class ASEHexBox : public QLineEdit {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEHexBox.
        ///
        ///////////////////////////////////////////////////////////
        ASEHexBox(QWidget *parent = NULL);


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Blocks non-hexadecimal input.
        ///
        /// Things like shortcuts stay permitted.
        ///
        ///////////////////////////////////////////////////////////
        void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Paints a hexadecimal identifier.
        ///
        /// In front of the box, paints a hexadecimal prefix
        /// in order to indicate that hex-input is required.
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    };
}


#endif  // __ASE_HEXBOX_HPP__

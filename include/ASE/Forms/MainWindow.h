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


#ifndef __ASE_MAINWINDOW_H__
#define __ASE_MAINWINDOW_H__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/Widgets/ASEHexBox.hpp>
#include <QBoy/Core/Rom.hpp>
#include <QMainWindow>
#include <QtWidgets>


namespace ase
{
    namespace Ui
    {
        class MainWindow;
    }


    ///////////////////////////////////////////////////////////
    /// \file    MainWindow.h
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Holds components for the main ASE window.
    ///
    ///////////////////////////////////////////////////////////
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        /// \param parent Parental widget (optional)
        ///
        /// Creates all resources that are required by the GUI.
        ///
        ///////////////////////////////////////////////////////////
        explicit MainWindow(QWidget *parent = 0);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources that belong to MainWindow.
        ///
        ///////////////////////////////////////////////////////////
        ~MainWindow();


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Programmatically creates widgets.
        ///
        ///////////////////////////////////////////////////////////
        void setupAdvanced();

        ///////////////////////////////////////////////////////////
        /// \brief Sets up everything related to the commands.
        ///
        ///////////////////////////////////////////////////////////
        void setupCommands();

        ///////////////////////////////////////////////////////////
        /// \brief Sets up the GUI after loading a ROM.
        ///
        ///////////////////////////////////////////////////////////
        void setupAfterRom();

        ///////////////////////////////////////////////////////////
        /// \brief Resets the GUI to default (no ROM loaded).
        ///
        ///////////////////////////////////////////////////////////
        void resetUi();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        Ui::MainWindow  *ui;
        ASEHexBox       *m_DecompileBox;
        QComboBox       *m_CommandBox;
        QLabel          *m_FileLabel;
        qboy::Rom        m_Rom;
        Int32            m_PrevTab;
    };
}


#endif  // __ASE_MAINWINDOW_H__

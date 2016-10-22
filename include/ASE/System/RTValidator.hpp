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


#ifndef __ASE_RTVALIDATOR_HPP__
#define __ASE_RTVALIDATOR_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <ASE/System/RTWorker.hpp>
#include <QPlainTextEdit>
#include <QThread>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    RTValidator.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Debugs the current script in a different thread.
    ///
    /// Loops through all the script file lines and searches
    /// for possible errors.
    ///
    ///////////////////////////////////////////////////////////
    class RTValidator : public QObject {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::RTValidator.
        ///
        ///////////////////////////////////////////////////////////
        RTValidator(QPlainTextEdit *parent);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Terminates the thread and destroys all used resources.
        ///
        ///////////////////////////////////////////////////////////
        ~RTValidator();


        ///////////////////////////////////////////////////////////
        /// \brief Starts the real-time debugger.
        ///
        ///////////////////////////////////////////////////////////
        void start();

        ///////////////////////////////////////////////////////////
        /// \brief Stops the real-time debugger.
        ///
        ///////////////////////////////////////////////////////////
        void stop();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the worker and it's signals.
        ///
        ///////////////////////////////////////////////////////////
        RTWorker *worker() const;


    signals:

        ///////////////////////////////////////////////////////////
        /// \brief Emits the worker debug function.
        ///
        ///////////////////////////////////////////////////////////
        void validate(const QString code);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QPlainTextEdit      *m_Editor;
        QTextEdit           *m_Edit;
        RTWorker            *m_Worker;
        QThread             *m_Thread;
    };
}


#endif  // __ASE_RTVALIDATOR_HPP__

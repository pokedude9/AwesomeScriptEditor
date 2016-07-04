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


#ifndef __ASE_RTWORKER_HPP__
#define __ASE_RTWORKER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QMutex>
#include <QThread>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \brief Defines the notification types.
    ///
    ///////////////////////////////////////////////////////////
    enum NotifyType
    {
        NT_Error,
        NT_Warning
    };

    ///////////////////////////////////////////////////////////
    /// \brief Defines a notification entry.
    ///
    ///////////////////////////////////////////////////////////
    struct NotifyEntry
    {
        uint        line;
        NotifyType  type;
        QString     msg;
    };


    ///////////////////////////////////////////////////////////
    /// \file    RTWorker.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Debugs the current script in a different thread.
    ///
    /// Loops through all the script file lines and searches
    /// for possible errors.
    ///
    ///////////////////////////////////////////////////////////
    class RTWorker : public QObject {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether the function is running.
        ///
        ///////////////////////////////////////////////////////////
        bool isRunning();

        ///////////////////////////////////////////////////////////
        /// \brief Requests an abort.
        ///
        ///////////////////////////////////////////////////////////
        void interrupt();


    public slots:

        ///////////////////////////////////////////////////////////
        /// \brief Debugs the given script code.
        /// \param code Script code to debug
        ///
        ///////////////////////////////////////////////////////////
        void debug(const QString code);


    signals:

        ///////////////////////////////////////////////////////////
        /// \brief Signals that debugging has been finished.
        /// \param notifications Found errors and warnings
        ///
        ///////////////////////////////////////////////////////////
        void finished(const QList<NotifyEntry> notifications);
    };
}


#endif  // __ASE_RTWORKER_HPP__

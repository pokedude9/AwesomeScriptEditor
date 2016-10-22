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
#include <ASE/System/RTValidator.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    RTValidator::RTValidator(QPlainTextEdit *parent)
        : QObject(parent)
    {
        m_Editor = parent;
        m_Thread = new QThread;
        m_Worker = new RTWorker;
        m_Edit = new QTextEdit;
        m_Worker->moveToThread(m_Thread);

        connect(this, SIGNAL(validate(QString)), m_Worker, SLOT(debug(QString)));
        qRegisterMetaType<QList<NotifyEntry>>("QList<NotifyEntry>");
        qRegisterMetaType<QTextCursor>("QTextCursor");

        m_Worker->setTextEdit(m_Edit);
        m_Thread->start();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    RTValidator::~RTValidator()
    {
        if (m_Thread->isRunning())
            m_Thread->terminate();

        delete m_Thread;
        delete m_Worker;
        delete m_Edit;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void RTValidator::start()
    {
        if (m_Worker->isRunning())
        {
            // Previous operation appearantly not finished;
            // must wait for next interval
            return;
        }

        emit validate(m_Editor->toPlainText());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void RTValidator::stop()
    {
        if (m_Worker->isRunning())
        {
            // Requests a stop
            m_Worker->interrupt();
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    RTWorker *RTValidator::worker() const
    {
        return m_Worker;
    }
}

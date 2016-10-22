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
#include <ASE/System/RTWorker.hpp>
#include <ASE/System/RTErrors.hpp>
#include <ASE/System/Configuration.hpp>
#include <QRegularExpression>
#include <QTextCursor>


namespace ase
{
    namespace
    {
        ///////////////////////////////////////////////////////////
        // Local objects
        //
        ///////////////////////////////////////////////////////////
        QMutex mutex;
        bool running = false;
    }


    ///////////////////////////////////////////////////////////
    // Helper macro functions
    //
    ///////////////////////////////////////////////////////////
    #define rtWarning(err, ...)                               \
    {                                                         \
        NotifyEntry entry;                                    \
        entry.msg.sprintf(err, __VA_ARGS__);                  \
        entry.type = NT_Warning;                              \
        entry.line = i;                                       \
        notifications.push_back(entry);                       \
    }

    #define rtError(err, ...)                                 \
    {                                                         \
        NotifyEntry entry;                                    \
        entry.msg.sprintf(err, __VA_ARGS__);                  \
        entry.type = NT_Error;                                \
        entry.line = i;                                       \
        notifications.push_back(entry);                       \
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   10/22/2016
    //
    ///////////////////////////////////////////////////////////
    inline bool isValidDef(const QString &s, const QString &chk, const QStringList &m)
    {
        // Check if move/item/byte definition has a macro
        QString pre = s + " %0";
        for (const QString &mac : m)
            if (pre.arg(mac) == chk)
                return true;

        return false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    inline bool checkRange(QString str, quint64 max)
    {
        quint64 value;
        if (str.startsWith("0x"))
            value = str.remove(0, 2).toULongLong(NULL, 16);
        else
            value = str.toULongLong();

        return (value <= max);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    inline uint extractNumber(QString str)
    {
        uint value;
        if (str.startsWith("0x"))
            value = str.remove(0, 2).toUInt(NULL, 16);
        else
            value = str.toUInt();

        return value;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool RTWorker::isRunning()
    {
        bool r = running;
        return r;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void RTWorker::interrupt()
    {
        running = false;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   10/22/2016
    //
    ///////////////////////////////////////////////////////////
    void RTWorker::setTextEdit(QTextEdit *edit)
    {
        m_Edit = edit;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void RTWorker::debug(const QString code)
    {
        // Informs the program that the worker is currently working
        mutex.lock();
        running = true;
        mutex.unlock();


        bool isXSE = (CONFIG(Language) == BL_XSE);

        // Defines some precompiled regexes for preprocessor
        static QRegularExpression regByte("#byte\\s\\K(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
        static QRegularExpression regHalf("#hword\\s\\K(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
        static QRegularExpression regWord("#word\\s\\K(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
        static QRegularExpression regMove("#move\\s\\K(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
        static QRegularExpression regItem("#item\\s\\K(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
        static QRegularExpression regDef("#define\\s\\K(\\w+\\s\\S+)$");
        static QRegularExpression regInc("#include\\s\\K(<.+>|\".+\")$");
        static QRegularExpression regOrg("#org\\s\\K(@\\w+$)");
        static QRegularExpression regLbl("\\w+:$");

        // Predefined variables
        static QString comment1 = "//";
        static QChar comment2 = '\'';
        static QChar comment3 = ';';
        static QChar preproc = '#';
        static QString p1 = "#define", p2 = "#include",
                       p3 = "#org",    p4 = "#move",
                       p5 = "#item",   p6 = "#byte",
                       p7 = "#hword",  p8 = "#word";

        QStringList macros;


        // Splits the entire code up in several lines
        QList<NotifyEntry> notifications;
        QStringList codeLines = code.split('\n');
        int i = -1;
        for (const QString &q : codeLines)
        {
            // Return if line is a comment
            QString line = q.simplified(); i++;
            if (line.isEmpty()            ||
                line.startsWith(comment1) ||
                line.startsWith(comment2) ||
                line.startsWith(comment3)) {
                continue;
            }


            //////////////////////////////////////////////////////////////////////////////
            //                                                                          //
            //                                                                          //
            //          Determines whether line is preprocessor directive               //
            //                                                                          //
            //                                                                          //
            //////////////////////////////////////////////////////////////////////////////
            if (line.startsWith(preproc))
            {
                if (line.startsWith(p1))
                {
                    QRegularExpressionMatch m = regDef.match(line);
                    if (!m.hasMatch())
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_DEFINITION, arg0.toStdString().c_str());
                    }
                    else
                    {
                        int pos = m.captured().indexOf(' ');
                        macros.push_back(m.captured().left(pos));
                    }
                }
                else if (line.startsWith(p2))
                {
                    QRegularExpressionMatch m = regInc.match(line);
                    if (!m.hasMatch())
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_INCLUDE, arg0.toStdString().c_str());
                    }
                }
                else if (isXSE && line.startsWith(p3))
                {
                    QRegularExpressionMatch m = regOrg.match(line);
                    if (!m.hasMatch())
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_LABEL, arg0.toStdString().c_str());
                    }
                }
                else if (line.startsWith(p4))
                {
                    QRegularExpressionMatch m = regMove.match(line);
                    if (!m.hasMatch() && !isValidDef(p4, line, macros))
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_MOVEMENT, arg0.toStdString().c_str());
                    }
                    else
                    {
                        if (!checkRange(m.captured(), 0xFF))
                            rtError(RT_INVALID_BYTE, m.captured().toStdString().c_str());
                    }
                }
                else if (line.startsWith(p5))
                {
                    QRegularExpressionMatch m = regItem.match(line);
                    if (!m.hasMatch() && !isValidDef(p5, line, macros))
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_ITEM, arg0.toStdString().c_str());
                    }
                    else
                    {
                        if (!checkRange(m.captured(), 0xFFFF))
                            rtError(RT_INVALID_HWORD, m.captured().toStdString().c_str());
                    }
                }
                else if (line.startsWith(p6))
                {
                    QRegularExpressionMatch m = regByte.match(line);
                    if (!m.hasMatch() && !isValidDef(p6, line, macros))
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_ITEM, arg0.toStdString().c_str());
                    }
                    else
                    {
                        if (!checkRange(m.captured(), 0xFF))
                            rtError(RT_INVALID_HWORD, m.captured().toStdString().c_str());
                    }
                }
                else if (line.startsWith(p7))
                {
                    QRegularExpressionMatch m = regHalf.match(line);
                    if (!m.hasMatch() && !isValidDef(p7, line, macros))
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_ITEM, arg0.toStdString().c_str());
                    }
                    else
                    {
                        if (!checkRange(m.captured(), 0xFFFF))
                            rtError(RT_INVALID_HWORD, m.captured().toStdString().c_str());
                    }
                }
                else if (line.startsWith(p8))
                {
                    QRegularExpressionMatch m = regWord.match(line);
                    if (!m.hasMatch() && !isValidDef(p8, line, macros))
                    {
                        QString arg0 = line.mid(line.indexOf(' ') + 1);
                        rtError(RT_INVALID_ITEM, arg0.toStdString().c_str());
                    }
                    else
                    {
                        if (!checkRange(m.captured(), 0xFFFFFFFF))
                            rtError(RT_INVALID_HWORD, m.captured().toStdString().c_str());
                    }
                }
                else
                {
                    QString arg0 = line.mid(0, line.indexOf(' '));
                    rtError(RT_UNKNOWN_PREPROC, arg0.toStdString().c_str());
                }

                continue;
            }
            else if (!isXSE && line.contains(":"))
            {
                QRegularExpressionMatch m = regLbl.match(line);
                if (!m.hasMatch())
                    rtError(RT_INVALID_LABEL, line.toStdString().c_str());

                continue;
            }


            //////////////////////////////////////////////////////////////////////////////
            //                                                                          //
            //                                                                          //
            //         Determines whether line is a command or macro function           //
            //                                                                          //
            //                                                                          //
            //////////////////////////////////////////////////////////////////////////////
            // Finds the name of the command or macro function
            QTextCursor cursor = m_Edit->textCursor();
            cursor.select(QTextCursor::Document);
            cursor.removeSelectedText();
            cursor.insertText(line);
            cursor.movePosition(QTextCursor::Start);
            cursor.select(QTextCursor::WordUnderCursor);

            QString command = cursor.selectedText();
            const Command *cmd = Configuration::findByName(command);
            if (cmd != NULL)
            {
                // Check command parameters
                QStringList params;
                int space = line.indexOf(' ');
                while (space != -1)
                {
                    int next = line.indexOf(' ', ++space);
                    params.push_back(line.mid(space, next-space));
                    space = next;
                }
                if (params.size() != cmd->params().size())
                {
                    // Invalid argument count
                    int arg1 = params.size(), arg2 = cmd->params().size();
                    rtError(RT_INVALID_ARGCOUNT, command.toStdString().c_str(), arg1, arg2);
                    continue;
                }
            }
            else
            {
                // Attempts to find a macro function instead
                const Macro *macro = Configuration::findMacroByName(command);
                if (macro == NULL)
                {
                    // Unknown identifier
                    rtError(RT_UNKNOWN_IDENTIFIER, line.toStdString().c_str());
                    continue;
                }

                // Check macro parameters
                QStringList params;
                QList<MacroDynamicParam *> dynParams;
                int space = line.indexOf(' ');
                while (space != -1)
                {
                    int next = line.indexOf(' ', ++space);
                    params.push_back(line.mid(space, next-space));
                    space = next;
                }
                for (int j = 0; j < macro->entries().size(); ++j)
                {
                    dynParams.append(macro->entries().at(j)->dynamicParams());
                }
                if (params.size() != dynParams.size())
                {
                    // Invalid argument count
                    int arg1 = params.size(), arg2 = dynParams.size();
                    rtError(RT_INVALID_ARGCOUNT, command.toStdString().c_str(), arg1, arg2);
                    continue;
                }
            }
        }


        // Informs the program that the worker has finished
        mutex.lock();
        running = false;
        mutex.unlock();

        // Emits a signal, sends the notifications list
        emit finished(notifications);
    }
}

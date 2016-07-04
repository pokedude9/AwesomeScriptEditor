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
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool checkRange(QString str, int max)
    {
        int value;
        if (str.startsWith("0x"))
            value = str.remove(0, 2).toInt(NULL, 16);
        else
            value = str.toInt();

        return (value <= max);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    int extractNumber(QString str)
    {
        int value;
        if (str.startsWith("0x"))
            value = str.remove(0, 2).toInt(NULL, 16);
        else
            value = str.toInt();

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


        // Splits the code up to it's lines while removing empty lines
        QList<NotifyEntry> notifications;
        QStringList codeLines = code.split('\n');
        QString charPreproc, charPointer;
        QRegularExpression regex;
        QRegExp regHex("(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)");
        QStringList labels;
        QString macroRegex("\\b(X)\\b");

        bool isXSE;
        if ((isXSE = (CONFIG(Language) == BL_XSE)))
        {
            charPreproc.push_back('#');
            charPointer.push_back('@');
        }
        else
        {
            charPreproc.push_back('.');
        }


        // Iterates through every line and searches for errors
        for (int i = 0; i < codeLines.size(); i++)
        {
            // Determines whether an abort was requested
            mutex.lock();
            if (!running)
            {
                mutex.unlock();
                return;
            }
            mutex.unlock();


            // Removes all leading and trailing whitespace
            QString trimmedLine = codeLines[i].trimmed();


            if (trimmedLine.isEmpty() || trimmedLine.startsWith("//"))
                continue;

            if (trimmedLine.contains("//"))
            {
                trimmedLine.remove(trimmedLine.indexOf("//"), INT_MAX);
                trimmedLine = trimmedLine.trimmed();
            }
            if (trimmedLine.startsWith(charPreproc))
            {
                // Define, Include, Move, Item, Byte, HWord, Word, Org
                trimmedLine.remove(0, 1);
                if (trimmedLine.startsWith("define"))
                {
                    regex.setPattern("define\\s+\\w(\\w|\\d)+\\s+\\d+$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid definition
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_DEFINITION, arg0.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        int ws1 = trimmedLine.indexOf(' ')+1;
                        int ws2 = trimmedLine.lastIndexOf(' ');
                        macroRegex.insert(4, trimmedLine.mid(ws1, ws2-ws1) + QString("|"));
                    }
                }
                else if (trimmedLine.startsWith("include"))
                {
                    regex.setPattern("include\\s+(\"|<)\\w(\\w|\\d)+(\"|>)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid include expression
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_INCLUDE, arg0.toStdString().c_str());
                        continue;
                    }
                }
                else if (trimmedLine.startsWith("move"))
                {
                    regex.setPattern("move\\s+(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Could be invalid, but we have to assure that
                        // there isn't a defined macro value.
                        regex.setPattern(macroRegex);
                        if (!regex.match(trimmedLine).hasMatch())
                        {
                            // Invalid move identifier
                            QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                            rtError(RT_INVALID_MOVEMENT, arg0.toStdString().c_str());
                            continue;
                        }
                    }
                }
                else if (trimmedLine.startsWith("item"))
                {
                    regex.setPattern("item\\s+(\\d+)|(0[xX][0-9a-fA-F]+)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Could be invalid, but we have to assure that
                        // there isn't a defined macro value.
                        regex.setPattern(macroRegex);
                        if (!regex.match(trimmedLine).hasMatch())
                        {
                            // Invalid item identifier
                            QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                            rtError(RT_INVALID_ITEM, arg0.toStdString().c_str());
                            continue;
                        }
                    }
                }
                else if (trimmedLine.startsWith("byte"))
                {
                    regex.setPattern("byte\\s+(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid byte identifier
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_BYTE, arg0.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        // Is most likely correct, but can exceed 8-bit
                        int ws = trimmedLine.indexOf(' ')+1;
                        QString sub = trimmedLine.mid(ws);

                        // Determines whether value is outside range
                        if (!checkRange(sub, 0xFF))
                        {
                            // Overflow
                            int arg0 = extractNumber(sub);
                            rtError(RT_OVERFLOW_BYTE, arg0);
                            continue;
                        }
                    }
                }
                else if (trimmedLine.startsWith("hword"))
                {
                    regex.setPattern("hword\\s+(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid hword identifier
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_BYTE, arg0.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        // Is most likely correct, but can exceed 16-bit
                        int ws = trimmedLine.indexOf(' ')+1;
                        QString sub = trimmedLine.mid(ws);

                        // Determines whether value is outside range
                        if (!checkRange(sub, 0xFFFF))
                        {
                            // Overflow
                            int arg0 = extractNumber(sub);
                            rtError(RT_OVERFLOW_HWORD, arg0);
                            continue;
                        }
                    }
                }
                else if (trimmedLine.startsWith("word"))
                {
                    regex.setPattern("word\\s+(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid word identifier
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_BYTE, arg0.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        // Is most likely correct, but can exceed 32-bit
                        int ws = trimmedLine.indexOf(' ')+1;
                        QString sub = trimmedLine.mid(ws);

                        // Determines whether value is outside range
                        if (!checkRange(sub, 0xFFFFFFFF))
                        {
                            // Overflow
                            int arg0 = extractNumber(sub);
                            rtError(RT_OVERFLOW_WORD, arg0);
                            continue;
                        }
                    }
                }
                else if (trimmedLine.startsWith("org"))
                {
                    if (isXSE)
                        regex.setPattern("org\\s+@(\\w|\\d)+$");
                    else
                        regex.setPattern("org\\s+\\w(\\w|\\d)+:$");

                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid label
                        QString arg0 = trimmedLine.mid(trimmedLine.indexOf(' ')+1);
                        rtError(RT_INVALID_LABEL, arg0.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        int ws = trimmedLine.indexOf(' ')+1;
                        QString sub = trimmedLine.mid(ws);
                        if (sub.right(1) == ":")
                            sub.rightRef(1).clear();

                        labels.push_back(sub);
                    }
                }
                else if (!isXSE && trimmedLine.right(1) == ":")
                {
                    regex.setPattern("^\\w(\\w|\\d)+:$");
                    if (!regex.match(trimmedLine).hasMatch())
                    {
                        // Invalid ASE label
                        rtError(RT_INVALID_ASE_LABEL, trimmedLine.toStdString().c_str());
                        continue;
                    }
                }
                else
                {
                    // Unknown preprocessor directive
                    QString arg0 = trimmedLine.mid(0, trimmedLine.indexOf(' '));
                    rtError(RT_UNKNOWN_PREPROC, arg0.toStdString().c_str());
                    continue;
                }
            }
            else
            {
                // Some unknown identifier we don't know yet
                // Can be a command or a macro command

                // Extracts the command itself
                QString command;
                int ws = trimmedLine.indexOf(' ');
                if (ws == -1) // command without args?
                    command = trimmedLine;
                else
                    command = trimmedLine.left(ws);

                // Attempts to find that command
                const Command *cmd = Configuration::findByName(command);
                if (cmd == NULL)
                {
                    // Attempts to find it in the macroes
                    const Macro *mcr = Configuration::findMacroByName(command);
                    if (mcr == NULL)
                    {
                        // Unknown identifier
                        rtError(RT_UNKNOWN_IDENTIFIER, trimmedLine.toStdString().c_str());
                        continue;
                    }
                    else
                    {
                        // TODO: Check macro parameters
                    }
                }
                else
                {
                    // Check command parameters
                    QStringList params;
                    while (ws != -1)
                    {
                        int next = trimmedLine.indexOf(' ', ws);
                        params.push_back(trimmedLine.mid(ws+1, next-(ws+1)));
                        ws = next;
                    }
                    if (params.size() != cmd->params().size())
                    {
                        // Invalid argument count
                        int arg0 = params.size();
                        rtError(RT_INVALID_ARGCOUNT, arg0);
                        continue;
                    }
                    else
                    {
                        for (int i = 0; i < params.size(); i++)
                        {
                            ParameterType type = cmd->params().at(i)->type();
                            QString param = params.at(i);
                            if (param.left(1) == "@")
                                param.leftRef(1).clear();

                            if (type < PT_Pointer)
                            {
                                if (!regHex.exactMatch(param))
                                {
                                    // Is not a number!
                                    rtError(RT_INVALID_PARAMETER_N, i, param.toStdString().c_str());
                                    break;
                                }
                                if (checkRange(param, ParameterTypeHelper::GetMaxValue(type)))
                                {
                                    // Parameter out of range!
                                    int arg1 = ParameterTypeHelper::GetMaxValue(type);
                                    int arg2 = extractNumber(param);
                                    rtError(RT_INVALID_PARAMETER_O, i, arg1, arg2);
                                    break;
                                }
                            }
                            else
                            {
                                if (!regHex.exactMatch(param))
                                {
                                    // Determines whether the label is existing
                                    if (!labels.contains(param))
                                    {
                                        regex.setPattern(macroRegex);
                                        if (!regex.match(param).hasMatch())
                                        {
                                            // Not a macro, not a label (warning!)
                                            rtWarning(RT_INVALID_PARAMETER_I, param.toStdString().c_str());
                                            break;
                                        }
                                    }
                                }
                                if (checkRange(param, ParameterTypeHelper::GetMaxValue(type)))
                                {
                                    // Parameter out of range!
                                    int arg1 = ParameterTypeHelper::GetMaxValue(type);
                                    int arg2 = extractNumber(param);
                                    rtError(RT_INVALID_PARAMETER_O, i, arg1, arg2);
                                    break;
                                }
                            }
                        }
                    }
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

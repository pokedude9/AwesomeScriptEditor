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
#include <ASE/System/Configuration.hpp>
#include <QXmlStreamReader>
#include <QApplication>
#include <QDirIterator>
#include <QFile>


///////////////////////////////////////////////////////////
// Defines
//
///////////////////////////////////////////////////////////
#define CF_NO_SUCCESS   successParams = false
#define ME_NO_SUCCESS   successEntries = false


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Static variable definition
    //
    ///////////////////////////////////////////////////////////
    QList<QList<Command *>>     Configuration::m_Commands;
    QList<QList<Macro *>>       Configuration::m_Macroes;
    QStringList                 Configuration::m_FileNames;
    Int32                       Configuration::m_Count = 0;
    Int32                       Configuration::m_Active = 0;
    BaseROMType                 Configuration::RomType = RT_RS;
    BaseLanguage                Configuration::Language = BL_ASE;


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    inline bool readNumber(QXmlStreamReader *reader, Command *command)
    {
        bool result = false;
        auto string = reader->readElementText();
        command->setByte(static_cast<UInt8>(string.toUInt(&result, 16)));
        return result;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    inline bool readValue(QXmlStreamReader *reader, UInt32 *number)
    {
        bool result = false;
        auto string = reader->readElementText();
        *number = string.toUInt(&result, 16);
        return result;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    inline bool readType(QXmlStreamReader *reader, Parameter *param)
    {
        auto string = reader->readElementText();
        if (string == "byte")
            param->setType(PT_Byte);
        else if (string == "hword")
            param->setType(PT_HWord);
        else if (string == "word")
            param->setType(PT_Word);
        else if (string == "pointer")
            param->setType(PT_Pointer);
        else if (string == "itempointer")
            param->setType(PT_ItemPointer);
        else if (string == "textpointer")
            param->setType(PT_StringPointer);
        else if (string == "movement")
            param->setType(PT_MovePointer);
        else
            return false;

        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Configuration::read()
    {
        //
        // Retrieves useful values for opening the script files
        //
        QStringList cmdFiles;
        QString cfgFolder("/config/");
        QString fileRegex("Script.*.xml");
        QString appFolder = QApplication::applicationDirPath() + cfgFolder;
        QDirIterator iter(appFolder, QStringList(fileRegex), QDir::Files, QDirIterator::Subdirectories);

        //
        // Finds every commands-file within the settings directory
        //
        while (iter.hasNext())
        {
            iter.next();
            m_Count++;
            m_FileNames.append(iter.fileName());
            cmdFiles.append(iter.filePath());
        }


        //
        // Parses all found commands-files
        //
        for (int currentFile = 0; currentFile < cmdFiles.size(); currentFile++)
        {
            // Creates new list-entries for this file
            m_Commands.push_back(QList<Command *>());
            m_Macroes.push_back(QList<Macro *>());


            // Attempts to open the file.
            QFile xmlFile(cmdFiles.at(currentFile));
            if (!xmlFile.open(QIODevice::ReadOnly))
                continue; // skip file on failure

            // Loads this file into a XML reader and tries to
            // find the commands tag (should be the first sub-tag)
            QXmlStreamReader xmlReader(&xmlFile);
            if (!xmlReader.readNextStartElement() || xmlReader.name() != "document")
                continue; // the top tag is not document; abort
            if (!xmlReader.readNextStartElement() || xmlReader.name() != "commands")
                continue; // the next sub tag is not commands; abort

            //
            // Loops through all the commands and loads them.
            //
            while (!xmlReader.hasError())
            {
                // Attempts to open the next command tag
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "command")
                    break; // command tag missing


                // Reads the command information; byte, name and info
                Command *currentCommand = new Command;
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "byte")
                {
                    delete currentCommand;
                    break; // byte tag not found; abort
                }
                if (!readNumber(&xmlReader, currentCommand))
                {
                    delete currentCommand;
                    break; // byte tag invalid; abort
                }
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "name")
                {
                    delete currentCommand;
                    break; // name tag not found; abort
                }
                else
                {
                    currentCommand->setName(xmlReader.readElementText());
                }
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "info")
                {
                    delete currentCommand;
                    break; // info tag not found; abort
                }
                else
                {
                    currentCommand->setInfo(xmlReader.readElementText());
                }


                // Attempts to read all the parameters out of this command
                Boolean successParams = true;
                QList<Parameter *> commandParams;
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "params")
                {
                    // This command doesn't have any parameters!
                    m_Commands[currentFile].push_back(currentCommand);
                    continue;
                }
                while (xmlReader.readNextStartElement() && xmlReader.name() == "param")
                {
                    // Reads every parameter within the params tag
                    Parameter *param = new Parameter;
                    if (!xmlReader.readNextStartElement() || xmlReader.name() != "type")
                    {
                        CF_NO_SUCCESS;
                        delete param;
                        break; // type tag not found; abort
                    }
                    if (!readType(&xmlReader, param))
                    {
                        CF_NO_SUCCESS;
                        delete param;
                        break; // invalid type specifier; abort
                    }
                    if (!xmlReader.readNextStartElement() || xmlReader.name() != "name")
                    {
                        CF_NO_SUCCESS;
                        delete param;
                        break; // name tag not found; abort
                    }
                    else
                    {
                        param->setName(xmlReader.readElementText());
                    }
                    if (!xmlReader.readNextStartElement() || xmlReader.name() != "info")
                    {
                        CF_NO_SUCCESS;
                        delete param;
                        break; // info tag not found; abort
                    }
                    else
                    {
                        param->setInfo(xmlReader.readElementText());
                    }


                    // Adds the parameter and jumps to the next start element
                    commandParams.push_back(param);
                    xmlReader.skipCurrentElement();
                }

                if (successParams == true)
                {
                    // Command loaded successfully; add to list
                    if (!commandParams.isEmpty())
                        xmlReader.skipCurrentElement();
                    currentCommand->setParams(commandParams);
                    m_Commands[currentFile].push_back(currentCommand);
                }
                else
                {
                    // Cleans up dynamic resources
                    delete currentCommand;
                    foreach (Parameter *param, commandParams)
                        delete param;

                    break;
                }
            }


            // Tries to find the macroes tag
            //xmlReader.skipCurrentElement();
            if (!xmlReader.readNextStartElement() || xmlReader.name() != "macroes")
                continue;


            //
            // Loops through all the macroes and loads them.
            //
            while (!xmlReader.hasError())
            {
                // Attempts to open the next macro tag
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "macro")
                    break; // macro tag missing


                Macro *currentMacro = new Macro;
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "name")
                {
                    delete currentMacro;
                    break; // name tag missing; abort
                }
                else
                {
                    currentMacro->setName(xmlReader.readElementText());
                }
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "info")
                {
                    delete currentMacro;
                    break; // info tag missing; abort
                }
                else
                {
                    currentMacro->setInfo(xmlReader.readElementText());
                }
                if (!xmlReader.readNextStartElement() || xmlReader.name() != "format")
                {
                    delete currentMacro;
                    break; // format tag missing; abort
                }

                // Reads every macro entry from the current macro
                Boolean successEntries = true;
                QList<MacroEntry *> macroEntries;
                while (xmlReader.readNextStartElement() && xmlReader.name() == "entry")
                {
                    MacroEntry *entry = new MacroEntry;
                    if (!xmlReader.readNextStartElement() || xmlReader.name() != "command")
                    {
                        ME_NO_SUCCESS;
                        break;
                    }
                    else
                    {
                        entry->setCommand(xmlReader.readElementText());
                    }


                    // Reads every parameter within this entry
                    bool successSub = true;
                    while (xmlReader.readNextStartElement() && xmlReader.name() == "param")
                    {
                        // Reads the type and checks whether it is fixed or dynamic
                        if (!xmlReader.readNextStartElement() || xmlReader.name() != "type")
                        {
                            successSub = false;
                            break;
                        }

                        QString typeString = xmlReader.readElementText();
                        if (typeString == "fixed")
                        {
                            // Reads the only value tag
                            unsigned value = 0;
                            if (!xmlReader.readNextStartElement() || xmlReader.name() != "value" || !readValue(&xmlReader, &value))
                            {
                                successSub = false;
                                break;
                            }

                            MacroFixedParam *subParam = new MacroFixedParam;
                            subParam->setValue(value);
                            entry->addParam(subParam);
                            xmlReader.skipCurrentElement();
                        }
                        else if (typeString == "dynamic")
                        {
                            // Reads the index tag
                            unsigned index = 0;
                            if (!xmlReader.readNextStartElement() || xmlReader.name() != "index" || !readValue(&xmlReader, &index))
                            {
                                successSub = false;
                                break;
                            }

                            MacroDynamicParam *subParam = new MacroDynamicParam;
                            subParam->setIndex(static_cast<Int32>(index));
                            entry->addParam(subParam);

                            // Reads the rest of the arguments
                            if (!xmlReader.readNextStartElement() || xmlReader.name() != "name")
                            {
                                successSub = false;
                                break;
                            }
                            else
                            {
                                subParam->setName(xmlReader.readElementText());
                            }
                            if (!xmlReader.readNextStartElement() || xmlReader.name() != "info")
                            {
                                successSub = false;
                                break;
                            }
                            else
                            {
                                subParam->setInfo(xmlReader.readElementText());
                                xmlReader.skipCurrentElement();
                            }
                        }
                        else
                        {
                            successSub = false;
                            break;
                        }
                    }
                    if (successSub == false)
                    {
                        ME_NO_SUCCESS;
                        break;
                    }
                    else
                    {
                        macroEntries.push_back(entry);
                    }
                }
                if (successEntries == true)
                {
                    currentMacro->setEntries(macroEntries);
                    currentMacro->parse();
                    m_Macroes[currentFile].push_back(currentMacro);
                    xmlReader.skipCurrentElement();
                    xmlReader.skipCurrentElement();
                    xmlReader.skipCurrentElement();
                    xmlReader.skipCurrentElement();
                }
                else
                {
                    // Cleans up dynamic resources
                    delete currentMacro;
                    break;
                }
            }

            xmlFile.close();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Configuration::dispose()
    {
        foreach (const QList<Command *> &cmdList, m_Commands)
            foreach (Command *cmd, cmdList)
                delete cmd;

        foreach (const QList<Macro *> &macroList, m_Macroes)
            foreach (Macro *macro, macroList)
                delete macro;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Command *> &Configuration::getCommands(Int32 index)
    {
        return m_Commands.at(index);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Macro *> &Configuration::getMacroes(Int32 index)
    {
        return m_Macroes.at(index);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    Int32 Configuration::getAmount()
    {
        return m_Count;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QStringList &Configuration::getFileNames()
    {
        return m_FileNames;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Command *> &Configuration::activeCommands()
    {
        return m_Commands.at(m_Active);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Macro *> &Configuration::activeMacroes()
    {
        return m_Macroes.at(m_Active);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void Configuration::setActive(Int32 index)
    {
        m_Active = index;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const Command *Configuration::findByByte(UInt8 byte)
    {
        const QList<Command *> &current = activeCommands();
        foreach (const Command *cmd, current)
        {
            if (cmd->byte() == byte)
                return cmd;
        }

        // No command found, returns invalid pointer
        return NULL;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const Command *Configuration::findByName(const QString &name)
    {
        const QList<Command *> &current = activeCommands();
        foreach (const Command *cmd, current)
        {
            if (cmd->name() == name)
                return cmd;
        }

        // No command found, returns invalid pointer
        return NULL;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Macro *> Configuration::findMacroByCmd(const Command *cmd)
    {
        QList<Macro *> macroes;
        Int32 count = activeMacroes().size();

        // Loops through all the macroes and finds matching ones
        for (int i = 0; i < count; i++)
        {
            if (activeMacroes().at(i)->commands().at(0) == cmd)
                macroes.push_back(activeMacroes().at(i));
        }

        // Returns the possible macroes. List might have size zero.
        return macroes;
    }
}

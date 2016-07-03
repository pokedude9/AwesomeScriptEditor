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
#include <ASE/Forms/MainWindow.h>
#include <ASE/Scripting/Decompiler.hpp>
#include "ui_MainWindow.h"


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Action pointer objects
    //
    ///////////////////////////////////////////////////////////
    QAction *s_ActionDecompile;
    QAction *s_ActionCommand;
    QAction *s_ActionLabel;


    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_PrevTab(-1)
    {
        ui->setupUi(this);
        setupCommands();
        setupAdvanced();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MainWindow::~MainWindow()
    {
        Configuration::dispose();

        delete ui;
        if (m_Rom.info().isLoaded())
            m_Rom.close();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupAdvanced()
    {
        m_DecompileBox = new ASEHexBox;
        m_CommandBox = new QComboBox;
        m_FileLabel = new QLabel;

        m_DecompileBox->setMaximumWidth(100);
        m_DecompileBox->setMaximumHeight(22);
        m_CommandBox->setFixedWidth(150);
        m_CommandBox->setMaximumHeight(22);
        m_CommandBox->setFixedHeight(22);
        m_CommandBox->setObjectName("m_CommandBox");
        m_DecompileBox->setMaxLength(7);
        m_DecompileBox->setContentsMargins(4, 0, 8, 0);
        m_CommandBox->setEditable(true);
        m_CommandBox->addItems(Configuration::getFileNames());
        m_CommandBox->setItemDelegate(new QStyledItemDelegate);
        m_FileLabel->setText("Command file: ");
        m_FileLabel->setContentsMargins(4, 0, 0, 0);

        s_ActionDecompile = ui->mainToolBar->insertWidget(ui->actionDecompile, m_DecompileBox);
        s_ActionCommand = ui->mainToolBar->insertWidget(ui->actionOpen_2, m_CommandBox);
        s_ActionLabel = ui->mainToolBar->insertWidget(s_ActionCommand, m_FileLabel);
        ui->mainToolBar->insertSeparator(ui->actionOpen_2);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupCommands()
    {
        Configuration::read();
        auto commands = Configuration::getCommands(0);
        auto macroes = Configuration::getMacroes(0);
        const QStringList preproc =
        {
            "org",
            "define",
            "include",
            "item",
            "move",
            "byte",
            "hword",
            "word",
            "pointer"
        };

        QStringList functionNames;
        QStringList functionInfos;
        QStringList macroFunctions;
        QStringList macroInfos;

        foreach (Command *cmd, commands)
        {
            functionNames.push_back(cmd->name());
            functionInfos.push_back(cmd->info());
        }
        foreach (Macro *macro, macroes)
        {
            macroFunctions.push_back(macro->name());
            macroInfos.push_back(macro->info());
        }

        ui->plainTextEdit->setFunctions(functionNames);
        ui->plainTextEdit->setFunctionInfos(functionInfos);
        ui->plainTextEdit->setPreprocessor(preproc);
        ui->plainTextEdit->setMacroFunctions(macroFunctions);
        ui->plainTextEdit->setMacroFunctionInfos(macroInfos);
    }
}

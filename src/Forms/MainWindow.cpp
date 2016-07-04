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
#include <ASE/System/RTValidator.hpp>
#include <ASE/Forms/MainWindow.h>
#include <ASE/Widgets/Misc/Messages.hpp>
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
        delete m_Timer;
        delete m_Validator;

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
        m_Timer = new QTimer;
        m_Validator = new RTValidator(ui->plainTextEdit);

        m_DecompileBox->setMaximumWidth(100);
        m_DecompileBox->setMaximumHeight(22);
        m_CommandBox->setFixedWidth(150);
        m_CommandBox->setMaximumHeight(22);
        m_CommandBox->setFixedHeight(22);
        m_CommandBox->setObjectName("m_CommandBox");
        m_DecompileBox->setObjectName("m_DecompileBox");
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

        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->errorToolBar->insertWidget(ui->actionCloseDebugger, spacer);

        ui->mainToolBar->insertSeparator(ui->actionOpen_2);
        ui->splitter->setSizes({ 600-124, 124 });
        ui->tableWidget->setColumnWidth(0, 24);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

        m_Timer->setInterval(1000);
        ui->plainTextEdit->installEventFilter(this);
        connect(m_Validator->worker(), SIGNAL(finished(QList<NotifyEntry>)), this, SLOT(doneValidating(QList<NotifyEntry>)));
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(executeValidating()));
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

    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::resetUi()
    {
        ui->actionSave_ROM->setEnabled(false);
        ui->actionFind->setEnabled(false);
        ui->actionDecompile->setEnabled(false);
        ui->actionDecompile_Script->setEnabled(false);
        ui->actionDecompile_Levelscript->setEnabled(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::setupAfterRom()
    {
        ui->actionSave_ROM->setEnabled(true);
        ui->actionFind->setEnabled(true);
        ui->actionDecompile->setEnabled(true);
        ui->actionDecompile_Script->setEnabled(true);
        ui->actionDecompile_Levelscript->setEnabled(true);

        if (m_Rom.info().isExpanded())
            m_DecompileBox->setMaxLength(7);
        else
            m_DecompileBox->setMaxLength(6);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::saveScript(int tabIndex)
    {
        QWidget *currentTab = ui->tabWidget->widget(tabIndex);
        QString currentScr = currentTab->property("file").toString();
        QFileInfo fileInfo(currentScr);


        // Must save _as_ if file is not existing
        if (currentScr.isEmpty() || !fileInfo.exists())
        {
            saveScriptAs(tabIndex);
        }
        else
        {
            QFile file(currentScr);
            if (!file.open(QIODevice::WriteOnly))
            {
                QString error("Error while writing to file: \"{0}\".");
                Messages::showError(this, error.replace("{0}", currentScr));
                return;
            }


            // Retrieves the file's contents
            QByteArray stringToWrite;
            if (tabIndex != ui->tabWidget->currentIndex())
                stringToWrite = ui->tabWidget->widget(tabIndex)->property("data").toString().toUtf8();
            else
                stringToWrite = ui->plainTextEdit->toPlainText().toUtf8();


            // Removes the asterisk that indicates an unsaved file
            QString tabText = ui->tabWidget->tabText(tabIndex);
            if (tabText.right(1) == "*")
                tabText.remove(tabText.length()-1, 1);


            // Writes the new contents
            ui->tabWidget->setTabText(tabIndex, tabText);
            ui->tabWidget->currentWidget()->setProperty("save", QVariant(false));
            ui->tabWidget->widget(tabIndex)->setProperty("prev", QString(stringToWrite));
            ui->tabWidget->widget(tabIndex)->setProperty("data", QString(stringToWrite));
            file.write(stringToWrite);
            file.close();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Internal
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::saveScriptAs(int tabIndex)
    {
        // Shows a dialog to retrieve the file path
        QString path = QFileDialog::getSaveFileName
        (
            this,
            tr("Save script as"),
            QDir::homePath(),
            "Scripts (*.asc)"
        );


        // Saves the file if valid location picked
        if (!path.isNull() && !path.isEmpty())
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly))
            {
                QString error("Error while writing to file: \"{0}\".");
                Messages::showError(this, error.replace("{0}", path));
                return;
            }


            // Retrieves the file's contents
            QFileInfo info(path);
            QByteArray stringToWrite;
            if (tabIndex != ui->tabWidget->currentIndex())
                stringToWrite = ui->tabWidget->widget(tabIndex)->property("data").toString().toUtf8();
            else
                stringToWrite = ui->plainTextEdit->toPlainText().toUtf8();


            // Writes the new contents
            ui->tabWidget->setTabText(tabIndex, info.fileName());
            ui->tabWidget->currentWidget()->setProperty("save", QVariant(false));
            ui->tabWidget->widget(tabIndex)->setProperty("prev", QString(stringToWrite));
            ui->tabWidget->widget(tabIndex)->setProperty("data", QString(stringToWrite));
            file.write(stringToWrite);
            file.close();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::showEvent(QShowEvent *event)
    {
        // Shows the window before displaying the msgbox
        QMainWindow::showEvent(event);


        // Determines whether at least one command and command file exist
        if (Configuration::getAmount() == 0 || Configuration::getCommands(0).isEmpty())
        {
            Messages::showMessage
            (
                this,
                "A valid XML file containing the scripting commands could not be found.\n"
                 "The file must be named \"Script.<name>.xml\" whereas \"<name>\" can be\n"
                 "any name you like, and it must be stored in the settings folder. It\n"
                 "furthermore has to contain at least one valid command."
            );


            close();
            qApp->exit();
        }
        else
        {
            Configuration::setActive(0);
            m_CommandBox->setCurrentIndex(0);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool MainWindow::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched->objectName() == "plainTextEdit")
        {
            if (event->type() == QEvent::FocusIn)
            {
                // Editor in focus, start RT validator
                m_Timer->start();
            }
            else if (event->type() == QEvent::FocusOut)
            {
                m_Timer->stop();
                m_Validator->stop();
            }
            else if (event->type() == QEvent::KeyPress)
            {
                // Do not debug while typing
                m_Timer->stop();
            }
            else if (event->type() == QEvent::KeyRelease)
            {
                // Debug after stopping to type
                // Should immediately update when deleting something
                if (((QKeyEvent *)event)->key() == Qt::Key_Backspace)
                    executeValidating();

                m_Timer->start();
            }
        }


        return QMainWindow::eventFilter(watched, event);
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::executeValidating()
    {
        m_Validator->start();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::doneValidating(const QList<NotifyEntry> e)
    {
        ui->plainTextEdit->setErrors(e);

        // Clears the old table except for header
        ui->tableWidget->setUpdatesEnabled(false);
        for (int i = ui->tableWidget->rowCount(); i > 0; i--)
            ui->tableWidget->removeRow(i);

        // Sets up the table
        for (int i = 0; i < e.size(); i++)
        {
            QTableWidgetItem type, line, msg;


            ui->tableWidget->insertRow(i+1);
            ui->tableWidget->setItem(i+1, 0, NULL);
            ui->tableWidget->setItem(i+1, 1, NULL);
            ui->tableWidget->setItem(i+1, 2, NULL);
        }

        ui->tableWidget->setUpdatesEnabled(true);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_tabWidget_currentChanged(int index)
    {
        // If there are no tabs at the moment, disable scripting
        if (ui->tabWidget->count() == 0)
        {
            ui->plainTextEdit->setEnabled(false);
            ui->tabWidget->setEnabled(false);
            ui->tabWidget->setVisible(false);
            ui->actionSave->setEnabled(false);
            ui->actionSave_As->setEnabled(false);
            ui->actionSave_Script->setEnabled(false);
        }
        else
        {
            ui->plainTextEdit->setEnabled(true);
            ui->tabWidget->setEnabled(true);
            ui->tabWidget->setVisible(true);
            ui->actionSave->setEnabled(true);
            ui->actionSave_As->setEnabled(true);
            ui->actionSave_Script->setEnabled(true);
        }


        // Abort loading script file if no tab selected
        if (index == -1)
        {
            ui->plainTextEdit->setEnabled(false);
            ui->plainTextEdit->clear();
            m_PrevTab = index;
            return;
        }


        // Saves the currently displayed data for this tab
        if (m_PrevTab >= 0 && ui->tabWidget->widget(m_PrevTab) != NULL)
            ui->tabWidget->widget(m_PrevTab)->setProperty("data", QVariant(ui->plainTextEdit->toPlainText()));


        // Displays the new data
        QByteArray data = ui->tabWidget->widget(index)->property("data").toByteArray();
        ui->plainTextEdit->setPlainText(QString(data));
        m_PrevTab = index;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_tabWidget_tabCloseRequested(int index)
    {
        if (ui->tabWidget->widget(index)->property("save").toBool())
        {
            QString question("The file \"{0}\" has unsaved changes. Want to save the file now?");
            QString fileName(ui->tabWidget->tabText(index));
            fileName.replace('*', "");
            question.replace("{0}", fileName);


            // Offers to save the modified data before closing the tab
            if (Messages::showQuestion(this, question))
                saveScript(index);
        }


        // Removes the tab
        ui->tabWidget->removeTab(index);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_plainTextEdit_textChanged()
    {
        if (!ui->plainTextEdit->isVisible() || !ui->plainTextEdit->isEnabled())
            return;


        // Determines whether text can be undone/redone
        ui->actionUndo->setEnabled(ui->plainTextEdit->document()->isUndoAvailable());
        ui->actionRedo->setEnabled(ui->plainTextEdit->document()->isRedoAvailable());
        ui->actionPaste->setEnabled(ui->plainTextEdit->canPaste());


        // Loads the previous and the current data
        QString dataOld = ui->tabWidget->currentWidget()->property("prev").toString();
        QString dataNew = ui->plainTextEdit->toPlainText();

        // Determines whether the data was changed
        if (dataOld != dataNew)
        {
            QString text = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
            if (text.right(1) != "*")
                text.append('*');

            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
            ui->tabWidget->currentWidget()->setProperty("save", QVariant(true));
        }
        else
        {
            QString text = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
            if (text.right(1) == "*")
                text.remove(text.length() - 1, 1);

            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
            ui->tabWidget->currentWidget()->setProperty("save", QVariant(false));
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionNew_Script_triggered()
    {
        // Prepares a new tab
        QWidget *tab = new QWidget;
        QString text = QString("NewScript") + QString::number(ui->tabWidget->count()+1);


        // Sets some properties for the new tab
        tab->setProperty("data", QVariant(QString("")));
        tab->setProperty("prev", QVariant(QString("")));
        tab->setProperty("save", QVariant(false));


        // Adds this tab and selects it
        ui->tabWidget->addTab(tab, text);
        ui->tabWidget->setCurrentWidget(tab);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionSave_Script_triggered()
    {
        saveScript(ui->tabWidget->currentIndex());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionSave_As_triggered()
    {
        saveScriptAs(ui->tabWidget->currentIndex());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionUndo_triggered()
    {
        ui->plainTextEdit->undo();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionRedo_triggered()
    {
        ui->plainTextEdit->redo();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionCut_triggered()
    {
        ui->plainTextEdit->cut();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionCopy_triggered()
    {
        ui->plainTextEdit->copy();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionPaste_triggered()
    {
        ui->plainTextEdit->paste();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionDelete_triggered()
    {
        QTextCursor cursor = ui->plainTextEdit->textCursor();
        cursor.removeSelectedText();
        ui->plainTextEdit->setTextCursor(cursor);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionSelect_All_triggered()
    {
        ui->plainTextEdit->selectAll();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionSave_ROM_triggered()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionReal_time_script_validator_triggered()
    {
        if (ui->actionReal_time_script_validator->isChecked())
            ui->frame->setVisible(true);
        else
            ui->frame->setVisible(false);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Slot
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void MainWindow::on_actionCloseDebugger_triggered()
    {
        ui->actionReal_time_script_validator->setChecked(false);
    }
}

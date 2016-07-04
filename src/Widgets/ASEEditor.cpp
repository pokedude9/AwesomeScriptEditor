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
#include <QBoy/Config.hpp>
#include <ASE/Widgets/ASEEditor.hpp>
#include <ASE/Widgets/ASEEditorPopup.hpp>
#include <ASE/Widgets/ASEEditorLineWidget.hpp>
#include <ASE/Widgets/ASEEditorHighlighter.hpp>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditor::ASEEditor(QWidget *parent)
        : QPlainTextEdit(parent),
          m_Popup(NULL),
          m_Highlighter(NULL),
          m_LineWidget(NULL),
          m_FilterModel(NULL),
          m_AutoComplete(NULL),
          m_ClrLineBkgr(0xE9E9E9),
          m_ClrLineFrgr(0xB9B9B9),
          m_ClrLineText(0x6A9FB5)
    {
        // Changes the font for this widget
        QFont font("Consolas");
        font.setPointSize(10);
        font.setKerning(true);
        font.setStyleStrategy(QFont::PreferAntialias);
        setFont(font);

        // Changes properties of this widget
        QPalette palette;
        palette.setColor(QPalette::Base, QColor(0xFFFFFF));
        palette.setColor(QPalette::Text, QColor(0x505050));
        setFrameStyle(QFrame::NoFrame);
        setAutoFillBackground(true);
        setPalette(palette);

        // Constructs the highlighter and popup and applies them
        m_Highlighter = new ASEEditorHighlighter(document());
        m_Popup = new ASEEditorPopup;

        // Constructs the auto-complete box and applies it
        m_AutoComplete = new QCompleter;
        m_AutoComplete->setWidget(this);
        m_AutoComplete->setCompletionMode(QCompleter::PopupCompletion);
        m_AutoComplete->setModelSorting(QCompleter::CaseSensitivelySortedModel);
        m_AutoComplete->setPopup(m_Popup);

        // Constructs the proxy model
        m_FilterModel = new QSortFilterProxyModel;
        m_FilterModel->setFilterCaseSensitivity(Qt::CaseSensitive);
        m_FilterModel->setDynamicSortFilter(false);

        // Constructs the line widget
        m_LineWidget = new ASEEditorLineWidget(this);
        updateLineWidgetWidth(1337);

        // Connects important signals with appropriate slots
        connect(m_AutoComplete, SIGNAL(activated(QString)), this, SLOT(editorInsertCompletion(QString)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineWidgetScroll(QRect,int)));
        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineWidgetWidth(int)));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditor::~ASEEditor()
    {
        delete m_Highlighter;
        delete m_LineWidget;
        delete m_FilterModel;
        delete m_AutoComplete;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setFunctions(const QStringList &functions)
    {
        QStandardItemModel *model = new QStandardItemModel;
        QIcon iconFunction(":/images/function.png");
        Int32 listSize = functions.size();

        // Constructs a model from the function keywords
        for (int i = 0; i < listSize; i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(functions.at(i));
            item->setIcon(iconFunction);
            model->appendRow(item);
        }

        // Changes the source model to the current one
        m_FilterModel->setSourceModel(model);
        m_AutoComplete->setModel(m_FilterModel);
        m_AutoComplete->connect(m_AutoComplete, SIGNAL(highlighted(QModelIndex)), m_Popup, SLOT(updateInfo(QModelIndex)));
        m_Functions = functions;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setFunctionInfos(const QStringList &functionInfos)
    {
        int listSize = functionInfos.size();
        for (int i = 0; i < listSize; i++)
        {
            m_FilterModel->setData(
                m_FilterModel->index(i, 0),
                QVariant(functionInfos.at(i)),
                Qt::UserRole
            );
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setMacroFunctions(const QStringList &functions)
    {
        QIcon iconFunction(":/images/macroFunction.png");
        Int32 listSize = functions.size();

        // Appends the macro functions to the current model
        for (int i = 0; i < listSize; i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(functions.at(i));
            item->setIcon(iconFunction);
            (static_cast<QStandardItemModel*>(m_FilterModel->sourceModel())->appendRow(item));
        }

        m_MacroFunctions = functions;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setMacroFunctionInfos(const QStringList &functionInfos)
    {
        int startIndex = m_Functions.size();
        int listSize = functionInfos.size();
        for (int i = 0; i < listSize; i++)
        {
            m_FilterModel->setData(
                m_FilterModel->index(i+startIndex, 0),
                QVariant(functionInfos.at(i)),
                Qt::UserRole
            );
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setMacroes(const QStringList &macroes)
    {
        QIcon iconMacro(":/images/macro.png");
        Int32 listSize = macroes.size();

        // Appends the macro definitions to the current model
        for (int i = 0; i < listSize; i++)
        {
            QStandardItem *item = new QStandardItem;
            item->setText(macroes.at(i));
            item->setIcon(iconMacro);
            (static_cast<QStandardItemModel*>(m_FilterModel->sourceModel())->appendRow(item));
        }

        m_Macroes = macroes;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setErrors(const QList<NotifyEntry> &e)
    {
        m_Errors = e;
        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditor::setPreprocessor(const QStringList &preprocessor)
    {
        m_Highlighter->setPreprocessor(preprocessor);
        m_Preprocessor = preprocessor;
    }
}

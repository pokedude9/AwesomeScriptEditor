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


#ifndef __ASE_EDITOR_HPP__
#define __ASE_EDITOR_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtGui/QtGui>
#include <QtWidgets>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Forward declarations
    //
    ///////////////////////////////////////////////////////////
    class ASEEditorHighlighter;
    class ASEEditorLineWidget;
    class ASEEditorPopup;


    ///////////////////////////////////////////////////////////
    /// \file    AseEditor.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Implements syntax highlighting and more.
    ///
    /// The following features will be implemented:
    /// - Line numbering to track script size
    /// - Syntax highlighting for easier reading
    /// - Auto completion and suggestions
    /// - Real-time debugger to check possible mistakes
    ///
    ///////////////////////////////////////////////////////////
    class ASEEditor : public QPlainTextEdit {
    Q_OBJECT
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEEditor.
        /// Sets various plugins and additions up.
        ///
        ///////////////////////////////////////////////////////////
        ASEEditor(QWidget *parent = NULL);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources that were used by the plugins
        /// and additions that were used by ase::ASEEditor.
        ///
        ///////////////////////////////////////////////////////////
        ~ASEEditor();


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the width of the line widget.
        ///
        /// The line widgets draws every line-number. In order to
        /// display contents correctly, it's width is needed.
        ///
        /// \returns the width of the line widget, in pixels.
        ///
        ///////////////////////////////////////////////////////////
        int widgetLineWidth() const;


        ///////////////////////////////////////////////////////////
        /// \brief Sets the functions for this pory::ASEEditor.
        ///
        /// The function names are loaded dynamically by the
        /// configuration XML parser and will be shown in an
        /// auto-complete box for usage.
        ///
        /// \param functions List of function names
        ///
        ///////////////////////////////////////////////////////////
        void setFunctions(const QStringList &functions);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the function descriptions.
        ///
        /// The function descriptions are loaded dynamically by the
        /// configuration XML parser and will be shown when the
        /// specified function is selected in the auto-complete box.
        ///
        /// \param functionInfos List of function descriptions
        ///
        ///////////////////////////////////////////////////////////
        void setFunctionInfos(const QStringList &functionInfos);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the macro functions for this ase::ASEEditor.
        ///
        /// The macro function names are loaded dynamically by the
        /// configuration XML parser and will be shown in an
        /// auto-complete box for usage.
        ///
        /// \param macroFunctions List of macro function names
        ///
        ///////////////////////////////////////////////////////////
        void setMacroFunctions(const QStringList &functions);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the macro function descriptions.
        ///
        /// The macro function descriptions are loaded dynamically
        /// by the configuration XML parser and will be shown in an
        /// auto-complete box for usage.
        ///
        /// \param functionInfos List of function descriptions
        ///
        ///////////////////////////////////////////////////////////
        void setMacroFunctionInfos(const QStringList &functionInfos);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the preprocessors for this ase::ASEEditor.
        ///
        /// The preprocessor directives are fixed for all
        /// configuration files and will be highlighted by
        /// the underlying QSyntaxHighlighter.
        ///
        /// \param preprocessors List of preprocessor directives
        ///
        ///////////////////////////////////////////////////////////
        void setPreprocessor(const QStringList &preprocessor);

        ///////////////////////////////////////////////////////////
        /// \brief Adds some macroes for this ase::ASEEditor.
        ///
        /// The macroes may differ between every script-file,
        /// but there are predefined macroes for items or moves
        /// that are loaded by the configuration parser.
        ///
        /// \params macroes List of macroes
        ///
        ///////////////////////////////////////////////////////////
        void setMacroes(const QStringList &macroes);


        ///////////////////////////////////////////////////////////
        /// \brief Paints the line widget's contents.
        ///
        /// Painting can not be performed in the line widget
        /// because no access to protected functions is granted.
        ///
        /// \param event Event from the ASEEditorLineWidget
        ///
        ///////////////////////////////////////////////////////////
        void paintEventLineWidget(QPaintEvent *event);


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Paints the line widget and it's numbers.
        ///
        /// This method will override the painting of the default
        /// QPlainTextEdit in order to paint the children widgets
        /// but performs painting of QPlainTextEdit afterwards.
        ///
        /// \param event Structure containing the visible rect
        ///
        ///////////////////////////////////////////////////////////
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Intercepts the key-press event.
        ///
        /// It is necessary to catch this event in order to
        /// trigger the intellisense box in case the latestly
        /// written word matches one of the functions or macroes.
        ///
        /// \param event Structure containing key and text
        ///
        ///////////////////////////////////////////////////////////
        void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

        ///////////////////////////////////////////////////////////
        /// \brief Provides custom resizing logic.
        ///
        /// If the height of the pory::ASEEditor changes, the
        /// line widget's height must also be adjusted.
        ///
        /// \param event Structure containing the new height
        ///
        ///////////////////////////////////////////////////////////
        void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


    private slots:

        ///////////////////////////////////////////////////////////
        /// \brief Updates the width of the line widget.
        ///
        /// In order to calculate the width of the line widget,
        /// the amount of line-numbers must be known in order to
        /// fit all the digits of the highest line-number.
        ///
        /// \param lineCount Total lines in the ase::ASEEditor
        ///
        ///////////////////////////////////////////////////////////
        void updateLineWidgetWidth(int lineCount);

        ///////////////////////////////////////////////////////////
        /// \brief Updates the scroll of the line widget.
        ///
        /// The first visible line-number must be updated in
        /// case the parental ase::ASEEditor was scrolled.
        ///
        /// \param r Visible rectangle of the ase::ASEEditor
        /// \param y The vertical scroll value
        ///
        ///////////////////////////////////////////////////////////
        void updateLineWidgetScroll(QRect r, int y);

        ///////////////////////////////////////////////////////////
        /// \brief Auto-completes the word, if requested.
        ///
        /// This action is requested by the underlying
        /// auto-complete box after an item was clicked.
        ///
        /// \param The full function or macro
        ///
        ///////////////////////////////////////////////////////////
        void editorInsertCompletion(const QString &full);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        ASEEditorPopup             *m_Popup;
        ASEEditorHighlighter       *m_Highlighter;
        ASEEditorLineWidget        *m_LineWidget;
        QSortFilterProxyModel      *m_FilterModel;
        QCompleter                 *m_AutoComplete;
        QStringList                 m_Functions;
        QStringList                 m_Macroes;
        QStringList                 m_Preprocessor;
        QStringList                 m_MacroFunctions;
        QColor                      m_ClrLineBkgr;
        QColor                      m_ClrLineFrgr;
        QColor                      m_ClrLineText;
    };
}


#endif //__ASE_EDITOR_HPP__

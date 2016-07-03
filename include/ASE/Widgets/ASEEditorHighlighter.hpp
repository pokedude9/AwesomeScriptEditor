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


#ifndef __ASE_EDITORHIGHLIGHTER_HPP__
#define __ASE_EDITORHIGHLIGHTER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QSyntaxHighlighter>
#include <QTextDocument>


namespace ase
{
    ///////////////////////////////////////////////////////////
    /// \file    ASEEditorHighlighter.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    7/3/2016
    /// \brief   Highlights functions, macroes and other.
    ///
    /// Additionally, highlights numbers, pointers and
    /// strings that are braille or Pokémon strings.
    ///
    ///////////////////////////////////////////////////////////
    class ASEEditorHighlighter : public QSyntaxHighlighter {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ase::ASEEditorHighlighter.
        /// Sets the specified QTextDocument as it's parent.
        ///
        ///////////////////////////////////////////////////////////
        ASEEditorHighlighter(QTextDocument *parent);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Releases all resources used by ase::ASEEditorHighligther.
        ///
        ///////////////////////////////////////////////////////////
        ~ASEEditorHighlighter();


        ///////////////////////////////////////////////////////////
        /// \brief Sets the preprocessors for this ase::ASEEEditor.
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
        /// \brief Determines the current script language.
        ///
        /// XSE and ASE differ concerning many coding guidelines,
        /// thus they evaluate to different highlighting code.
        ///
        /// \param useXse True if using XSE language
        ///
        ///////////////////////////////////////////////////////////
        void setLanguage(bool useXse);


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Highlights symbols in the current line.
        ///
        /// Highlights numbers, pointers and strings. Additionally
        /// highlights possible functions, macroes and directives.
        ///
        ///////////////////////////////////////////////////////////
        void highlightBlock(const QString &text) Q_DECL_OVERRIDE;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QStringList     m_Macroes;      ///< Contains all the macroes
        QStringList     m_Preprocessor; ///< Contains the preprocessor
        QColor          m_ClrMacro;     ///< Color of the macro strings
        QColor          m_ClrNumber;    ///< Color of the numbers
        QColor          m_ClrComment;   ///< Color of the comments
        QColor          m_ClrDirective; ///< Color of the directives
        QColor          m_ClrPointer;   ///< Color of the pointers
        QColor          m_ClrPokeText;  ///< Color of the strings
        bool            m_UseXSE;       ///< Use XSE langauge?
    };
}


#endif //__ASE_EDITORHIGHLIGHTER_HPP__

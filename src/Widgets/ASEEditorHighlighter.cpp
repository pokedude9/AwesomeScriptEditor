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
#include <ASE/Widgets/ASEEditorHighlighter.hpp>
#include <QRegularExpression>


namespace ase
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditorHighlighter::ASEEditorHighlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent),
          m_ClrMacro(0x8959A8),
          m_ClrNumber(0xC82829),
          m_ClrComment(0x8E908C),
          m_ClrDirective(0x718C00),
          m_ClrPointer(0x4271AE),
          m_ClrPokeText(0xF5871F),
          m_UseXSE(false)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    ASEEditorHighlighter::~ASEEditorHighlighter()
    {
        m_Macroes.clear();
        m_Preprocessor.clear();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditorHighlighter::setMacroes(const QStringList &macroes)
    {
        m_Macroes = macroes;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditorHighlighter::setPreprocessor(const QStringList &preprocessor)
    {
        m_Preprocessor = preprocessor;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditorHighlighter::setLanguage(bool useXse)
    {
        m_UseXSE = useXse;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    void ASEEditorHighlighter::highlightBlock(const QString &text)
    {
        // Highlights decimal and hexadecimal numbers through regex
        QRegularExpressionMatch match;
        QRegularExpression regex("(\\b\\d+\\b)|(\\b0[xX][0-9a-fA-F]+\\b)");
        QRegularExpressionMatchIterator iter = regex.globalMatch(text);
        while (iter.hasNext())
        {
            // Proceeds the next match and highlights it
            match = iter.next();
            setFormat(match.capturedStart(), match.capturedLength(), m_ClrNumber);
        }


        // Highlights single-line commands through regex
        regex.setPattern("(\\/\\/)+.*");
        match = regex.match(text);
        if (match.hasMatch())
        {
            // This regex can only have one match!
            setFormat(match.capturedStart(), match.capturedLength(), m_ClrComment);
        }


        // Highlights dynamic pointers that point to sub-sections in the script
        if (m_UseXSE)
        {
            regex.setPattern("(^|\\s)@\\w+(\\s|$)");
            iter = regex.globalMatch(text);
            while (iter.hasNext())
            {
                // Proceeds the next match and highlights it
                match = iter.next();
                setFormat(match.capturedStart(), match.capturedLength(), m_ClrPointer);
            }
        }
        else
        {
            regex.setPattern("(^|\\s)\\w+:");
            iter = regex.globalMatch(text);
            while (iter.hasNext())
            {
                // Proceeds the next match and highlights it
                match = iter.next();
                setFormat(match.capturedStart(), match.capturedLength(), m_ClrPointer);
            }
        }


        // Hightlights macro definitions
        QString pattern("(^|\\s)");
        if (m_UseXSE)
            pattern.append("#");
        else
            pattern.append(".");
        pattern.append("define");
        pattern.append("\\s+\\w+\\s+");
        regex.setPattern(pattern);
        match = regex.match(text);
        if (match.hasMatch())
        {
            // This regex can only have one match!
            setFormat(match.capturedStart(), match.capturedLength(), m_ClrPointer);
        }


        // Highlights normal strings through regex
        regex.setPattern("(\"|<)\\D+(\"|>)");
        match = regex.match(text);
        if (match.hasMatch())
        {
            // This regex can only have one match!
            setFormat(match.capturedStart(), match.capturedLength(), m_ClrPokeText);
        }


        // Highlights Pokémon text through regex
        regex.setPattern("^=\\s.*$");
        match = regex.match(text);
        if (match.hasMatch())
        {
            // This regex can only have one match!
            setFormat(match.capturedStart(), match.capturedLength(), m_ClrPokeText);
        }


        // Highlights the only preprocessor (if existing) in this line
        if (!m_Preprocessor.isEmpty())
        {
            if (m_UseXSE)
                pattern = (QString("^#(") + m_Preprocessor.at(0));
            else
                pattern = (QString("^\\.(") + m_Preprocessor.at(0));

            foreach (QString directive, m_Preprocessor)
                pattern.append(QString("|") + directive);

            pattern.append(QString(")\\b"));
            regex.setPattern(pattern);
            match = regex.match(text);
            if (match.hasMatch())
            {
                // This regex can only have one match!
                setFormat(match.capturedStart(), match.capturedLength(), m_ClrDirective);
            }
        }


        // Highlights possible macro directives in this line
        if (!m_Macroes.isEmpty())
        {
            QString pattern = QString("^#(") + m_Macroes.at(0);
            foreach (QString macro, m_Macroes)
                pattern.append(QString("|") + macro);

            pattern.append(QString(")\\b"));
            regex.setPattern(pattern);
            iter = regex.globalMatch(text);
            while (iter.hasNext())
            {
                // Proceeds the next match and highlights it
                match = iter.next();
                setFormat(match.capturedStart(), match.capturedLength(), m_ClrMacro);
            }
        }
    }
}

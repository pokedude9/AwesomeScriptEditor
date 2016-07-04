#
# AwesomeScriptEditor: A script editor for GBA Pokémon games.
# Copyright (C) 2016 Pokedude, Diegoisawesome
# License: General Public License 3.0
#


#
# QMake Settings, 1
#
QT         += xml core gui widgets
TARGET      = AwesomeScriptEditor
TEMPLATE    = app
CONFIG     += c++11
DEFINES    += ASE_EDITOR


#
# QMake Settings, 2
#
VERSION = 1.0
win:QMAKE_TARGET_COMPANY     = DoMoreAwesome
win:QMAKE_TARGET_PRODUCT     = AwesomeScriptEditor
win:QMAKE_TARGET_DESCRIPTION = A script editor for GBA Pokémon games.
win:QMAKE_TARGET_COPYRIGHT   = (C) 2016 Pokedude, Diegoisawesome


#
# QMake Settings, 3
#
INCLUDEPATH += include
INCLUDEPATH += include/ASE/Forms
INCLUDEPATH += include/ASE/Widgets
INCLUDEPATH += $$PWD/../QBoy/include
DEPENDPATH  += $$PWD/../QBoy/include
QMAKE_LFLAGS += -static-libgcc -static-libstdc++


#
# QMake Settings, 4
#
unix|win32: LIBS += -L$$PWD/../QBoy/bin/debug/ -lQBoy


#
# Source Files
#
SOURCES += \
    src/Forms/MainWindow.cpp \
    src/main.cpp \
    src/Widgets/Misc/Messages.cpp \
    src/Widgets/Misc/StyleSheets.cpp \
    src/Widgets/ASEEditorHighlighter.cpp \
    src/Widgets/ASEEditorLineWidget.cpp \
    src/Widgets/ASEEditor.cpp \
    src/Widgets/ASEEditorEvents.cpp \
    src/Widgets/ASEEditorPopupLabel.cpp \
    src/Widgets/ASEEditorPopup.cpp \
    src/Widgets/ASEEditorSlots.cpp \
    src/Widgets/ASEHexBox.cpp \
    src/System/Configuration.cpp \
    src/Parser/Parameter.cpp \
    src/Parser/Command.cpp \
    src/Parser/MacroParam.cpp \
    src/Parser/MacroFixedParam.cpp \
    src/Parser/MacroDynamicParam.cpp \
    src/Parser/MacroEntry.cpp \
    src/Parser/Macro.cpp \
    src/Scripting/ScriptBlock.cpp \
    src/Scripting/Blocks/FunctionBlock.cpp \
    src/Scripting/Blocks/ItemBlock.cpp \
    src/Scripting/Blocks/MoveBlock.cpp \
    src/Scripting/Blocks/StringBlock.cpp \
    src/Text/String.cpp \
    src/Text/Tables.cpp \
    src/Scripting/ASEScript.cpp \
    src/Scripting/Decompiler.cpp \
    src/System/RTValidator.cpp \
    src/System/RTWorker.cpp


#
# Header Files
#
HEADERS  += \
    include/ASE/Forms/MainWindow.h \
    include/ASE/Widgets/Misc/Messages.hpp \
    include/ASE/Widgets/Misc/StyleSheets.hpp \
    include/ASE/Widgets/ASEEditorHighlighter.hpp \
    include/ASE/Widgets/ASEEditorLineWidget.hpp \
    include/ASE/Widgets/ASEEditor.hpp \
    include/ASE/Widgets/ASEEditorPopupLabel.hpp \
    include/ASE/Widgets/ASEEditorPopup.hpp \
    include/ASE/Widgets/ASEHexBox.hpp \
    include/ASE/System/Configuration.hpp \
    include/ASE/Parser/ParameterType.hpp \
    include/ASE/Parser/Parameter.hpp \
    include/ASE/Parser/Command.hpp \
    include/ASE/Parser/MacroParam.hpp \
    include/ASE/Parser/MacroFixedParam.hpp \
    include/ASE/Parser/MacroDynamicParam.hpp \
    include/ASE/Parser/MacroEntry.hpp \
    include/ASE/Parser/Macro.hpp \
    include/ASE/Scripting/ScriptBlock.hpp \
    include/ASE/Scripting/Blocks/FunctionBlock.hpp \
    include/ASE/Scripting/Blocks/ItemBlock.hpp \
    include/ASE/Scripting/Blocks/MoveBlock.hpp \
    include/ASE/Scripting/Blocks/StringBlock.hpp \
    include/ASE/Text/String.hpp \
    include/ASE/Text/Tables.hpp \
    include/ASE/Scripting/ASEScript.hpp \
    include/ASE/Scripting/Decompiler.hpp \
    include/ASE/System/RTWorker.hpp \
    include/ASE/System/RTValidator.hpp \
    include/ASE/System/RTErrors.hpp


#
# Forms
#
FORMS    += \
    resources/UI/MainWindow.ui


#
# Resources
#
RESOURCES += \
    resources/ASE.qrc

DISTFILES += \
    resources/config/BPRE.xml \
    resources/config/Script.Default.xml \
    resources/config/AXVE.xml \
    resources/config/BPEE.xml

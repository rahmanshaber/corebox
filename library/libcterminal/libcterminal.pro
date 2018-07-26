QT      += widgets

TARGET   = cterminal
TEMPLATE = lib

DEFINES += CTERMINAL_LIBRARY

DEFINES += KB_LAYOUT_DIR=\\\"/usr/share/cterminal/kb-layouts\\\"
DEFINES += COLORSCHEMES_DIR=\\\"/usr/share/cterminal/color-schemes\\\"

DEFINES += BUNDLE_KEYBOARDLAYOUTS
DEFINES += BUNDLE_COLORSCHEMES

VERSION += 0.9.0

LIBS    += -lutil -lX11

# C++11 Support
# =============
QMAKE_CFLAGS   += -std=c99
QMAKE_CXXFLAGS += -std=c++11

DEFINES += "HAVE_POSIX_OPENPT"
DEFINES += "HAVE_SYS_TIME_H"

unix {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        INSTALLS	+= target includes data1 data2 data3 data4
        CONFIG		+= create_pc no_install_prl link_pkgconfig
        contains(DEFINES, LIB64): target.path = $$INSTALL_PREFIX/lib64
        else: target.path = $$INSTALL_PREFIX/lib

        target.path			= $$PREFIX/lib/
        includes.files  = qtermwidget.h Emulation.h Filter.h
        includes.path   = $$PREFIX/include/

        data1.path = $$PREFIX/share/$$TARGET/kb-layouts/
        data1.files = ./kb-layouts/*.keytab

        data2.path = $$PREFIX/share/$$TARGET/kb-layouts/historic
        data2.files = ./kb-layouts/historic/*.keytab

        data3.path = $$PREFIX/share/$$TARGET/color-schemes/
        data3.files = ./color-schemes/*.*schem*

        data4.path = $$PREFIX/share/$$TARGET/color-schemes/historic
        data4.files = ./color-schemes/historic/*.*schem*

        QMAKE_PKGCONFIG_NAME = libcterminal
        QMAKE_PKGCONFIG_DESCRIPTION = Terminal emulator in Qt 5
        QMAKE_PKGCONFIG_PREFIX  = $$INSTALL_PREFIX
        QMAKE_PKGCONFIG_LIBDIR  = $$target.path
        QMAKE_PKGCONFIG_INCDIR  = $$includes.path
        QMAKE_PKGCONFIG_VERSION = $$VERSION
        QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}

SOURCES += \
        qtermwidget.cpp \
        BlockArray.cpp \
        ColorScheme.cpp \
        Emulation.cpp \
        Filter.cpp \
        History.cpp \
        HistorySearch.cpp \
        KeyboardTranslator.cpp \
        konsole_wcwidth.cpp \
        kprocess.cpp \
        kpty.cpp \
        kptydevice.cpp \
        kptyprocess.cpp \
        Pty.cpp \
        Screen.cpp \
        ScreenWindow.cpp \
        SearchBar.cpp \
        Session.cpp \
        ShellCommand.cpp \
        TerminalCharacterDecoder.cpp \
        TerminalDisplay.cpp \
        tools.cpp \
        Vt102Emulation.cpp

HEADERS += \
        qtermwidget.h \
        qtermwidget_global.h \
        BlockArray.h \
        Character.h \
        CharacterColor.h \
        ColorScheme.h \
        ColorTables.h \
        DefaultTranslatorText.h \
        Emulation.h \
        ExtendedDefaultTranslator.h \
        Filter.h \
        History.h \
        HistorySearch.h \
        KeyboardTranslator.h \
        konsole_wcwidth.h \
        kprocess.h \
        kpty.h \
        kptydevice.h \
        kpty_p.h \
        kptyprocess.h \
        LineFont.h \
        Pty.h \
        Screen.h \
        ScreenWindow.h \
        SearchBar.h \
        Session.h \
        ShellCommand.h \
        TerminalCharacterDecoder.h \
        TerminalDisplay.h \
        tools.h \
        Vt102Emulation.h

FORMS += \
        SearchBar.ui

RESOURCES += ./kb-layouts/kb-layouts.qrc \
             ./color-schemes/color-schemes.qrc

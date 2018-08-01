TEMPLATE = app

QT += core gui dbus multimedia multimediawidgets charts concurrent x11extras #svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = coreBox

DEFINES += QT_DEPRECATED_WARNINGS

DEPENDPATH  += .

INCLUDEPATH += ../library/libcpdf/
LIBS += -L../library/libcpdf -lcpdf

INCLUDEPATH += ../library/libcterminal/
LIBS += -L../library/libcterminal -lcterminal -lX11

INCLUDEPATH += . ../library/libcarchiver/
LIBS += -L../library/libcarchiver/ -lcarchiver

INCLUDEPATH += ../library/libcsys/
LIBS += -L../library/libcsys/ -lcsys

FORMS += \
    about/about.ui \
    bookmarks/bookmarkdialog.ui \
    bookmarks/bookmarks.ui \
    coreaction/coreaction.ui \
    coreaction/wbattery.ui \
    coreaction/wcalculator.ui \
    coreaction/wcalender.ui \
    coreaction/wnetwork.ui \
    coreaction/wnotes.ui \
    coreaction/wsystem.ui \
    corearchiver/corearchiver.ui \
    corebox/corebox.ui \
    corefm/corefm.ui \
    corefm/propertiesw.ui \
    corefm/renamedialog.ui \
    coreimage/coreimage.ui \
    corepad/corepad.ui \
    corepaint/corepaint.ui \
    coreplayer/coreplayer.ui \
    corerenamer/corerenamer.ui \
    coreshot/coreshot.ui \
    coreshot/coreshotdialog.ui \
    coretime/coretime.ui \
    coretime/snooze.ui \
    dashboard/circlebar.ui \
    dashboard/dashboard.ui \
    dashboard/history_chart.ui \
    dashboard/pbattery.ui \
    dashboard/pdisplay.ui \
    dashboard/pdrives.ui \
    dashboard/pgeneral.ui \
    dashboard/presources.ui \
    help/help.ui \
    search/search.ui \
    settings/settings.ui \
    start/sessionsavedialog.ui \
    start/start.ui

HEADERS += \
    about/about.h \
    bookmarks/bookmarkdialog.h \
    bookmarks/bookmarkmanage.h \
    bookmarks/bookmarks.h \
    coreaction/coreaction.h \
    coreaction/wbattery.h \
    coreaction/wcalculator.h \
    coreaction/wcalender.h \
    coreaction/wnetwork.h \
    coreaction/wnotes.h \
    coreaction/wsystem.h \
    corearchiver/corearchiver.h \
    corebox/corebox.h \
    corebox/globalfunctions.h \
    corefm/applicationdialog.h \
    corefm/clickoutlistview.h \
    corefm/corefm.h \
    corefm/desktopfile.h \
    corefm/fileutils.h \
    corefm/mimeutils.h \
    corefm/mymodel.h \
    corefm/mymodelitem.h \
    corefm/processdialog.h \
    corefm/progressdlg.h \
    corefm/properties.h \
    corefm/propertiesw.h \
    corefm/renamedialog.h \
    corefm/tabbar.h \
    corefm/udisks2.h \
    coreimage/coreimage.h \
    corepad/chighlighter.h \
    corepad/coreedit.h \
    corepad/corepad.h \
    corepaint/dialogs/resizedialog.h \
    corepaint/dialogs/textdialog.h \
    corepaint/instruments/abstractinstrument.h \
    corepaint/instruments/abstractselection.h \
    corepaint/instruments/colorpickerinstrument.h \
    corepaint/instruments/curvelineinstrument.h \
    corepaint/instruments/ellipseinstrument.h \
    corepaint/instruments/eraserinstrument.h \
    corepaint/instruments/fillinstrument.h \
    corepaint/instruments/lineinstrument.h \
    corepaint/instruments/pencilinstrument.h \
    corepaint/instruments/rectangleinstrument.h \
    corepaint/instruments/selectioninstrument.h \
    corepaint/instruments/sprayinstrument.h \
    corepaint/instruments/textinstrument.h \
    corepaint/widgets/colorchooser.h \
    corepaint/widgets/imagepreview.h \
    corepaint/additionaltools.h \
    corepaint/corepaint.h \
    corepaint/datasingleton.h \
    corepaint/easypaintenums.h \
    corepaint/imagearea.h \
    corepaint/undocommand.h \
    corepdf/corepdf.h \
    coreplayer/coreplayer.h \
    corerenamer/corerenamer.h \
    corerenamer/undocommands.h \
    coreshot/coreshot.h \
    coreshot/coreshotdialog.h \
    coreshot/modefullscreen.h \
    coreshot/modeselectarea.h \
    coreshot/previewwidget.h \
    coreshot/rectarea.h \
    coreshot/screenwidget.h \
    coreterminal/coreterminal.h \
    coretime/alarm.h \
    coretime/coretime.h \
    coretime/fileio.h \
    coretime/schedule.h \
    coretime/schedulecollection.h \
    coretime/snooze.h \
    coretime/timer.h \
    dashboard/battery.h \
    dashboard/circlebar.h \
    dashboard/dashboard.h \
    dashboard/history_chart.h \
    dashboard/info_manager.h \
    dashboard/pbattery.h \
    dashboard/pdisplay.h \
    dashboard/pdrives.h \
    dashboard/pgeneral.h \
    dashboard/presources.h \
    dashboard/signal_mapper.h \
    dashboard/upower.h \
    dashboard/utilities.h \
    help/help.h \
    search/search.h \
    settings/settings.h \
    settings/settingsmanage.h \
    start/sessionsavedialog.h \
    start/slidingstackedwidget.h \
    start/start.h

SOURCES += \
    about/about.cpp \
    bookmarks/bookmarkdialog.cpp \
    bookmarks/bookmarkmanage.cpp \
    bookmarks/bookmarks.cpp \
    coreaction/coreaction.cpp \
    coreaction/wbattery.cpp \
    coreaction/wcalculator.cpp \
    coreaction/wcalender.cpp \
    coreaction/wnetwork.cpp \
    coreaction/wnotes.cpp \
    coreaction/wsystem.cpp \
    corearchiver/corearchiver.cpp \
    corebox/corebox.cpp \
    corebox/globalfunctions.cpp \
    corefm/applicationdialog.cpp \
    corefm/corefm.cpp \
    corefm/desktopfile.cpp \
    corefm/fileutils.cpp \
    corefm/mimeutils.cpp \
    corefm/mymodel.cpp \
    corefm/mymodelitem.cpp \
    corefm/processdialog.cpp \
    corefm/progressdlg.cpp \
    corefm/properties.cpp \
    corefm/propertiesw.cpp \
    corefm/renamedialog.cpp \
    corefm/tabbar.cpp \
    corefm/udisks2.cpp \
    coreimage/coreimage.cpp \
    corepad/chighlighter.cpp \
    corepad/coreedit.cpp \
    corepad/corepad.cpp \
    corepaint/dialogs/resizedialog.cpp \
    corepaint/dialogs/textdialog.cpp \
    corepaint/instruments/abstractinstrument.cpp \
    corepaint/instruments/abstractselection.cpp \
    corepaint/instruments/colorpickerinstrument.cpp \
    corepaint/instruments/curvelineinstrument.cpp \
    corepaint/instruments/ellipseinstrument.cpp \
    corepaint/instruments/eraserinstrument.cpp \
    corepaint/instruments/fillinstrument.cpp \
    corepaint/instruments/lineinstrument.cpp \
    corepaint/instruments/pencilinstrument.cpp \
    corepaint/instruments/rectangleinstrument.cpp \
    corepaint/instruments/selectioninstrument.cpp \
    corepaint/instruments/sprayinstrument.cpp \
    corepaint/instruments/textinstrument.cpp \
    corepaint/widgets/colorchooser.cpp \
    corepaint/widgets/imagepreview.cpp \
    corepaint/additionaltools.cpp \
    corepaint/corepaint.cpp \
    corepaint/datasingleton.cpp \
    corepaint/imagearea.cpp \
    corepaint/undocommand.cpp \
    corepdf/corepdf.cpp \
    coreplayer/coreplayer.cpp \
    corerenamer/corerenamer.cpp \
    coreshot/coreshot.cpp \
    coreshot/coreshotdialog.cpp \
    coreshot/modefullscreen.cpp \
    coreshot/modeselectarea.cpp \
    coreshot/previewwidget.cpp \
    coreshot/rectarea.cpp \
    coreshot/screenwidget.cpp \
    coreterminal/coreterminal.cpp \
    coretime/alarm.cpp \
    coretime/coretime.cpp \
    coretime/fileio.cpp \
    coretime/schedule.cpp \
    coretime/schedulecollection.cpp \
    coretime/snooze.cpp \
    coretime/timer.cpp \
    dashboard/battery.cpp \
    dashboard/circlebar.cpp \
    dashboard/dashboard.cpp \
    dashboard/history_chart.cpp \
    dashboard/info_manager.cpp \
    dashboard/pbattery.cpp \
    dashboard/pdisplay.cpp \
    dashboard/pdrives.cpp \
    dashboard/pgeneral.cpp \
    dashboard/presources.cpp \
    dashboard/signal_mapper.cpp \
    dashboard/upower.cpp \
    help/help.cpp \
    search/search.cpp \
    settings/settings.cpp \
    settings/settingsmanage.cpp \
    start/sessionsavedialog.cpp \
    start/slidingstackedwidget.cpp \
    start/start.cpp \
    main.cpp

RESOURCES += \
    icons.qrc \
    other.qrc \
    ../docs/docs.qrc \
    style.qrc

# C99/C++11 Support for Qt5
isEqual( QT_MAJOR_VERSION, 5 ) {
        QMAKE_CFLAGS += -std=c99
        QMAKE_CXXFLAGS += -std=c++11
}

# Enable warnings and threading support
CONFIG += thread silent build_all #warn_off

# Disable Debug on Release
#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# Build location

isEmpty( BUILD_PREFIX ) {
        BUILD_PREFIX = ./build
}

greaterThan(QT_MAJOR_VERSION, 4) {
        MOC_DIR 	= $$BUILD_PREFIX/moc-qt5
        OBJECTS_DIR = $$BUILD_PREFIX/obj-qt5
        RCC_DIR		= $$BUILD_PREFIX/qrc-qt5
        UI_DIR      = $$BUILD_PREFIX/uic-qt5
}

unix {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }
        BINDIR = $$PREFIX/bin

        target.path = $$BINDIR

        #QMAKE_RPATHDIR += $$PREFIX/lib/corebox/

        desktop.path = $$PREFIX/share/applications/
        desktop.files = ../CoreBox.desktop

        icons.path = $$PREFIX/share/icons/hicolor/scalable/apps/
        icons.files = ../CoreBox.svg

        data.path = $$PREFIX/share/corebox/
        data.files = ../docs/ChangeLog ../docs/ReleaseNotes

        INSTALLS += target icons data desktop #mime templates
}



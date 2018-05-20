TEMPLATE = app

QT += core gui dbus multimedia multimediawidgets charts concurrent #svg printsupport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = coreBox

INCLUDEPATH += ../library/qpdflib
LIBS += -L$$OUT_PWD/../library/qpdflib

LIBS += -lqpdf
LIBS += -lmagic

# install
isEmpty(PREFIX) {
    PREFIX = /usr
}
 
isEmpty(BINDIR) {
    BINDIR = $$PREFIX/bin
}
 
isEmpty(DATADIR) {
    DATADIR = $$PREFIX/share
}

isEmpty(ETCDIR) {
    ETCDIR = /etc
}

isEmpty(LIBDIR) {
    LIBDIR = $$PREFIX/lib
}

target.path = $$BINDIR
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
desktop.files += CoreBox.desktop
desktop.path = $$DATADIR/applications
icon.files += coreBox.svg
icon.path = $$DATADIR/icons
INSTALLS += target desktop icon

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += \
    icons.qrc \
    other.qrc

FORMS += \
    about/about.ui \
    bookmarks/bookmarks.ui \
    coreaction/coreaction.ui \
    corebox/corebox.ui \
    corefm/corefm.ui \
    corefm/propertiesw.ui \
    coreimage/coreimage.ui \
    corepad/corepad.ui \
    corepaint/corepaint.ui \
    coreplayer/coreplayer.ui \
    coretime/coretime.ui \
    coretime/snooze.ui \
    dashboard/info/circlebar.ui \
    dashboard/info/sysinfo.ui \
    dashboard/resources/history_chart.ui \
    dashboard/resources/resources_page.ui \
    dashboard/dashboard.ui \
    help/help.ui \
    search/search.ui \
    settings/settings.ui \
    start/start.ui

HEADERS += \
    about/about.h \
    bookmarks/bookmarkdialog.h \
    bookmarks/bookmarkmanage.h \
    bookmarks/bookmarks.h \
    coreaction/coreaction.h \
    corebox/corebox.h \
    corebox/globalfunctions.h \
    corefm/applicationdialog.h \
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
    corefm/tabbar.h \
    corefm/udisks2.h \
    coreimage/coreimage.h \
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
    corepaint/instruments/magnifierinstrument.h \
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
    coreplayer/coreplayer.h \
    corescreenshot/mainwindow.h \
    corescreenshot/modefullscreen.h \
    corescreenshot/modeselectarea.h \
    corescreenshot/previewwidget.h \
    corescreenshot/rectarea.h \
    corescreenshot/screenwidget.h \
    coretime/alarm.h \
    coretime/coretime.h \
    coretime/fileio.h \
    coretime/schedule.h \
    coretime/schedulecollection.h \
    coretime/snooze.h \
    coretime/timer.h \
    dashboard/info/circlebar.h \
    dashboard/info/sysinfo.h \
    dashboard/info/system_info.h \
    dashboard/lib/command_util.h \
    dashboard/lib/cpu_info.h \
    dashboard/lib/info_manager.h \
    dashboard/lib/memory_info.h \
    dashboard/lib/network_info.h \
    dashboard/lib/stacer-core_global.h \
    dashboard/resources/history_chart.h \
    dashboard/resources/resources_page.h \
    dashboard/battery.h \
    dashboard/dashboard.h \
    dashboard/upower.h \
    help/help.h \
    search/search.h \
    settings/settings.h \
    settings/settingsmanage.h \
    start/start.h \
    corepad/coreedit.h \
    corefm/clickoutlistview.h \
    corepdf/corepdf.h

SOURCES += \
    about/about.cpp \
    bookmarks/bookmarkdialog.cpp \
    bookmarks/bookmarkmanage.cpp \
    bookmarks/bookmarks.cpp \
    coreaction/coreaction.cpp \
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
    corefm/tabbar.cpp \
    corefm/udisks2.cpp \
    coreimage/coreimage.cpp \
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
    corepaint/instruments/magnifierinstrument.cpp \
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
    coreplayer/coreplayer.cpp \
    corescreenshot/mainwindow.cpp \
    corescreenshot/modefullscreen.cpp \
    corescreenshot/modeselectarea.cpp \
    corescreenshot/previewwidget.cpp \
    corescreenshot/rectarea.cpp \
    corescreenshot/screenwidget.cpp \
    coretime/alarm.cpp \
    coretime/coretime.cpp \
    coretime/fileio.cpp \
    coretime/schedule.cpp \
    coretime/schedulecollection.cpp \
    coretime/snooze.cpp \
    coretime/timer.cpp \
    dashboard/info/circlebar.cpp \
    dashboard/info/sysinfo.cpp \
    dashboard/info/system_info.cpp \
    dashboard/lib/command_util.cpp \
    dashboard/lib/cpu_info.cpp \
    dashboard/lib/info_manager.cpp \
    dashboard/lib/memory_info.cpp \
    dashboard/lib/network_info.cpp \
    dashboard/resources/history_chart.cpp \
    dashboard/resources/resources_page.cpp \
    dashboard/battery.cpp \
    dashboard/dashboard.cpp \
    dashboard/upower.cpp \
    help/help.cpp \
    search/search.cpp \
    settings/settings.cpp \
    settings/settingsmanage.cpp \
    start/start.cpp \
    corepad/coreedit.cpp \
    main.cpp \
    corepdf/corepdf.cpp

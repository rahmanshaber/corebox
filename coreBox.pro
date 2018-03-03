QT += core gui dbus printsupport multimedia multimediawidgets charts concurrent
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = coreBox
TEMPLATE = app
LIBS += -lmagic

target.path = $$BINDIR
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
desktop.files += CoreBox.desktop
desktop.path = $$DATADIR/applications
icon.files += coreBox.svg
icon.path = $$DATADIR/icons
INSTALLS += target desktop icon

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    corebox.cpp \
    bookmarks.cpp \
    about.cpp \
    bookmarkdialog.cpp \
    coreedit.cpp \
    bookmarkmanage.cpp \
    settings.cpp \
    coreaction/coreaction.cpp \
    help.cpp \
    start.cpp \
    dashboard/battery.cpp \
    dashboard/upower.cpp \
    search/search.cpp \
    settingsmanage.cpp \
    globalfunctions.cpp \
    dashboard/dashboard.cpp \
    dashboard/info/circlebar.cpp \
    dashboard/info/sysinfo.cpp \
    dashboard/info/system_info.cpp \
    dashboard/lib/command_util.cpp \
    dashboard/lib/cpu_info.cpp \
    dashboard/lib/file_util.cpp \
    dashboard/lib/format_util.cpp \
    dashboard/lib/info_manager.cpp \
    dashboard/lib/memory_info.cpp \
    dashboard/lib/network_info.cpp \
    dashboard/resources/history_chart.cpp \
    dashboard/resources/resources_page.cpp


HEADERS += \
    corebox.h \
    bookmarks.h \
    about.h \
    bookmarkdialog.h \
    coreedit.h \
    bookmarkmanage.h \
    settings.h \
    coreaction/coreaction.h \
    help.h \
    start.h \
    dashboard/battery.h \
    dashboard/upower.h \
    search/search.h \
    settingsmanage.h \
    globalfunctions.h \
    dashboard/dashboard.h \
    dashboard/info/circlebar.h \
    dashboard/info/sysinfo.h \
    dashboard/info/system_info.h \
    dashboard/lib/command_util.h \
    dashboard/lib/cpu_info.h \
    dashboard/lib/file_util.h \
    dashboard/lib/format_util.h \
    dashboard/lib/info_manager.h \
    dashboard/lib/memory_info.h \
    dashboard/lib/network_info.h \
    dashboard/lib/stacer-core_global.h \
    dashboard/resources/history_chart.h \
    dashboard/resources/resources_page.h


FORMS += \
    corebox.ui \
    bookmarks.ui \
    about.ui \
    settings.ui \
    coreaction/coreaction.ui \
    help.ui \
    start.ui \
    search/search.ui \
    dashboard/dashboard.ui \
    dashboard/info/circlebar.ui \
    dashboard/info/sysinfo.ui \
    dashboard/resources/history_chart.ui \
    dashboard/resources/resources_page.ui


#coreApps files

SOURCES += \
    corepad/corepad.cpp \
    coreimage/coreimage.cpp \
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
    corescreenshot/mainwindow.cpp \
    corescreenshot/modefullscreen.cpp \
    corescreenshot/modeselectarea.cpp \
    corescreenshot/previewwidget.cpp \
    corescreenshot/rectarea.cpp \
    corescreenshot/screenwidget.cpp \
    coreplayer/coreplayer.cpp \
    coreplayer/playercontrols.cpp \
    coreplayer/playlistmodel.cpp \
    corefm/applicationdialog.cpp \
    corefm/corefm.cpp \
    corefm/desktopfile.cpp \
    corefm/fileutils.cpp \
    corefm/icondlg.cpp \
    corefm/mimeutils.cpp \
    corefm/mymodel.cpp \
    corefm/mymodelitem.cpp \
    corefm/processdialog.cpp \
    corefm/progressdlg.cpp \
    corefm/properties.cpp \
    corefm/propertiesw.cpp \
    corefm/tabbar.cpp \
    corefm/udisks2.cpp \
    coretime/alarm.cpp \
    coretime/coretime.cpp \
    coretime/fileio.cpp \
    coretime/schedule.cpp \
    coretime/schedulecollection.cpp \
    coretime/settingdialog.cpp \
    coretime/snooze.cpp \
    coretime/timer.cpp \

HEADERS += \
    corepad/corepad.h \
    coreimage/coreimage.h \
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
    corescreenshot/mainwindow.h \
    corescreenshot/modefullscreen.h \
    corescreenshot/modeselectarea.h \
    corescreenshot/previewwidget.h \
    corescreenshot/rectarea.h \
    corescreenshot/screenwidget.h \
    coreplayer/coreplayer.h \
    coreplayer/playercontrols.h \
    coreplayer/playlistmodel.h \
    corefm/applicationdialog.h \
    corefm/corefm.h \
    corefm/desktopfile.h \
    corefm/fileutils.h \
    corefm/icondlg.h \
    corefm/mimeutils.h \
    corefm/mymodel.h \
    corefm/mymodelitem.h \
    corefm/processdialog.h \
    corefm/progressdlg.h \
    corefm/properties.h \
    corefm/propertiesw.h \
    corefm/tabbar.h \
    corefm/udisks2.h \
    coretime/alarm.h \
    coretime/coretime.h \
    coretime/fileio.h \
    coretime/schedule.h \
    coretime/schedulecollection.h \
    coretime/settingdialog.h \
    coretime/snooze.h \
    coretime/timer.h \

FORMS += \
    corepad/corepad.ui \
    coreimage/coreimage.ui \
    corepaint/corepaint.ui \
    coreplayer/coreplayer.ui \
    corefm/corefm.ui \
    corefm/propertiesw.ui \
    coretime/coretime.ui \
    coretime/settingdialog.ui \
    coretime/snooze.ui

DISTFILES += \
    CoreBox.desktop \

RESOURCES += \
    icons.qrc \




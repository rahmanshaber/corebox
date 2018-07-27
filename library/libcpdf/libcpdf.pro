TEMPLATE = lib
TARGET = cpdf

CONFIG += dll
CONFIG += c++11

# disable all build warnings
CONFIG += warn_off

# Disable Debug on Release
CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

QT += widgets\
      webengine\
      webenginecore\
      webenginewidgets\
      webchannel

DEFINES += CPDFLIB_BUILD

VERSION += 0.1.1

HEADERS =\
    qpdfwidget.h \
    pdfjsbridge.h

SOURCES =\
    qpdfwidget.cpp \
    pdfjsbridge.cpp

RESOURCES += pdfview.qrc

unix {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        INSTALLS	+= target
        CONFIG		+= create_pc no_install_prl link_pkgconfig
        contains(DEFINES, LIB64): target.path = $$INSTALL_PREFIX/lib64
        else: target.path = $$INSTALL_PREFIX/lib

        target.path			= $$PREFIX/lib/

        QMAKE_PKGCONFIG_NAME = libcpdf
        QMAKE_PKGCONFIG_DESCRIPTION = A Qt based archiving solution with libarchive backend
        QMAKE_PKGCONFIG_PREFIX  = $$INSTALL_PREFIX
        QMAKE_PKGCONFIG_LIBDIR  = $$target.path
        QMAKE_PKGCONFIG_INCDIR  = $$includes.path
        QMAKE_PKGCONFIG_VERSION = $$VERSION
        QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}

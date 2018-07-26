TEMPLATE	= lib
TARGET		= carchiver

greaterThan( QT_MAJOR_VERSION, 4 ) {
	TARGET = carchiver
}

VERSION   = 1.1.0

DEPENDPATH 	+= . MimeHandler StandardPaths
INCLUDEPATH	+= . MimeHandler StandardPaths

LIBS += -larchive -lz -lbz2 -llzma

# Input
HEADERS += libarchiveqt.h LibLzma.hpp LibLzma2.hpp LibBZip2.hpp LibGZip.hpp
SOURCES += LibArchive.cpp LibLzma.cpp LibLzma2.cpp LibBZip2.cpp LibGZip.cpp

# MimeType handling + QStandardPaths
lessThan(QT_MAJOR_VERSION, 5) {
	HEADERS += MimeHandler/*.hpp StandardPaths/*.hpp
	SOURCES += MimeHandler/*.cpp StandardPaths/*.cpp
}

CONFIG += silent warn_on
QT -= gui

MOC_DIR			= ../build/moc
OBJECTS_DIR		= ../build/obj
RCC_DIR			= ../build/qrc
UI_DIR			= ../build/uic

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	INSTALLS	+= target includes data
	CONFIG		+= create_pc no_install_prl link_pkgconfig
	contains(DEFINES, LIB64): target.path = $$INSTALL_PREFIX/lib64
	else: target.path = $$INSTALL_PREFIX/lib

	target.path			= $$PREFIX/lib/
	includes.files	= libarchiveqt.h
	includes.path		= $$PREFIX/include/

	data.path = $$PREFIX/share/lib$$TARGET/
	data.files = Changelog README

	QMAKE_PKGCONFIG_NAME = libcarchiver
	QMAKE_PKGCONFIG_DESCRIPTION = A Qt based archiving solution with libarchive backend
	QMAKE_PKGCONFIG_PREFIX  = $$INSTALL_PREFIX
	QMAKE_PKGCONFIG_LIBDIR  = $$target.path
	QMAKE_PKGCONFIG_INCDIR  = $$includes.path
	QMAKE_PKGCONFIG_VERSION = $$VERSION
	QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}

macx {
	INCLUDEPATH += /usr/local/opt/libarchive/include /usr/local/opt/xz/include
	QMAKE_LFLAGS += -L/usr/local/opt/libarchive/lib -L/usr/local/opt/xz/lib
}

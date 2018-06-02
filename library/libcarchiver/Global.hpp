/*
    *
    * Global.hpp - Globally used header
    *
*/

#pragma once

// STL
#include <iostream>
#include <fstream>

// LibArchive
//#include <archive.h>
//#include <archive_entry.h>

// Qt Headers
#include <QtCore>

// SystemWide Headers
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#if QT_VERSION >= 0x050000
	#include <QMimeDatabase>
	#include <QMimeType>
	#include <QStandardPaths>
#else
	#include <NBMimeDatabase.hpp>
	#include <NBMimeType.hpp>
	#include <NBStandardPaths.hpp>
#endif

static QMimeDatabase mimeDb;

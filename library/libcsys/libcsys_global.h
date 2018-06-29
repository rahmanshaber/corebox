#ifndef LIBCSYS_GLOBAL_H
#define LIBCSYS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBCSYS_LIBRARY)
#  define LIBCSYSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBCSYSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBCSYS_GLOBAL_H

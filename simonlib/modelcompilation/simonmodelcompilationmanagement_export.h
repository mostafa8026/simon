#ifndef MODELCOMPILATIONMANAGEMENT_EXPORT_H
#define MODELCOMPILATIONMANAGEMENT_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef MODELCOMPILATIONMANAGEMENT_EXPORT
# if defined(MAKE_MODELCOMPILATIONMANAGEMENT_LIB)
   // We are building this library
#  define MODELCOMPILATIONMANAGEMENT_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define MODELCOMPILATIONMANAGEMENT_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef MODELCOMPILATIONMANAGEMENT_EXPORT_DEPRECATED
#  define MODELCOMPILATIONMANAGEMENT_EXPORT_DEPRECATED KDE_DEPRECATED MODELCOMPILATIONMANAGEMENT_EXPORT
# endif
 
#endif
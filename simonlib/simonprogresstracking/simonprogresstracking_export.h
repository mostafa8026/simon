#ifndef SIMON_SIMONPROGRESSTRACKING_EXPORT_H_1250BD8FD4E949E1B720AF1D9D49282E
#define SIMON_SIMONPROGRESSTRACKING_EXPORT_H_1250BD8FD4E949E1B720AF1D9D49282E
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONPROGRESSTRACKING_EXPORT
# if defined(MAKE_SIMONPROGRESSTRACKING_LIB)
   // We are building this library
#  define SIMONPROGRESSTRACKING_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONPROGRESSTRACKING_EXPORT 
# endif
#endif
 
# ifndef SIMONPROGRESSTRACKING_EXPORT_DEPRECATED
#  define SIMONPROGRESSTRACKING_EXPORT_DEPRECATED KDE_DEPRECATED SIMONPROGRESSTRACKING_EXPORT
# endif
 
#endif

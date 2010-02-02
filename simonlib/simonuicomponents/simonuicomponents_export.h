#ifndef SIMON_SIMONUICOMPONENTS_EXPORT_H_8C1BCEFB83F444A7AB46723036FEBCD8
#define SIMON_SIMONUICOMPONENTS_EXPORT_H_8C1BCEFB83F444A7AB46723036FEBCD8
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONUICOMPONENTS_EXPORT
# if defined(MAKE_SIMONUICOMPONENTS_LIB)
   // We are building this library
#  define SIMONUICOMPONENTS_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONUICOMPONENTS_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONUICOMPONENTS_EXPORT_DEPRECATED
#  define SIMONUICOMPONENTS_EXPORT_DEPRECATED KDE_DEPRECATED SIMONUICOMPONENTS_EXPORT
# endif
 
#endif

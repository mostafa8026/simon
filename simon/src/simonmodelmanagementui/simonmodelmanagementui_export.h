#ifndef SIMON_SIMONMODELMANAGEMENTUI_EXPORT_H_C3841445DD894A0686B8259A98489E0A
#define SIMON_SIMONMODELMANAGEMENTUI_EXPORT_H_C3841445DD894A0686B8259A98489E0A
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONMODELMANAGEMENTUI_EXPORT
# if defined(MAKE_SIMONMODELMANAGEMENTUI_LIB)
   // We are building this library
#  define SIMONMODELMANAGEMENTUI_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONMODELMANAGEMENTUI_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONMODELMANAGEMENTUI_EXPORT_DEPRECATED
#  define SIMONMODELMANAGEMENTUI_EXPORT_DEPRECATED KDE_DEPRECATED SIMONMODELMANAGEMENTUI_EXPORT
# endif
 
#endif

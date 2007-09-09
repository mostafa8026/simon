//
// C++ Interface: kdeprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KDEPROGRAMMANAGER_H
#define KDEPROGRAMMANAGER_H

#include "programmanager.h"
#include "program.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class KDEProgramManager : public ProgramManager {
private:
	QStringList kdeCategoriesToSimonCategories(QStringList kdeCategories);
public:
	bool loadPrograms();
    	KDEProgramManager();
	ProgramList* getPrograms(ProgramCategory category);

    ~KDEProgramManager();

};

#endif

/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_DIALOGTEXTPARSER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_DIALOGTEXTPARSER_H_7A7B9100FF5245329569C1B540119C37

#include <QList>
#include "simondialogengine_export.h"

class DialogDataProvider;
class DialogTemplateOptions;
class DialogBoundValues;
class QStringList;

class SIMONDIALOGENGINE_EXPORT DialogTextParser
{
  private:
    DialogTemplateOptions *m_templateOptions;
    DialogBoundValues *m_boundValues;

    bool parseTemplates(QString& data);
    bool parseBoundValues(QString& data);

  public:
    DialogTextParser(DialogTemplateOptions* templateOptions, DialogBoundValues* boundValues);
    ~DialogTextParser();
    
    void setArguments(const QStringList& arguments);

    bool parse(QString& data);
};

#endif




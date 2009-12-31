/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_DESKTOPGRIDCONFIGURATION_H_F800C7EC08DA40FEBD5B5BA233D7AC7C
#define SIMON_DESKTOPGRIDCONFIGURATION_H_F800C7EC08DA40FEBD5B5BA233D7AC7C

#include <simonscenarios/commandconfiguration.h>
#include "ui_desktopgridconfigurationdlg.h"

class DesktopGridConfiguration : public CommandConfiguration
{
	Q_OBJECT
	
	private:
		Ui::DesktopGridConfigurationDlg ui;
 
	public slots:
		virtual bool deSerialize(const QDomElement&);
		virtual QDomElement serialize(QDomDocument *doc);
		virtual void defaults();
	
	public:
		DesktopGridConfiguration(Scenario *parent, const QVariantList &args = QVariantList());
		~DesktopGridConfiguration();
		
		//configuration options
		bool useRealTransparency();
};

#endif

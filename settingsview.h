//
// C++ Interface: settingsview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QDialog>
#include "ui_settings.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class SettingsView : public QDialog
{
private:
	Ui::SettingsDialog ui;
public:
    SettingsView(QWidget *parent);

    ~SettingsView();

};

#endif

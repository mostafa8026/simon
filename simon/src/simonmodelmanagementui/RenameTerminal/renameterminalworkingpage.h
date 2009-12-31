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


#ifndef SIMON_RENAMETERMINALWORKINGPAGE_H_3D51221A254347329D67EF19AE2477BB
#define SIMON_RENAMETERMINALWORKINGPAGE_H_3D51221A254347329D67EF19AE2477BB

#include <QWizardPage>
#include "ui_renameterminalworkingpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class RenameTerminal;
class RenameTerminalWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void done();
private slots:
	void finish();
	void displayProgress(int);
private:
	Ui::RenameTerminalWorkingPage ui;
	bool complete;
	RenameTerminal *renameTerminal;

public:
    RenameTerminalWorkingPage(QWidget *parent);

	void initializePage();
	bool isComplete() const { return this->complete; }

    ~RenameTerminalWorkingPage();

};

#endif

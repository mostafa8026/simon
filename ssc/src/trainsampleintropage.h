/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_TRAINSAMPLEINTROPAGE_H_417017FF7135400DAC9F8BA0263C0FA5
#define SIMON_TRAINSAMPLEINTROPAGE_H_417017FF7135400DAC9F8BA0263C0FA5

#include <QWizardPage>

class QCheckBox;

class TrainSampleIntroPage : public QWizardPage
{

  private:
    QCheckBox *cbPowerTrain;

  public:
    TrainSampleIntroPage(QWidget *parent=0);
    void initializePage();
    ~TrainSampleIntroPage();

};
#endif

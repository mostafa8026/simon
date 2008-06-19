/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QDir>
#include <QCoreApplication>
#include "selectplacepage.h"

SelectPlacePage::SelectPlacePage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);

	setTitle(tr("Ort ausw�hlen"));
	
	connect(ui.cbProtocol->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.leUser, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.lePass, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.leHost, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.lePath, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.cbAuthentification, SIGNAL(toggled(bool)), this, SLOT(buildRemoteUrl()));
	connect(ui.leRemoteUrl, SIGNAL(textEdited(QString)), this, SLOT(parseRemoteUrl()));
	
	connect(ui.rbLocalPlace, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(ui.leRemoteUrl, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
	connect(ui.leLocalUrl, SIGNAL(urlChanged(QString)), this, SIGNAL(completeChanged()));
	
}

void SelectPlacePage::initializePage()
{
	ui.gbRemoteHelp->setChecked(false);
	
	ui.leLocalUrl->hide();
	ui.leRemoteUrl->hide();
	ui.lbRemoteUrl->hide();
	ui.gbRemoteHelp->hide();
	ui.lbProtocol->hide();
	ui.cbProtocol->hide();
	ui.lbHost->hide();
	ui.leHost->hide();
	ui.lbPath->hide();
	ui.lePath->hide();
	ui.cbAuthentification->hide();
	ui.leUser->hide();
	ui.lbUser->hide();
	ui.lbPassword->hide();
	ui.lePass->hide();
}

bool SelectPlacePage::isComplete() const
{
	bool complete=false;
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		complete = ! (ui.leLocalUrl->text().isEmpty());
	} else
	{ //remote place
		complete = ! (ui.leRemoteUrl->text().isEmpty());
	}
	return (ui.rbLocalPlace->isChecked() || ui.rbRemotePlace->isChecked()) && complete;
}

void SelectPlacePage::buildRemoteUrl()
{
	QUrl url;
	url.setScheme(ui.cbProtocol->currentText());
	url.setHost(ui.leHost->text());
	url.setPath(ui.lePath->text());
	url.setUserName(ui.leUser->text());
	url.setPassword(ui.lePass->text());
	QString urlStr = url.toString((!ui.cbAuthentification->isChecked()) ? QUrl::RemoveUserInfo : QUrl::None);
	ui.leRemoteUrl->setText(urlStr);
}

QString SelectPlacePage::getName()
{
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		return QDir(ui.leLocalUrl->text()).dirName();
	} else
	{ //remote place
		return QUrl(ui.leRemoteUrl->text()).host();
	}
}

void SelectPlacePage::parseRemoteUrl()
{
	QUrl url(ui.leRemoteUrl->text());
	ui.cbProtocol->setEditText(url.scheme());
	ui.leHost->setText(url.host());
	ui.lePath->setText(url.path());
	
	if (!url.userInfo().isEmpty())
	{
		ui.cbAuthentification->setChecked(true);
		ui.leUser->setText(url.userName());
		ui.lePass->setText(url.password());
	} else ui.cbAuthentification->setChecked(false);
}

QUrl SelectPlacePage::getUrl()
{
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		return QUrl(ui.leLocalUrl->text());
	} else
	{ //remote place
		return QUrl(ui.leRemoteUrl->text());
	}
}
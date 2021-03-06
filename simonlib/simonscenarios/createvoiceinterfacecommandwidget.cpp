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

#include "createvoiceinterfacecommandwidget.h"
#include "createvoiceinterfacecommandwidgetprivate.h"
#include "commandmanager.h"
#include "voiceinterfacecommand.h"
#include <QVBoxLayout>
#include <KDebug>

CreateVoiceInterfaceCommandWidget::CreateVoiceInterfaceCommandWidget(CommandManager *manager, QWidget *parent) :
CreateCommandWidget(manager, parent),  d(new CreateVoiceInterfaceCommandWidgetPrivate(manager, parent))
{
  connect(d, SIGNAL(completeChanged()), this, SIGNAL(completeChanged()));
  connect(d, SIGNAL(commandSuggested(Command*)), this, SIGNAL(commandSuggested(Command*)));

  QVBoxLayout *lay = new QVBoxLayout(this);
  lay->addWidget(d);

  setWindowIcon(manager->icon());
  setWindowTitle(i18nc("%1 is name of the parent command manager (plugin name)", "%1: Voice commands", manager->name()));
}


Command* CreateVoiceInterfaceCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  kDebug() << "Calling create command";
  return d->createCommand(name, iconSrc, description);
}


bool CreateVoiceInterfaceCommandWidget::init(Command* command)
{
  return d->init(command);
}


bool CreateVoiceInterfaceCommandWidget::isComplete()
{
  return d->isComplete();
}

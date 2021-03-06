/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "newcommand.h"

#include <simonscenarios/command.h>
#include <simonscenarios/createcommandwidget.h>
#include <simonactions/actionmanager.h>

#include <KUrl>
#include <KMessageBox>
#include <KKeySequenceWidget>
#include <KDialogButtonBox>
#include <KDebug>

NewCommand::NewCommand(QWidget* parent) : KDialog(parent)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  ui.swCommandCreaters->removeWidget(ui.swCommandCreaters->currentWidget());

  setMainWidget( widget );
  setCaption( i18n("Command") );

  connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(setWindowTitleToCommandName(QString)));
  connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));

  checkIfComplete();
}


void NewCommand::deleteLater()
{
  QObject::deleteLater();
}


bool NewCommand::registerCreators(QList<CreateCommandWidget*> newCommandCreaters)
{
  qDeleteAll(commandCreaters);
  
  foreach (CreateCommandWidget *widget, newCommandCreaters) {
    ui.cbType->addItem(widget->windowIcon(), widget->windowTitle());
    ui.swCommandCreaters->addWidget(widget);
    connect(widget, SIGNAL(completeChanged()), this, SLOT(checkIfComplete()));
    connect(widget, SIGNAL(commandSuggested(Command*)), this, SLOT(commandSuggested(Command*)));
  }

  commandCreaters = newCommandCreaters;
  return true;
}


void NewCommand::commandSuggested(Command *command)
{
  if (!command) return;
  init(command);
  delete command;
}

void NewCommand::switchToTypeOfManager(CommandManager *manager)
{
  int i = 0;
  foreach (CreateCommandWidget *widget, commandCreaters) {
    if (widget->isInstanceOfSameManager(manager)) {
      ui.cbType->setCurrentIndex(i);
      break;
    }
    i++;
  }
}

void NewCommand::init(Command *command)
{
  if (!command) return;

  ui.leTrigger->setText(command->getTrigger());
  ui.ibIcon->setIcon(command->getIconSrc());
  ui.teDescription->setPlainText(command->getDescription());

  bool found=false;
  int i=0;
  foreach (CreateCommandWidget *widget, commandCreaters) {
    if (widget->isInstanceOfSameManager(command)) {
      widget->init(command);
      found=true;
      ui.cbType->setCurrentIndex(i);
      break;
    }
    i++;
  }
  if (!found)
    KMessageBox::error(this, i18n("Could not determine command type"));

  checkIfComplete();
}


void NewCommand::checkIfComplete()
{
  CreateCommandWidget *creater = dynamic_cast<CreateCommandWidget*>(ui.swCommandCreaters->currentWidget());
  // 	Q_ASSERT(creater);

  bool complete;
  if (!creater)
    complete = false;
  else
    complete = (!ui.leTrigger->text().isEmpty()) && creater->isComplete();

  enableButtonOk(complete);
}


void NewCommand::setWindowTitleToCommandName(QString name)
{
  if (!name.isEmpty())
    setCaption(i18nc("%1 is the name of the command", "Command: %1", name));
  else setCaption(i18n("Command"));
}


bool NewCommand::newCommand(CommandManager* preSelectedCategory)
{
  switchToTypeOfManager(preSelectedCategory);

  if (ui.swCommandCreaters->count() == 0) {
    KMessageBox::information(this, i18n("No command plugins loaded that provide command engines.\n\nYou can load some in the \"Manage plugins\" dialog."));
    return false;
  }

  if (KDialog::exec()) {
    //creating
    CreateCommandWidget *creater = dynamic_cast<CreateCommandWidget*>(ui.swCommandCreaters->currentWidget());
    kDebug() << "Creating with creater: " << creater;
    Q_ASSERT(creater);

    if (!creater) return false;

    return creater->addCommand(ui.leTrigger->text(), ui.ibIcon->icon(), ui.teDescription->toPlainText());
  }
  return false;
}


NewCommand::~NewCommand()
{
  qDeleteAll(commandCreaters);
}

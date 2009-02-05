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

#include "createcompositecommandwidget.h"
#include "compositecommand.h"
#include "delaycommand.h"

#include <simonactions/actionmanager.h>
#include <simonactions/commandtablemodel.h>

#include <QListWidget>
#include <QInputDialog>
#include <KMessageBox>
#include <QStringList>

CreateCompositeCommandWidget::CreateCompositeCommandWidget(QWidget *parent) : CreateCommandWidget(parent)
{
	ui.setupUi(this);
	
	setWindowIcon(CompositeCommand::staticCategoryIcon());
	setWindowTitle(CompositeCommand::staticCategoryText());
	
	allCommands = ActionManager::getInstance()->getCommandList();
	foreach (const Command* com, *allCommands)
	{
		QString name = com->getTrigger();
		QString category = com->getCategoryText();
		ui.cbCommands->addItem(com->getIcon(), name+" ("+category+")");
	}

	model = new CommandTableModel();
	ui.tvCommands->setModel(model);

	connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(removeCommand()));
	connect(ui.pbAddCommand, SIGNAL(clicked()), this, SLOT(addCommand()));
	connect(ui.pbAddDelay, SIGNAL(clicked()), this, SLOT(addDelay()));
	connect(ui.pbMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui.pbMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(ui.tvCommands, SIGNAL(clicked(const QModelIndex&)), this, SLOT(enableButtons(const QModelIndex&)));
	enableButtons(ui.tvCommands->currentIndex());

	ui.pbAddCommand->setIcon(KIcon("list-add"));
	ui.pbAddDelay->setIcon(KIcon("list-add"));
	ui.pbRemove->setIcon(KIcon("list-remove"));
	ui.pbMoveUp->setIcon(KIcon("arrow-up"));
	ui.pbMoveDown->setIcon(KIcon("arrow-down"));
}

void CreateCompositeCommandWidget::enableButtons(const QModelIndex& index)
{
	if (!index.isValid())
	{
		ui.pbRemove->setEnabled(false);
		ui.pbMoveUp->setEnabled(false);
		ui.pbMoveDown->setEnabled(false);
		return;
	} else
		ui.pbRemove->setEnabled(true);

	Q_ASSERT(allCommands);
	ui.pbMoveUp->setEnabled(index.row() > 0);
	ui.pbMoveDown->setEnabled(index.row() < model->rowCount()-1);
	
}

void CreateCompositeCommandWidget::moveUp()
{
	model->moveUp(ui.tvCommands->currentIndex());
	int row = ui.tvCommands->currentIndex().row();
	ui.tvCommands->selectRow(row-1);
	enableButtons(ui.tvCommands->currentIndex());
}

void CreateCompositeCommandWidget::moveDown()
{
	model->moveDown(ui.tvCommands->currentIndex());
	int row = ui.tvCommands->currentIndex().row();
	ui.tvCommands->selectRow(row+1);
	enableButtons(ui.tvCommands->currentIndex());
}


bool CreateCompositeCommandWidget::isComplete()
{
	return (model->rowCount() > 0);
}

bool CreateCompositeCommandWidget::init(Command* command)
{
	Q_ASSERT(command);
	
	CompositeCommand *compositeCommand = dynamic_cast<CompositeCommand*>(command);
	if (!compositeCommand) return false;

	QStringList selectedTriggers = compositeCommand->getCommands();
	QStringList selectedCategories = compositeCommand->getCommandTypes();

	Q_ASSERT(selectedTriggers.count() == selectedCategories .count());

	QStringList notFound;
	int i=0;
	foreach (const QString& trigger, selectedTriggers)
	{
		QString cat = selectedCategories[i];
		if (cat == i18n("Delay"))
		{
			bool ok;
			int delay = trigger.toInt(&ok);
			if (ok)
				model->selectCommand(new DelayCommand(delay));
		} else {
			bool found=false;
			foreach (Command* com, *allCommands)
			{
				if ((com->getTrigger() == trigger) &&
					(com->getCategoryText() == cat))
				{
					//found the command
					model->selectCommand(com);
					found=true;
					break;
				}
			}
			if (!found)
				notFound << trigger;
		}


		i++;
	}


	if (!notFound.isEmpty())
	{
		KMessageBox::sorry(this, i18n("Couldn't find all of the commands that make up this composited command.\n\n"
						"The missing commands are: %1.", notFound.join(", ")));
	}

	return true;
}


void CreateCompositeCommandWidget::addCommand()
{
	model->selectCommand(allCommands->at(ui.cbCommands->currentIndex()));
	enableButtons(ui.tvCommands->currentIndex());
	emit completeChanged();
}


void CreateCompositeCommandWidget::removeCommand()
{
	model->removeCommand(ui.tvCommands->currentIndex().row());
	enableButtons(ui.tvCommands->currentIndex());
	emit completeChanged();
}

void CreateCompositeCommandWidget::addDelay()
{
	int delay = ui.sbDelay->value();

	model->selectCommand(new DelayCommand(delay));
	enableButtons(ui.tvCommands->currentIndex());
	emit completeChanged();
}


Command* CreateCompositeCommandWidget::createCommand(const QString& name, const QString& iconSrc)
{
	CommandList *selectedCommands = model->selectedCommands();
	QStringList selectedTriggers, selectedCategories;

	foreach (Command* com, *selectedCommands)
	{
		selectedTriggers << com->getTrigger();
		selectedCategories << com->getCategoryText();
	}

	return new CompositeCommand(name, iconSrc, selectedTriggers, selectedCategories);
}

CreateCompositeCommandWidget::~CreateCompositeCommandWidget()
{
}

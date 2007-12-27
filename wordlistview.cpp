//
// C++ Implementation: wordlistview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QProgressDialog>
#include <QCoreApplication>
#include <QIcon>
#include "logger.h"
#include "wordlistview.h"
#include "settings.h"
#include "deleteworddialog.h"

/**
 * @brief Constructor
 *
 * Creates a new WordlistView Objects, creates the Links
 * and initializes the list of vocabulary
 *
 * @author Peter Grasch
 */
WordListView::WordListView(TrainingView *trainView, QWidget *parent) : InlineWidget(tr("Wortliste"), 
	QIcon(":/images/icons/format-justify-fill.svg"), 
	tr("Betrachten und bearbeiten der Wortliste"), parent)
{

	shownDialogs = 0;
	abortVocabInsertion = false;
	
	ui.setupUi(this);

	ui.twVocab->verticalHeader()->hide();
	
	importDictView = new ImportDictView(this);
	guessChildTriggers(this);
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(ui.lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	connect(ui.pbAddWord, SIGNAL(clicked()), this, SIGNAL(showAddWordDialog()));
	
	connect(ui.pbSuggestTrain, SIGNAL(clicked()), this, SLOT(suggestTraining()));
	connect(ui.pbRemoveWord, SIGNAL(clicked()), this, SLOT(deleteSelectedWord()));
	connect(ui.leSearch, SIGNAL(returnPressed()), this, SLOT(filterListbyPattern()));
	connect(ui.pbClearSearch, SIGNAL(clicked()), this, SLOT(clearSearchText()));
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	connect(importDictView, SIGNAL(dictGenerated(WordList*)), this, SLOT(importDict(WordList*)));
	
	connect(ui.cbShowCompleteLexicon, SIGNAL(toggled(bool)), this, SLOT(toggleExtraWords()));

	this->trainView = trainView; 
	this->wordListManager = new WordListManager(trainView->getManager());
	connect(this->wordListManager, SIGNAL(wordlistChanged()), this, SLOT(reloadList()));
	connect(this->wordListManager, SIGNAL(shadowListChanged()), this, SLOT(reloadList()));
	connect(this->wordListManager, SIGNAL(wordlistChanged()), this, SLOT(askForRebuild()));
	this->initializeItems();

	dirty = false;
	
	hide();
	
	
	
	//guessChildTriggers();
	
	/*registerControl("Suche", ui.leSearch, SLOT(setFocus()));
	registerControl("suchen", ui.leSearch, SLOT(returnPressed()));
	registerControl("leere Suche", ui.pbClearSearch, SLOT(animateClick()));
	
	registerControl("Wechsle Tab", ui.twCurrentAction, SLOT(setCurrentIndex(ui.twCurrentAction->count()-ui.twCurrentAction->currentIndex())));
	
	registerControl("Neues Wort hinzuf�gen", ui.pbAddWord, SLOT(animateClick()));
	registerControl("Importiere W�rterbuch", ui.pbImport, SLOT(animateClick()));
	registerControl("zum Training hinzuf�gen", ui.pbAddToTraining, SLOT(animateClick()));
	registerControl("Trainingsprogramm vorschlagen", ui.pbSuggestTrain, SLOT(animateClick()));
	registerControl("Wort aus Liste l�schen", ui.pbDeleteTrainingWord, SLOT(animateClick()));
	registerControl("Trainieren", ui.pbTrainList, SLOT(animateClick()));*/
	
}


void WordListView::askForRebuild()
{
	//we changed the wordlist
	//we should thus recompile the model
	if (QMessageBox::question(this, tr("�bernehmen"), tr("Um die �nderung zu �bernehmen, muss das Sprachmodell neu generiert werden.\n\nWollen Sie es jetzt neu generieren?"), QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
					wordListManager->compileModel();
}


void WordListView::reloadList()
{
	this->clearList();
	readVocab();
}

/**
 * @brief Clears the Search-field
 *
 * @author Peter Grasch
 */
void WordListView::clearSearchText()
{
	ui.leSearch->setText("");
	filterListbyPattern("");
	ui.leSearch->setFocus();
}

void WordListView::showImportDictDialog()
{
	importDictView->restart();
	importDictView->show();
}

/**
 * \brief Compiles the language model
 * \author Peter Grasch
 */
void WordListView::compileModel()
{
	this->wordListManager->compileModel();
}

/**
 * \brief Import an existing dictionary
 * \author Peter Grasch
 */
void WordListView::importDict(WordList* list)
{
	if (list)
	{
		wordListManager->addWords(list, true,true);
	}
}

/**
 * @brief Suggest a training
 *
 * Uses the recognition rate and chooses the lowest 10
 *
 * @author Peter Grasch
 */
void WordListView::suggestTraining()
{
	ui.twVocab->sortItems(3);
	int toInsert = 10;
	if (ui.twVocab->rowCount() < toInsert) toInsert = ui.twVocab->rowCount();

	if (toInsert==0) QMessageBox::information(this, ("Keine W�rter in der Wortliste"), tr("Es sind nicht gen�gend W�rter im w�rterbuch"));
	for (int i=0; i<toInsert; i++)
	{
		ui.twVocab->setCurrentItem(ui.twVocab->item(i,0));
		copyWordToTrain();
	}
}

/**
 * @brief Marks a word for Training
 *
 * @author Peter Grasch
 */
void WordListView::markWordToTrain(Word word)
{
	this->trainingwordlist.append( word );
	ui.lwTrainingWords->addItem( word.getWord() );
}


/**
 * @brief Filters the QList
 *
 * @param QString filter
 * The pattern which the Vocablist is matched against
 * @author Peter Grasch
 */
void WordListView::filterListbyPattern(QString filter)
{
	if (filter.isEmpty()) filter = ui.leSearch->text();
	WordList *vocab = this->wordListManager->getWordList();
	
	WordList *limitedVocab = new WordList();

	int i=0;
	while (i < vocab->count())
	{
		if (vocab->at(i).getWord().contains(filter, Qt::CaseInsensitive))
			limitedVocab->append(vocab->at(i));
		
		i++;
	}
	insertVocab( limitedVocab );
	
	if (ui.cbShowCompleteLexicon->isChecked())
	{
		vocab = this->wordListManager->getShadowList();
		limitedVocab->clear();
		i=0;
		while (i < vocab->count())
		{
			if (vocab->at(i).getWord().contains(filter, Qt::CaseInsensitive))
				limitedVocab->append(vocab->at(i));
			
			i++;
		}
		insertVocab( limitedVocab );
	}
}

/**
 * \brief Trains the list of words to train
 * Tells the TrainingView to guide the user to the process of training the
 * Wordlist given in lwTrainingWords (member)
 * \author Peter Grasch
 */
void WordListView::trainList()
{
	if (!trainView) return;
	if (this->trainingwordlist.count()==0)
	{
		QMessageBox::critical(this, tr("Keine W�rter selektiert"), tr("Bitte w�hlen Sie zuerst ein paar W�rter f�r das spezielle Training aus.\nZiehen Sie sie dazu von der gro�en Liste links in die kleine Liste oben rechts.\n\nWenn Sie generische Texte vorlesen wollen, gehen Sie bitte zum Allgemeine Training.\n(Der Punkt \"Trainieren\" ist in der \"Globale Aktion\"-Toolbar)."));
		return;
	}
	trainView->trainWords(&trainingwordlist);
	trainView->exec();
}

/**
 * @brief Copys a word to the Traininglist
 *
 * Copys the currently selected word from the twVocab (member) to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void WordListView::copyWordToTrain()
{
	if (ui.twVocab->selectedItems().isEmpty())
	{
		QMessageBox::information(this,tr("Nichts ausgew�hlt"),tr("Bitte selektieren Sie zuerst ein Wort aus der Liste links"));
		return;
	}
	
	QString word = ui.twVocab->item(ui.twVocab->currentRow(),0)->text();
	QString sampa = ui.twVocab->item(ui.twVocab->currentRow(),1)->text();
	QString category = ui.twVocab->item(ui.twVocab->currentRow(),2)->text();
	int probability = ui.twVocab->item(ui.twVocab->currentRow(),3)->text().toInt();
	
	this->trainingwordlist.append(Word(word, sampa, category, probability));
	
	ui.lwTrainingWords->addItem(word);
}


void WordListView::toggleExtraWords()
{
	clearList();
	ui.leSearch->setText("");
	if (ui.cbShowCompleteLexicon->isChecked())
	{
		insertVocab(this->wordListManager->getWordList());
		insertVocab(this->wordListManager->getShadowList());
	} else {
		insertVocab(this->wordListManager->getWordList());
	}
}


/**
 * \brief Clears the VocabList
 * \author Peter Grasch
 */
void WordListView::clearList()
{
	ui.twVocab->setRowCount(0);
}

/**
 * \brief Sets the Member and lets the user know about it
 * \author Peter Grasch
 * \param bool dirty
 */
void WordListView::setDirty(bool dirty)
{
	this->dirty = dirty;
	if (dirty)
	{
		setWindowTitle(tr("Wortliste - Ungesicherte �nderungen"));
	} else setWindowTitle(tr("Wortliste"));
}

void WordListView::show()
{
	if (sImportDict & shownDialogs)
	{
		importDictView->show();
		importDictView->move(importDictPos);
	}
	QWidget::show();
	shownDialogs = 0;
}

void WordListView::hide()
{
	if (importDictView->isVisible())
	{
		shownDialogs = shownDialogs | sImportDict;
		importDictPos = importDictView->pos();
		importDictView->hide();
	}
	
	QWidget::hide();
}

/**
 * \brief Asks the user if he wants to save
 * \author Peter Grasch
 */
void WordListView::askToSave()
{
	if (!dirty) return;
	
	if (QMessageBox::question(this, tr("�nderungen speichern"), tr("Sie haben das Sprachmodell ver�ndert. M�chten Sie die �nderungen speichern?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	{
		//save the dict
		QString filename = "test.out";
		if (this->wordListManager->save(filename))
			setDirty(false);
		else 
		{	
			QMessageBox::critical(this, tr("Fehler beim Speichern"), tr("Das W�rterbuch konnte nicht nach %1 geschrieben werden. Bitte �berpr�fen Sie Ihre Berechtigungen.").arg(filename));
			Logger::log("[ERR] Fehler beim schreiben des W�rterbuches");
		}
	}
}


/**
 * \brief Displays a dialog to ask the user what to do
 * \author Peter Grasch
 * 
 * Available options: 
 * 	Remove completely
 * 	Move to shadow list
 */
void WordListView::deleteSelectedWord()
{
	int row = ui.twVocab->currentRow();
	if (row == -1) //none selected
	{
		QMessageBox::information(this, tr("Nichts ausgew�hlt"), tr("Bitte selektieren Sie ein Wort"));
		return;
	}

	DeleteWordDialog *del = new DeleteWordDialog(this);

	QString word = ui.twVocab->item(row,0)->text();
	QString pronunciation = ui.twVocab->item(row,1)->text();
	QString terminal = ui.twVocab->item(row,2)->text();
	
	bool isShadowed=false;
	bool success = false;

	Word *w = this->wordListManager->getWord(word, pronunciation, terminal, isShadowed);
	if (!w)
	{
		qDebug() << "WTF?";
		return; //word not found?!
	}
	
	if (del->exec(*w, isShadowed))
	{
		//delete the word
		if (del->getDeletionType() == DeleteWordDialog::MoveToShadow)
		{
			success = wordListManager->moveToShadow(w);
		}
		if (del->getDeletionType() == DeleteWordDialog::RemoveCompletely)
		{
			qDebug("Remove completely...");
			if (!wordListManager->deleteCompletely(w, isShadowed))
				QMessageBox::critical(this, tr("Fehler beim L�schen"), tr("Das Wort konnte nicht komplett gel�scht werden.\n\nBitte �berpr�fen Sie die Pfade der Dateien: prompts, codetrain.scp, Pfad der Samples, dict, shadow-dict, voca"));
			else 
				success = true;
		}
	}
}

/**
 * @brief Asks the WordListManager for the whole Vocabulary and sends it to insertVocab
 *
 * @author Peter Grasch
 * @see insertVocab()
 */
void WordListView::readVocab()
{
	WordList *vocab = this->wordListManager->getWordList();
	if (!vocab)
	{
		QMessageBox::critical(0, tr("Lesefehler"), tr("Konnte ben�tigte Dateien f�r die Wortliste nicht einlesen. Bitte �berpr�fen Sie die Pfade zu den Vocab- und Lexikon-Dateien und stellen Sie sicher das sie die n�tigen Leserechte haben.\n\nDie Wortliste wird leer im RAM erstellt. �nderungen werden NICHT dauerhaft gespeichert."));
		wordListManager->safelyInit();
	} else 	insertVocab( vocab );

	if (ui.cbShowCompleteLexicon->isChecked())
	{
		WordList *shadow = this->wordListManager->getShadowList();
		if (shadow)
		{
			insertVocab( shadow );
		}
	}
}

/**
 * @brief Inserts a given Wordlist to the QTableWidget
 *
 * @author Peter Grasch
 * @see readVocab()
 * @param WordList *vocab
 * The Wordlist to insert
 */
void WordListView::insertVocab(WordList *vocab)
{
	abortVocabInsertion=false;
	ui.twVocab->setRowCount(vocab->count());
	QProgressDialog *pgDlg = new QProgressDialog(tr("Lade Wortliste zur Anzeige...\n(Ein Abbruch beeinflusst das intern verwendete W�rterbuch nicht!)"), tr("Abbrechen"), 0, 
			((vocab->count() < 1000) ? vocab->count() : 1000), this);

	connect(pgDlg, SIGNAL(canceled()), this, SLOT(abortInsertion()));

        int i=0;
	int limit=1000;
	
	ui.twVocab->setRowCount(vocab->count());
	while ((!abortVocabInsertion) && (i<vocab->count()) && (i<limit))
	{
		if (!vocab->at(i).getWord().isEmpty())
		{
			QTableWidgetItem *wordName = new QTableWidgetItem(vocab->at(i).getWord());
			ui.twVocab->setItem(i, 0, wordName);
			ui.twVocab->setItem(i, 1, new QTableWidgetItem(vocab->at(i).getPronunciation()));
			ui.twVocab->setItem(i, 2, new QTableWidgetItem(vocab->at(i).getTerminal()));
			
			QTableWidgetItem *prob = new QTableWidgetItem(QString().setNum(vocab->at(i).getPropability()));
			if (vocab->at(i).getPropability() == 0)
				prob->setBackgroundColor( QColor(255,0,0) );
			else 
				if (vocab->at(i).getPropability() < 2)
					prob->setBackgroundColor( QColor( 241, 134, 134 ) );
			ui.twVocab->setItem(i, 3, prob);
			
	
			for (int j = 0; j<4; j++)
				ui.twVocab->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		} else {
			vocab->removeAt(i);
			pgDlg->setMaximum(vocab->count());
			i--;
		}
		pgDlg->setValue(++i);
	}
	ui.twVocab->setRowCount(i);
	pgDlg->hide();
	emit wordlistLoaded();
}


/**
 * @brief Deletes the selected word from the Training-List
 *
 * @todo The list only knows the word - not the pronounciation - if there are more than one it should open a dialog and ask what pronounciation should be deleted
 * @author Peter Grasch
 * @note This deletes every pronounciation of the word in the list
 */
void WordListView::deleteTrainingWord()
{
	if (ui.lwTrainingWords->item(ui.lwTrainingWords->currentRow()))
	{
		QString word = ui.lwTrainingWords->takeItem(ui.lwTrainingWords->currentRow())->text();
		
		int i=0;
		while  (i < trainingwordlist.count())//(wordlist.at(i)->getWord() != word)
		{
			if (trainingwordlist.at(i).getWord() == word)
				trainingwordlist.removeAt(i);
			i++;
		}
		i=0;
		while  (i < ui.lwTrainingWords->count())//(wordlist.at(i)->getWord() != word)
		{
			if (ui.lwTrainingWords->item(i)->text() == word)
				ui.lwTrainingWords->takeItem(i--);
			i++;
		}
	}
}

/**
 * @brief Loads the Vocab
 *
 * @author Peter Grasch
 */
void WordListView::initializeItems()
{

	this->readVocab();
}

/**
 * @brief Hides the tab (editModel), until the pbKeyed Button is checked and the password where given
 *
 * @autor Susanne Tschernegg
 */
void WordListView::hideTbEditModel()
{
    ui.twCurrentAction->removeTab(0);
}

/**
 * @brief Sets the tab (editModel) visible, until the pbKeyed Button is unchecked
 *
 * @autor Susanne Tschernegg
 */
void WordListView::setTbEditModelVisible()
{
    ui.twCurrentAction->insertTab(0,ui.tbEditModel, tr("Wortliste �ndern"));
    ui.twCurrentAction->setCurrentIndex(0);
}


/**
 * @brief Destructor
 *
 * Empty Destructor
 *
 * @author Peter Grasch
 */
WordListView::~WordListView()
{
}

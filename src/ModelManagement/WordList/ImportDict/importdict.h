//
// C++ Interface: importdict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDICT_H
#define IMPORTDICT_H

#define HADIFIXBOMP 1
#define WIKTIONARY 2
#define LEXICON 3

#include "../word.h"
#include <QThread>

class Dict;

/**
 *	\class ImportDict
 *	\brief The ImportDict class provides the functions to import e.g. Wiktionary dicts
 *	\author Peter Grasch
 *	\version 0.1
 */
class ImportDict : public QThread{
Q_OBJECT

private:
	QString pathToDict; //!< the path to the dictionary
	int type;
	Dict *dict;
	bool deleteFileWhenDone;
signals:
	void status(QString);
	void progress(int);
	void finished(WordList*);
	void opened();
private slots:
	void loadProgress(int prog);
	void openingFinished();
public slots:
	void deleteDict();
public:
    ImportDict(QObject *parent=0);
    void run();
    void parseWordList(QString pathToDict, int type, bool deleteFileWhenDone=false);
    ~ImportDict();

};

#endif
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

#ifndef SIMON_SCENARIO_H_24758EAA6EE243FEB25B1E32515C761A
#define SIMON_SCENARIO_H_24758EAA6EE243FEB25B1E32515C761A
#include <QString>
#include <QList>
#include <QObject>
#include <QDateTime>
#include <QDomDocument>
#include <QHash>
#include <KIcon>
#include "speechmodel.h"
#include "vocabulary.h"
#include "scenariomanager.h"
#include <simonrecognitionresult/recognitionresult.h>
#include "simonmodelmanagement_export.h"
#include <simoncontextdetection/compoundcondition.h>

class ScenarioObject;
class Author;
class ActionCollection;
class Word;
class VersionNumber;
class Grammar;
class ActiveVocabulary;
class TrainingTextCollection;
class TrainingText;

class MODELMANAGEMENT_EXPORT Scenario : public QObject
{
  Q_OBJECT

  signals:
    void changed(Scenario*);
    void activationChanged();

  private:
    QString m_prefix;
    int m_inGroup;
    bool m_dirty;
    QString m_scenarioId;
    bool m_active;

    QDateTime m_lastModifiedDate;

    int m_version;

    QString m_name;
    QString m_iconSrc;
    QString m_license;
    QList<Author*> m_authors;
    VersionNumber *m_simonMinVersion, *m_simonMaxVersion;
    ActiveVocabulary *m_vocabulary;
    TrainingTextCollection *m_texts;
    Grammar *m_grammar;
    ActionCollection *m_actionCollection;
    CompoundCondition *m_compoundCondition;
    QStringList m_childScenarioIds;
    QList<Scenario*> m_childScenarios;
    Scenario* m_parentScenario;

    QStringList getValidSentences(QList< QList<Word*> > sentenceMatrix, int* alreadyFoundExamples=0);

    bool setupPathToParse(QString& path);
    bool setupToParse(QString& path, QDomDocument*& doc, bool& deleteDoc);
    static QDateTime utcTime();

    QDomElement serialize(QDomDocument doc, bool full);

  public:
    explicit Scenario(const QString& scenarioId, const QString& prefix=QString(), QObject *parent = 0);

    KIcon icon() { return KIcon(m_iconSrc); }
    QString iconSrc() { return m_iconSrc; }
    QString name() { return m_name; }
    int version() { return m_version; }
    QString id() { return m_scenarioId; }
    QString license() { return m_license; }
    VersionNumber* simonMinVersion() { return m_simonMinVersion; }
    VersionNumber* simonMaxVersion() { return m_simonMaxVersion; }
    QList<Author*> authors() { return m_authors; }
    QDateTime modifiedDate() { return m_lastModifiedDate; }
    bool isActive() {return m_active;}

    QString serialize(bool full);

    bool addWords(QList<Word*> w);
    bool addWord(Word* w);
    bool removeWord(Word* w);

    bool addStructures(const QStringList& newStructures);

    QList<Word*> findWords(const QString& name, Vocabulary::MatchType type);
    QList<Word*> findWordsByCategory(const QString& name);

    QStringList getAllPossibleSentences();
    QStringList getAllPossibleSentencesOfStructure(const QString& structure, int* alreadyFoundExamples=0);

    QStringList getExampleSentencesOfStructure(const QString& structure);

    QStringList getExampleSentences(const QString& name, const QString& category, int count);
    QString fillGrammarSentenceWithExamples(const QString& categorySentence, bool &ok, const QString& toDemonstrate=QString(),
      const QString& toDemonstrateCategory=QString());

    QStringList getCategories(SpeechModel::ModelElements elements);
    bool renameCategory(const QString& category, const QString& newName, SpeechModel::ModelElements affect);

    QString getRandomWord(const QString& category);
    bool containsWord(const QString& word);
    bool containsWord(const QString& word, const QString& category, const QString& pronunciation);

    static QDateTime skimDate(QString path=QString());
    bool skim(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool readLanguageModel(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool readActions(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool readTrainingsTexts(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool readCompoundCondition(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool readChildScenarioIds(QString path=QString(), QDomDocument* doc=0, bool deleteDoc=false);
    bool init(QString path=QString());
    bool create(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion,
      VersionNumber* simonMaxVersion, const QString& license, QList<Author*> authors);
    bool update(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion,
      VersionNumber* simonMaxVersion, const QString& license, QList<Author*> authors);

    bool setupChildScenarios();
    QList<Scenario*> childScenarios() const;
    QStringList childScenarioIds() const;
    Scenario* parentScenario();
    void setParentScenario(Scenario* parent);
    void setChildScenarioIds(QStringList ids);

    ActiveVocabulary* vocabulary() { return m_vocabulary; }
    Grammar* grammar() { return m_grammar; }
    TrainingTextCollection* texts() { return m_texts; }
    ActionCollection* actionCollection() { return m_actionCollection; }
    CompoundCondition* compoundCondition() {return m_compoundCondition;}

    bool removeText(TrainingText* text);
    bool addTrainingText(TrainingText* text);

    bool removeCommand(Command *command);

    bool setAutorun(bool active, const QString& type, const QString& trigger);
    bool autorunActive();
    QString autorunType();
    QString autorunTrigger();

    bool processResult(RecognitionResult recognitionResult);
    bool triggerCommand(const QString& type, const QString& trigger, bool silent);

    CommandList getCommandList();

    void startGroup();
    bool commitGroup();

    ~Scenario();

    void setPluginFont(const QFont& font);

    QString preferredPath() const;
    static QString createId(const QString& name);

    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();
    void setListInterfaceCommands(QHash<CommandListElements::Element, VoiceInterfaceCommand*> commands);

    static QString pathFromId(const QString& id, const QString& prefix=QString(), bool local=true);
    static QString idFromPath(const QString& path);

    /**
     * @warning Input file will *always* be modified, use temporary files!
     * @param isIntegrated true, if the scenario is already part of the Simon scenario folder
     */
    static QStringList explode(const QString& inFile, bool isIntegrated = false);

    static bool updateChildScenarioIds(const QString& id, const QStringList& ids);

  public slots:
    bool save(QString path=QString(), bool full = false, bool touchModifiedTime = true);
    void updateActivation();
};
#endif

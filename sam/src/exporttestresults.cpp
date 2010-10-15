/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "exporttestresults.h"
#include "reportparameters.h"
#include "corpusinformation.h"
#include "templatevaluelist.h"
#include "corpusinformationwidget.h"
#include "testresultwidget.h"
#include "testconfigurationwidget.h"
#include "version.h"
#include "latexreporttemplateengine.h"
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <QWidget>
#include <QDir>
#include <QFile>
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
#include <KTabWidget>
#include <KStandardDirs>
#include <KProcess>
#include <KDebug>
#include <KFileDialog>

ExportTestResults::ExportTestResults(QWidget *parent) : KDialog(parent)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);
  setMainWidget(w);
  setCaption(i18n("Export test results"));
  connect(ui.pbRetrieveSystemInformation, SIGNAL(clicked()), this, SLOT(initSystemDefinition()));
}

void ExportTestResults::initTemplates()
{
  QStringList templates;

  QStringList directories = KGlobal::dirs()->findDirs("data", "sam/reports/templates");
  QDir d;
  QStringList files;
  foreach (const QString& dir, directories)
  {
    if (!d.cd(dir)) continue;
    QStringList relFiles = d.entryList(QDir::NoDotAndDotDot|QDir::Files);
    foreach (const QString& f, relFiles)
      files << dir+f;
  }
  kDebug() << "Found templates: " << files;

  foreach (const QString& file, files)
  {
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly))
    {
      kDebug() << "Could not open file: " << file;
      continue;
    }

    QString name = QString::fromUtf8(f.readLine().trimmed());
    ui.cbOutputFormat->addItem(name, file);
  }
}

void ExportTestResults::clearCorpora()
{
  qDeleteAll(m_creationCorporaWidgets);
  m_creationCorporaWidgets.clear();
  qDeleteAll(m_testCorporaWidgets);
  m_testCorporaWidgets.clear();
}

void ExportTestResults::initSystemDefinition()
{
  QString systemInfo;
  systemInfo += i18n("sam: part of simon %1\n", simon_version);
  
  systemInfo += ModelCompilationManager::information(
      !ui.cbDetailedSystemInformation->isChecked() /*short version*/);

  ui.teSystemDefinition->setPlainText(systemInfo);
}

QHash<QString, QString> ExportTestResults::createTemplateValues()
{
  QHash<QString,QString> values;
  values.insert("title", ui.leTitle->text());
  values.insert("tag", ui.leTag->text());
  values.insert("taskDefinition", ui.teTaskDefinition->toPlainText());
  values.insert("conclusion", ui.teConclusion->toPlainText());
  values.insert("experimentTag", ui.leExperimentTag->text());
  values.insert("experimentDate", ui.dpExperimentDate->date().toString(Qt::DefaultLocaleShortDate));
  values.insert("experimentDateISO", ui.dpExperimentDate->date().toString(Qt::ISODate));
  values.insert("experimentDescription", ui.teExperimentDescription->toPlainText());
  values.insert("systemTag", ui.leSystemTag->text());
  values.insert("systemDefinition", ui.teSystemDefinition->toPlainText());
  values.insert("vocabularyTag", ui.leVocabularyTag->text());
  values.insert("vocabularyNotes", ui.teVocabularyNotes->toPlainText());
  values.insert("grammarTag", ui.leGrammarTag->text());
  values.insert("grammarNotes", ui.teGrammarNotes->toPlainText());
  values.insert("trainingsCorpusCount", QString::number(m_creationCorporaWidgets.count()));
  values.insert("testCorpusCount", QString::number(m_testCorporaWidgets.count()));
  return values;
}

TemplateValueList* ExportTestResults::extractCorpusTemplateInformation(const QString& id, QList<CorpusInformationWidget*> widgets)
{
  TemplateValueList *l = new TemplateValueList(id);
  foreach (CorpusInformationWidget* w, widgets)
  {
    QHash<QString,QString> values;
    CorpusInformation *info = w->information();
    values.insert("corpusTag", info->tag());
    values.insert("corpusNotes", info->notes());
    values.insert("corpusSpeakers", QString::number(info->speakers()));
    values.insert("corpusSamples", QString::number(info->samples()));
    l->add(values);
  }

  return l;
}

QList<TemplateValueList*> ExportTestResults::createTemplateValueLists()
{
  QList<TemplateValueList*> list;

  list << extractCorpusTemplateInformation("trainingCorpora", m_creationCorporaWidgets);
  list << extractCorpusTemplateInformation("testCorpora", m_testCorporaWidgets);

  return list;
}

void ExportTestResults::createReport()
{
  QString templateFile = getSelectedTemplate();
  QFile f(templateFile);
  if (!f.open(QIODevice::ReadOnly))
  {
    KMessageBox::sorry(this, i18n("Could not initialize output template at \"%1\". "
          "Please select a valid output format.", templateFile));
    return;
  }

  QString outputTemplateName = QString::fromUtf8(f.readLine().trimmed());
  QByteArray outputEngine = f.readLine().trimmed();

  //hardcode only currently available output engine
  if (outputEngine != "LATEX")
  {
    KMessageBox::sorry(this, i18n("Output type not supported: %1.\n\n"
          "Please make sure that your output format is compatible with this version of sam.", QString::fromUtf8(outputEngine)));
    return;
  }

  ReportTemplateEngine *engine = new LatexReportTemplateEngine;
  QByteArray templateData = f.readAll().trimmed();

  QString outputFile = KFileDialog::getSaveFileName(KUrl(), engine->fileType(), this);
  
  if (!outputFile.isEmpty() &&
      !engine->parse(templateData, createTemplateValues(),  createTemplateValueLists(),
      ui.cbTables->isChecked(), ui.cbGraphs->isChecked(), /*m_creationCorporaWidgets, m_testCorporaWidgets,
      m_testResults,*/ outputFile))
    KMessageBox::sorry(this, i18n("Failed to parse template: %1", engine->lastError()));
   
  delete engine;
}

int ExportTestResults::exec()
{
  return KDialog::exec();
}

void ExportTestResults::displayCorpora(KTabWidget* tableWidget, QList<CorpusInformationWidget*>& list,
    const QList<CorpusInformation*>& corpora)
{
  kDebug() << "Displaying corpora: " << corpora;
  foreach (CorpusInformation* corpus, corpora)
  {
    kDebug() << corpus->tag();
    CorpusInformationWidget *widget = new CorpusInformationWidget(corpus, this);
    tableWidget->addTab(widget, corpus->tag());
    list << widget;
  }
}

bool ExportTestResults::exportTestResults(ReportParameters *reportParameters, QList<CorpusInformation*> creationCorpora,
                            QList<TestResultWidget*> testResults)
{
  clearCorpora();
  initTemplates();
  this->testResults = testResults;
  if (reportParameters)
  {
    ui.leTitle->setText(reportParameters->title());
    ui.leTag->setText(reportParameters->tag());
    ui.teTaskDefinition->setPlainText(reportParameters->taskDefinition());
    ui.cbOutputFormat->setCurrentIndex(ui.cbOutputFormat->findData(reportParameters->outputTemplate()));
    ui.teConclusion->setPlainText(reportParameters->conclusion());
    ui.leExperimentTag->setText(reportParameters->experimentTag());
    ui.dpExperimentDate->setDate(reportParameters->experimentDate());
    ui.teExperimentDescription->setPlainText(reportParameters->experimentDescription());
    ui.leSystemTag->setText(reportParameters->systemTag());
    ui.teSystemDefinition->setPlainText(reportParameters->systemDefinition());
    ui.leVocabularyTag->setText(reportParameters->vocabularyTag());
    ui.teVocabularyNotes->setPlainText(reportParameters->vocabularyNotes());
    ui.leGrammarTag->setText(reportParameters->grammarTag());
    ui.teGrammarNotes->setPlainText(reportParameters->grammarNotes());

    ReportParameters::OutputOptions options = reportParameters->options();
    ui.cbTables->setChecked(options & ReportParameters::Tables);
    ui.cbGraphs->setChecked(options & ReportParameters::Graphs);
  }
  else
    initSystemDefinition();

  QList<CorpusInformation*> testCorpora;
  foreach (TestResultWidget* testResult, testResults)
    testCorpora << testResult->getConfiguration()->corpusInformation();

  kDebug() << "Exporting creation corpora: " << creationCorpora.count();
  displayCorpora(ui.twTrainingCorpora, m_creationCorporaWidgets, creationCorpora);
  displayCorpora(ui.twTestCorpora, m_testCorporaWidgets, testCorpora);

  if (exec())
  {
    createReport();
    return true;
  }
  return false;
}

QString ExportTestResults::getSelectedTemplate()
{
  return ui.cbOutputFormat->itemData(ui.cbOutputFormat->currentIndex()).toString();
}

QList<CorpusInformation*> ExportTestResults::getCorpusInformation(const QList<CorpusInformationWidget*>& widgets)
{
  QList<CorpusInformation*> infos;
  foreach (CorpusInformationWidget *w, widgets)
  {
    w->submit();
    infos << w->information();
  }
  return infos;
}

QList<CorpusInformation*> ExportTestResults::getTestCorpusInformation()
{
  return getCorpusInformation(m_testCorporaWidgets);
}

QList<CorpusInformation*> ExportTestResults::getTrainingCorpusInformation()
{
  return getCorpusInformation(m_creationCorporaWidgets);
}


ReportParameters* ExportTestResults::getReportParameters()
{
  ReportParameters::OutputOptions options = ReportParameters::None;
  if (ui.cbTables->isChecked())
    options |= ReportParameters::Tables;
  if (ui.cbGraphs->isChecked())
    options |= ReportParameters::Graphs;

  ReportParameters *reportParameters = new ReportParameters(ui.leTitle->text(), 
      ui.leTag->text(), ui.teTaskDefinition->toPlainText(),
      options, getSelectedTemplate(), ui.teConclusion->toPlainText(),
      ui.leExperimentTag->text(), ui.dpExperimentDate->date(),
      ui.teExperimentDescription->toPlainText(), ui.leSystemTag->text(),
      ui.teSystemDefinition->toPlainText(), ui.leVocabularyTag->text(),
      ui.teVocabularyNotes->toPlainText(), ui.leGrammarTag->text(), 
      ui.teGrammarNotes->toPlainText());
  return reportParameters;
}

void ExportTestResults::saveCorporaInformation()
{
  foreach (CorpusInformationWidget* w, m_testCorporaWidgets)
    w->submit();
  foreach (CorpusInformationWidget* w, m_creationCorporaWidgets)
    w->submit();

    //QList<CorpusInformationWidget*> m_creationCorporaWidgets;
    //QList<CorpusInformationWidget*> m_testCorporaWidgets;
  //Q_ASSERT(m_testCorporaWidgets == testResults.count());
//
  //int i=0;
  //foreach (CorpusInformationWidget* w, m_testCorporaWidgets)
  //{
    //TestResultWidget *testResult = testResults[i];
//
    //testResult->
    //i++;
  //}
  
}

ExportTestResults::~ExportTestResults()
{
  qDeleteAll(m_creationCorporaWidgets);
  qDeleteAll(m_testCorporaWidgets);
}


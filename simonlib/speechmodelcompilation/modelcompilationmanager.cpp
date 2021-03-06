/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "modelcompilationmanager.h"

#include <KStandardDirs>
#include <KDebug>
#include <QFile>
#include <QString>
#include <stdexcept>

ModelCompilationManager::ModelCompilationManager ( const QString& userName, QObject* parent ) :
    QThread ( parent ), keepGoing(false), userName(userName), compiler(0), adapter(0)
{
//  connect(compiler, SIGNAL(phonemeUndefined(QString)), this, SIGNAL(phonemeUndefined(QString)));
}

ModelCompilationManager::~ModelCompilationManager()
{
  abort();
  delete adapter;
  delete compiler;
}

void ModelCompilationManager::slotPhonemeUndefined ( const QString& phoneme )
{
  adapter->poisonPhoneme(phoneme);
  tryAgain = true;
}

QString ModelCompilationManager::cachedModelPath ( uint fingerprint, bool* exists )
{
  QString path = KStandardDirs::locateLocal("appdata", "models/"+userName+"/active/"+QString::number(fingerprint)+".sbm");
  if (exists)
    *exists = QFile::exists(path);
  return path;
}

void ModelCompilationManager::startModelCompilation ( int baseModelType, const QString& baseModelPath, const QStringList& scenarioPaths, const QString& promptsPathIn )
{
  this->baseModelType = baseModelType;
  this->baseModelPath = baseModelPath;
  this->scenarioPaths = scenarioPaths;
  this->promptsPathIn = promptsPathIn;

  keepGoing = true;

  start();
}

void ModelCompilationManager::abort()
{
  if (!keepGoing) return;
  keepGoing = false;

  if (adapter) adapter->abort();
  if (compiler) compiler->abort();

  if (!wait(3000)) {
    terminate();
    if (compiler) compiler->reset();
    wait();
  }
  emit modelCompilationAborted(ModelCompilation::Manual);
}

QString ModelCompilationManager::getBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->getBuildLog();
}
QString ModelCompilationManager::getGraphicBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->getGraphicBuildLog();
}
bool ModelCompilationManager::hasBuildLog() const
{
  Q_ASSERT(compiler);
  return compiler->hasBuildLog();
}

int ModelCompilationManager::pronunciationCount() const
{
  return adapter->pronunciationCount();
}
int ModelCompilationManager::sampleCount() const
{
  return adapter->sampleCount();
}

uint ModelCompilationManager::getFingerPrint(const QStringList& files, ModelCompiler::CompilationType compilationType)
{
  uint fingerprint(0);
  foreach (const QString& file, files)
  {
    qDebug() << "Analyzing file: " << file;
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly))
    {
      kDebug() << "Error building fingerprint";
      kDebug() << file;
//      emit modelCompilationAborted();
      //WARNING:&
//      throw std::runtime_error(qPrintable("Error building fingerprint. Can't open file " + file));
      abort();
    }
    fingerprint ^= qHash(f.readAll());
  }

  return fingerprint ^= compilationType;
}

ModelCompiler::CompilationType ModelCompilationManager::getCompilationType(int baseModelType)
{
  ModelCompiler::CompilationType compilationType;

  switch (baseModelType)
  {
    case 0:
      //static model
      compilationType = (ModelCompiler::CompileLanguageModel);
      break;
    case 1:
      //adapted base model
      compilationType = (ModelCompiler::CompilationType) (ModelCompiler::CompileLanguageModel|ModelCompiler::AdaptSpeechModel);
      break;

    default:
      //dynamic model
      compilationType = (ModelCompiler::CompilationType) (ModelCompiler::CompileLanguageModel|ModelCompiler::CompileSpeechModel);
      break;
  }

  return compilationType;
}

int ModelCompilationManager::wordCount() const
{
  return adapter->wordCount();
}

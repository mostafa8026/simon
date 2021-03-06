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

#include "modelcompiler.h"

#include <QDomDocument>
#include <QMetaType>
#include <QDateTime>
#include <KDebug>

bool ModelCompiler::hasBuildLog() const
{
  return (buildLog.count() > 0);
}

/**
 * \brief Gets a user-readable error and checks if we have not run into a common problem; Generates the final error and emits it using error()
 * \author Peter Grasch
 *
 * This method is kind of a proxy: It gets its error message from the compilation process itself. Such a message might be:
 * 	"Could not generate HMM13. Check paths to..."
 *
 * Before emitting that exakt error to the controlling process, we will check with processError which in turn will inspect the build protocol
 * to find common mistakes.
 * Only if processError does not know how to handle this error (i.e. not a common mistake) we will emit the given readableError.
 */
void ModelCompiler::analyseError(QString readableError)
{
  if (!keepGoing) return;

  if (!processError())
    emit error(readableError);
}

QString ModelCompiler::getGraphicBuildLog() const
{
  QString htmlLog = QString::fromLocal8Bit(buildLog);
  htmlLog=htmlLog.replace('\n', "<br />");
  return "<html><head /><body>"+htmlLog+"</body></html>";
}

QString ModelCompiler::getBuildLog() const
{
  QString plainLog = QString::fromLocal8Bit(buildLog);
  plainLog.remove("<p>");
  plainLog.replace("</p>", "\n\n");
  plainLog.remove("<span style=\"color:#aa0000;\">");
  plainLog.remove("<span style=\"font-weight:bold; color:#00007f;\">");
  plainLog.remove("<span style=\"font-weight:bold; color:#358914;\">");
  plainLog.remove("</span>");
  return plainLog;
}

void ModelCompiler::addStatusToLog(const QString& status)
{
  buildLogMutex.lock();
  buildLog.append("<p><span style=\"font-weight:bold; color:#358914;\">"+status.toLocal8Bit()+"</span></p>");
  buildLogMutex.unlock();
}

bool ModelCompiler::execute(const QString& command, const QString &wDir)
{
  if (!keepGoing)
    return false;
  kDebug() << command;
  kDebug() << wDir;
  QProcess proc;
  proc.setWorkingDirectory(wDir);
  proc.start(command);

  activeProcesses << &proc;

  buildLogMutex.lock();
  buildLog.append("<p><span style=\"font-weight:bold; color:#00007f;\">"+command.toLocal8Bit()+"</span></p>");
  buildLogMutex.unlock();

  kDebug() << "Waiting for process to finish..." << activeProcesses.count();
  proc.waitForFinished(-1);
  kDebug() << "Process finished..." << activeProcesses.count();

  activeProcesses.removeAll(&proc);
  kDebug() << "Process removed" << activeProcesses.count();

  QByteArray err = proc.readAllStandardError();
  QByteArray out = proc.readAllStandardOutput();

  proc.close();

  buildLogMutex.lock();
  if (!out.isEmpty())
  {
    buildLog.append("<p>"+out+"</p>");
    kDebug() << "Process output: " << out;
  }

  if (!err.isEmpty())
  {
    buildLog.append("<p><span style=\"color:#aa0000;\">"+err+"</span></p>");
    kDebug() << "Appended error: " << err;
  }
  buildLogMutex.unlock();

  if (proc.exitCode() != 0)
    return false;
  else return true;
}

void ModelCompiler::reset()
{
  clearLog();
  activeProcesses.clear();
}

void ModelCompiler::clearLog()
{
  buildLogMutex.lock();
  buildLog.clear();
  buildLogMutex.unlock();
}

void ModelCompiler::abort()
{
  kDebug() << "Compilation Manager Aborting.";

  keepGoing=false;

  foreach(QProcess *proc, activeProcesses)
  //tell any running process to commit suicide
    proc->kill();

  emit activeModelCompilationAborted(ModelCompilation::Manual);
}

ModelMetadata ModelCompiler::getMetaData(const QString &name, const QString &type)
{
  return ModelMetadata(name, QDateTime::currentDateTime(), type, m_droppedTranscriptions);
}


ModelCompiler::ModelCompiler ( const QString& userName, QObject* parent ) : QObject(parent), userName(userName)
{
  qRegisterMetaType<ModelCompilation::AbortionReason>("ModelCompilation::AbortionReason");
}

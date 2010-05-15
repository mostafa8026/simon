/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_SIMONSOUNDOUTPUT_H_BAC62651BE6A419EA6156220815A2AAD
#define SIMON_SIMONSOUNDOUTPUT_H_BAC62651BE6A419EA6156220815A2AAD

class QAudioOutput;

#include <simonsound/simonsound.h>
#include <QList>
#include <QIODevice>
#include <qaudio.h>

class SoundOutputClient;

class SimonSoundOutput : public QIODevice
{
	Q_OBJECT

	signals:
		void error(const QString& str);
		void outputStateChanged(QAudio::State state);

	private:
		QAudioOutput *m_output;
		SoundOutputClient* m_activeOutputClient;
		QList<SoundOutputClient*> m_suspendedOutputClients;

	protected:
		qint64 readData(char *toRead, qint64 maxLen);
		qint64 writeData(const char *toWrite, qint64 len);

	private slots:
		void slotOutputStateChanged(QAudio::State state);

	public:
		SimonSoundOutput(QObject *parent=NULL);
		~SimonSoundOutput();
		
		void registerOutputClient(SoundOutputClient* client);
		bool deRegisterOutputClient(SoundOutputClient* client);

		bool startPlayback(SimonSound::DeviceConfiguration& device);

		void suspendOutput();
		void resumeOutput();

		bool stopPlayback();
};

#endif



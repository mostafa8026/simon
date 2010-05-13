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

#include "singledevicesettings.h"

#include <KMessageBox>
#include <QAudioDeviceInfo>
#include <KIcon>

#include <KLocalizedString>
#include <KDebug>

#include "ui_singledeviceconfiguration.h"



SingleDeviceSettings::SingleDeviceSettings(SimonSound::SoundDeviceType type, QString deviceName, int channels, 
			int sampleRate, SimonSound::SoundDeviceUses uses, SimonSound::SoundDeviceOptions options, QWidget* parent):
		QWidget(parent),
	enabled(true),
	m_type(type),
	m_deviceName(deviceName),
	m_uses(uses),
	m_options(options)
{
	ui = new Ui::SingleDeviceConfiguration();

	ui->setupUi(this);

	ui->sbChannels->setValue(channels);
	ui->sbSampleRate->setValue(sampleRate);

	ui->pbTest->setIcon(KIcon("help-hint"));
	ui->pbRemove->setIcon(KIcon("list-remove"));
	
	if (!(options & SimonSound::Removable))
		ui->pbRemove->hide();
	
	connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(checkWithSuccessMessage()));
	connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(sendRemoveRequest()));
	connect(ui->cbSoundDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));

	connect(ui->sbChannels, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));
	connect(ui->sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(slotChanged()));
	connect(ui->cbTraining, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui->cbRecognition, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));

	load(deviceName, channels, sampleRate);
}


void SingleDeviceSettings::sendRemoveRequest()
{
	emit requestRemove(this);
}

void SingleDeviceSettings::checkWithSuccessMessage()
{
	if (!enabled) return;
	if (check())
		KMessageBox::information(this, i18n("The soundconfiguration has been tested successfully."));
}

void SingleDeviceSettings::refreshDevices()
{
	if (!getSelectedDeviceId().isEmpty())
		m_deviceName = getSelectedDeviceId();

	load(m_deviceName, getChannels(), getSampleRate());
}

void SingleDeviceSettings::load(QString deviceName, int channels, 
			int sampleRate)
{
	ui->cbSoundDevice->clear();

	foreach(const QAudioDeviceInfo &deviceInfo, (m_type == SimonSound::Input) ? QAudioDeviceInfo::availableDevices(QAudio::AudioInput) :
			QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		ui->cbSoundDevice->addItem(deviceInfo.deviceName());

	//select
	ui->cbSoundDevice->setCurrentIndex(ui->cbSoundDevice->findText(deviceName));

	ui->sbChannels->setValue(channels);
	ui->sbSampleRate->setValue(sampleRate);

	ui->cbRecognition->setChecked(m_uses & SimonSound::Recognition);
	ui->cbTraining->setChecked(m_uses & SimonSound::Training);

	bool hasChanged=false;
	if ((!deviceName.isEmpty()) &&
		(ui->cbSoundDevice->currentText() != deviceName))
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("simon noticed that the sound device \"%1\" is no longer available.\n\nThis is perfectly normal if you are connected to simond or are otherwise using an application that blocks the device.\n\nDid you plug / unplug a device or otherwise change your systems audio setup?\n\nSelecting \"Yes\" will allow you to change your sound configuration, essentially deleting your previous configuration. Selecting \"No\" will temporarily deactivate the sound configuration in order to protect your previous configuration from being overwritten.", deviceName)) == KMessageBox::Yes)
		{
			ui->cbSoundDevice->setCurrentIndex(
					ui->cbSoundDevice->findText(
						(m_type == SimonSound::Input) ? QAudioDeviceInfo::defaultInputDevice().deviceName() :
						QAudioDeviceInfo::defaultOutputDevice().deviceName()));
			hasChanged=true;
			KMessageBox::information(this, i18n("Please adjust your soundconfiguration accordingly."));
			enable();
		} else disable();

	} else enable();

	if (hasChanged) emit changed(true);
}

void SingleDeviceSettings::enable()
{
	ui->frmDevice->setEnabled(true);
	enabled=true;
}

void SingleDeviceSettings::disable()
{
	ui->frmDevice->setEnabled(false);
	enabled=false;
}


bool SingleDeviceSettings::check()
{
	if (!enabled) return true;

	QString device = getSelectedDeviceId();
	int channels = getChannels();
	int samplerate = getSampleRate();

	bool ok = true;
	QAudioFormat format;
	format.setFrequency(samplerate);
	format.setChannels(channels);
	format.setSampleSize(16); // 16 bit
	format.setSampleType(QAudioFormat::SignedInt); // SignedInt currently
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setCodec("audio/pcm");

	foreach(const QAudioDeviceInfo &deviceInfo, (m_type == SimonSound::Input) ? QAudioDeviceInfo::availableDevices(QAudio::AudioInput) :
			QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		if (deviceInfo.deviceName() == device)
		{
			kDebug() << "Checking device: " << device;
			if (!deviceInfo.isFormatSupported(format))
			{
				ok = false;
				break;
			}
		}
	}

	if (!ok)
		KMessageBox::error(this, i18n("The selected sound configuration is not supported by the following device:\n%1\n\nPlease double-check your configuration and, if necessairy, please contact your vendor.", device));

	return ok;
}

bool SingleDeviceSettings::isEnabled()
{
	return enabled;
}
	
QString SingleDeviceSettings::getSelectedDeviceId()
{
	if (!isEnabled())
		return m_deviceName;

	return ui->cbSoundDevice->currentText();
}

int SingleDeviceSettings::getSampleRate()
{
	return ui->sbSampleRate->value();
}

int SingleDeviceSettings::getChannels()
{
	return ui->sbChannels->value();
}

SimonSound::SoundDeviceType SingleDeviceSettings::getType()
{
	return m_type;
}

SimonSound::SoundDeviceUses SingleDeviceSettings::getUses()
{
	SimonSound::SoundDeviceUses uses = SimonSound::None;
	if (ui->cbRecognition->isChecked())
		uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Recognition);
	if (ui->cbTraining->isChecked())
		uses = (SimonSound::SoundDeviceUses) (uses|SimonSound::Training);

	return uses;
}

void SingleDeviceSettings::slotChanged()
{
	emit changed(true);
}


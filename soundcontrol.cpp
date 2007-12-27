//
// C++ Implementation: soundcontrol
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "soundcontrol.h"
#include <string.h>
#include "simoninfo.h"
#include "logger.h"
#include <QObject>
/**
 *	@brief Constructor
 *
 *	Creates a new Soundbackend
 *
 *	@author Peter Grasch
 */
SoundControl::SoundControl()
{
	try
	{
		audio = new RtAudio();
	}
	catch ( RtError &error )
	{
		error.printMessage();
		exit ( EXIT_FAILURE );
	}
}



/**
 *	@brief Returns the current volume in percent
 *
 *	@author Peter Grasch
 *	@return int
 *	volume in percent
*/
int SoundControl::getVolume()
{
	return 0;
}

/**
 *	@brief Sets the volume in percent
 *
 *	@author Peter Grasch
 *	@param int percent
 *	volume in percent
*/


void SoundControl::setVolume ( int percent )
{
}

/**
 *	@brief Sets the volume in percent
 *
 *	@author Gigerl Martin
 *	@param int percent
 *	returns the aviable sound devices
*/

SoundDeviceList* SoundControl::getOutputDevices()
{
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verf�gbaren devices"));
	SoundDeviceList *sdl= new SoundDeviceList();

	// Determine the number of devices available
	int devices = audio->getDeviceCount();

	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{

		try
		{
			info = audio->getDeviceInfo ( i );
			if ( info.outputChannels>0 )
			{
				sdl->append ( SoundDevice ( QString::number ( i ),QString ( info.name.c_str() ) ) );
			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}

	return sdl;
}

/**
 * \brief Gets the available samplerates of the device given by id
 * \author Gigerl Martin
 * @param id the id of the device
 * @return The possible samplerates for that device
 */
QList<int>* SoundControl::getSamplerate ( QString id )
{
	Logger::log(QObject::tr("[INF] Bestimmen der Samplerate des device \"")+id+"\"");
	// Determine the number of devices available
	int devices = audio->getDeviceCount();
	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( QString ( info.name.c_str() ) ==id )
			{
				QList<int>* temp= new QList<int>();

				for ( unsigned int i=0; i<info.sampleRates.size();i++ )
				{
					temp->append ( info.sampleRates.at ( i ) );
				}
				return temp;


			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}
	return NULL;
}


/**
 * \brief Gets the available channels for the device identified by the given id
 * @param id the id of the device
 * @author Gigerl Martin
 * @return Available channels
 */
int SoundControl::getChannel ( QString id )
{
	Logger::log(QObject::tr("[INF] Zur�ckgeben verf�gbarer Kan�le des device \"")+id+"\"");
	// Determine the number of devices available
	int devices = audio->getDeviceCount();
	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( QString ( info.name.c_str() ) ==id )
			{
				return info.inputChannels;

			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}
	return 0;

}


/**
 * \author Gigerl Martin
 * \brief Returns the available input devices
 * @return The list of found Sounddevices
 */
SoundDeviceList* SoundControl::getInputDevices()
{
	Logger::log(QObject::tr("[INF] Bekommen einer Liste mit den verf�gbaren Input-devices"));
	SoundDeviceList *sdl= new SoundDeviceList();

	// Determine the number of devices available
	int devices = audio->getDeviceCount();

	// Scan through devices for various capabilities
	RtAudioDeviceInfo info;
	for ( int i=1; i<=devices; i++ )
	{
		try
		{
			info = audio->getDeviceInfo ( i );
			if ( info.inputChannels>0 )
			{
				sdl->append ( SoundDevice ( QString::number ( i ),QString ( info.name.c_str() ) ) );
			}
		}
		catch ( RtError &error )
		{
			error.printMessage();
			break;
		}
	}


	return sdl;

}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
*/
SoundControl::~SoundControl()
{
// 	delete this->soundbackend;
// 	delete in_audio;
// 	delete out_audio;
}

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

#include <QString>
// this needs to be included first because the 
#include <KMessageBox>	// X11 headers included in the xevents header define "Status"

#include "xeventsprivate.h"
// #include "../Logging/logger.h"
#include <KLocalizedString>

XEventsPrivate::XEventsPrivate(char* displayName)
{
	display = openDisplay(displayName);
}

/**
 * @brief Opens the Display and returns the handle
 *
 * @param int keycombination
 * The keycombination
 * 
 * @author Peter Grasch
 */
Display* XEventsPrivate::openDisplay(char* displayName)
{
	int Event, Error;
	int Major, Minor;
	
// 	Logger::log(i18n("[INF] Öffne display \"%1\"", QString(displayName)));

	Display * display = XOpenDisplay(displayName);

	if (!display) {
// 		Logger::log(i18n("[ERR] Fehler beim öffnen des display \"%1\"", QString(displayName)));
		KMessageBox::error(0,i18n("Konnte Display nicht öffnen. Bitte überprüfen Sie ihre Konfiguration und / oder setzen Sie sich mit den simon-Entwickler in Verbindung. (Display: \"%1\")", QString(XDisplayName ( displayName ))));
		return NULL;
	}

	//check whether the XTest extension is installed
	if ( !XTestQueryExtension(display, &Event, &Error, &Major, &Minor) ) {
// 		Logger::log("[ERR] Display "+QString(displayName)+" unterstützt XTest nicht");
		KMessageBox::error(0,i18n("Der X-Server unterstützt die \"XTest\" nicht - bitte installieren Sie diese. (Display: \"%1\")", QString(DisplayString(display))));

		XCloseDisplay(display);
		return NULL;
	}


	//The following should be logged somewhere... Interresting for debugging purposes...
	//We'll do that once we have the logging classes...
// 	Logger::log(i18n("[INF] XTest für Server \"%1\" ist Version %2.%3", QString(DisplayString(display)), Major, Minor));

// 	Logger::log(i18n("[INF] Aufnahme der Display-Kontrolle"));
	XTestGrabControl( display, True ); 
// 	Logger::log(i18n("[INF] Synchronisiere Display"));
	XSync( display,True ); 
	return display;
}

/**
 * \brief Clicks the coordinates with a simple LMB-Click and release
 * \author Peter Grasch
 * @param x The x coordinate
 * @param y The y coordinate
 */
void XEventsPrivate::click(int x, int y)
{
	if (!display) return;
	XTestFakeMotionEvent(display, 0, x, y, 10);
	XTestFakeButtonEvent(display, 1, true, 10);
	XTestFakeButtonEvent(display, 1, false, 10);
	XFlush(display);
}

/**
 * \brief Resolves the string to an appropriate keysym and sends it using pressKey(...)
 * \author Peter Grasch
 * @param keysymString The string to convert/send
 * @see sendKey()
 */
void XEventsPrivate::sendKeySymString(const QString& keysymString)
{
	pressKey(XStringToKeysym(keysymString.toAscii().data()));
}


/**
 * \brief Sends the raw unicode character given by key
 * \author Peter Grasch
 * @param key The key to send
 */
void XEventsPrivate::sendKey(unsigned int key /*unicode*/)
{
	if (!display) return;
	KeyCode keyCode = XKeysymToKeycode(display, key);
	
	if (keyCode)
	{
		KeySym shiftSym = XKeycodeToKeysym(display, keyCode, 1);
		KeySym altGrSym = XKeycodeToKeysym(display, keyCode, 2);
		
		if (shiftSym == key)
			setModifierKey(Qt::SHIFT);
		else if (altGrSym == key)
			setModifierKey(Qt::Key_AltGr);
		
		pressKeyCode(keyCode);
		
		if (shiftSym == key)
			unsetModifier(Qt::SHIFT);
		else if (altGrSym == key)
			unsetModifier(Qt::Key_AltGr);
	} else {
		QKeySequence k(key); //do some magic
		pressKey(XStringToKeysym(k.toString().toUtf8().data())); //this was: toLatin1(); TEST THIS!
	}
}

void XEventsPrivate::pressKey(const KeySym& key)
{
	pressKeyCode(XKeysymToKeycode(display, key));
}

void XEventsPrivate::pressKeyCode(const KeyCode& code)
{
	XTestFakeKeyEvent(display, code, True, 15);
	XTestFakeKeyEvent(display, code, False, 15);
	XFlush ( display );
}


/**
 * @brief Sets the modifier key
 *
 * @param int virtualKey
 * The keycode (see Qts Key)
 * 
 * @author Peter Grasch
 */
void XEventsPrivate::setModifierKey(int virtualKey)
{
	switch (virtualKey)
	{
		case Qt::SHIFT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 5);
			break;
			
		case Qt::Key_AltGr:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), True, 5);
			break;
			
		case Qt::CTRL:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, 5);
			break;
			
		case Qt::ALT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), True, 5);
			break;
			
		case Qt::META:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), True, 5);
			break;
	}
	
	XFlush ( display );
}

/**
 * @brief Un-Set the modifier
 *
 * @param int virtualKey
 * The keycode
 * 
 * @author Peter Grasch
 */
void XEventsPrivate::unsetModifier(int virtualKey)
{
	switch (virtualKey)
	{
		case Qt::SHIFT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 5);
			break;
		
		case Qt::Key_AltGr:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_ISO_Level3_Shift), False, 5);
			break;
		
		case Qt::CTRL:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, 5);
			break;
		
		case Qt::ALT:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Alt_L), False, 5);
			break;
		
		case Qt::META:
			XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Super_L), False, 5);
			break;
	}
	XFlush ( display );
}



XEventsPrivate::~XEventsPrivate()
{
	XCloseDisplay(display);
	delete display;
}

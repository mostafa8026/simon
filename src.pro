# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  ./bin/simon

unix {
    HEADERS += runlinuxbackend.h xevents.h 
    SOURCES += xevents.cpp runlinuxbackend.cpp
    CONFIG += x11
    DEFINES += __LINUX_ALSA__
    INCLUDEPATH += /usr/include/alsa \
		/usr/X11R6/include
    LIBS += \
	-L/usr/X11R6/lib \
	-L/usr/lib/alsa-lib \
	-lX11 \
	-lXtst \
	-lasound \
	-lpthread
}

win32 {
    HEADERS += runwindowsbackend.h
    SOURCES += runwindowsbackend.cpp
    DEFINES += __WINDOWS_DS__
    LIBS += -ldsound -lwinmm -lole32
}


FORMS += ui/main.ui \
         ui/wordlist.ui \
         ui/rundialog.ui \
         ui/addword.ui \
         ui/settings.ui \
         ui/trainmain.ui \
         ui/importdict.ui
HEADERS += simonview.h \
           addwordview.h \
           simoncontrol.h \
           simoninfo.h \
           wordlistview.h \
           trainingview.h \
           juliuscontrol.h \
           wordlistmanager.h \
           command.h \
           modelmanager.h \
           runcommand.h \
           trainingmanager.h \
           trainingtext.h \
           word.h \
           trayiconmanager.h \
	   RtAudio.h \
	   RtError.h \
           osd.h \
           dragtablewidget.h \
           droplistwidget.h \
           runapplicationview.h \
           runbackend.h \
           vumeter.h \
           xmlreader.h \
           eventhandler.h \
           coreevents.h \
           settingsview.h \
           wav.h \
           xmlcommand.h \
           xmltrainingtext.h \
           xmlsetting.h \
           wavrecorder.h \
           wavplayer.h \
	   sounddevice.h \
           soundcontrol.h \
           importdictview.h \
           importdict.h \
           wiktionarydict.h \
           dict.h \
           xmldomreader.h \
           xmlsaxreader.h \
           importtrainingtexts.h \
	   recwidget.h \
 quickdownloader.h \
 xmltrainingtextlist.h \
 importlocalwizardpage.h \
 selectsourcewizardpage.h \
 importremotewizardpage.h \
 importworkingwizardpage.h \
 logger.h
SOURCES += main.cpp \
           simonview.cpp \
	   RtAudio.cpp \
           addwordview.cpp \
           simoncontrol.cpp \
           simoninfo.cpp \
           wordlistview.cpp \
           trainingview.cpp \
           juliuscontrol.cpp \
           wordlistmanager.cpp \
           modelmanager.cpp \
           runcommand.cpp \
           trainingmanager.cpp \
           trainingtext.cpp \
           trayiconmanager.cpp \
           osd.cpp \
           dragtablewidget.cpp \
           droplistwidget.cpp \
           runapplicationview.cpp \
           vumeter.cpp \
           eventhandler.cpp \
           settingsview.cpp \
           wav.cpp \
		xmlreader.cpp \
           xmlcommand.cpp \
           xmltrainingtext.cpp \
           xmlsetting.cpp \
           wavrecorder.cpp \
           wavplayer.cpp \
           soundcontrol.cpp \
           importdictview.cpp \
           importdict.cpp \
           wiktionarydict.cpp \
           dict.cpp \
           xmldomreader.cpp \
           xmlsaxreader.cpp \
           importtrainingtexts.cpp \
	   recwidget.cpp \
 quickdownloader.cpp \
 xmltrainingtextlist.cpp \
 importlocalwizardpage.cpp \
 selectsourcewizardpage.cpp \
 importremotewizardpage.cpp \
 importworkingwizardpage.cpp \
 logger.cpp

QT += network \
xml 

TARGET = ./bin/simon
TEMPLATE = app

CONFIG += qt \
warn_on \
thread \
debug


RESOURCES += simon.qrc

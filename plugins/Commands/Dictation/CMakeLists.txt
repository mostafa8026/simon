set (simondictationcommandplugin_SRCS 
  dictationcommandmanager.cpp 
  dictationconfiguration.cpp
)

kde4_add_ui_files(simondictationcommandplugin_SRCS dictationconfigurationdlg.ui)

kde4_add_plugin(simondictationcommandplugin ${simondictationcommandplugin_SRCS})

target_link_libraries(simondictationcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenarios eventsimulation simonscenariobase)

install(TARGETS simondictationcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddictationplugin)
install(FILES simondictationcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddictationplugin)

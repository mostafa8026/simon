set( webcamsettings_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/simonwebcamconfiguration.cpp
        webcamconfigurationanalyzer.cpp
)

kde4_add_ui_files( webcamsettings_LIB_SRCS
        simonwebcamconfiguration.ui
)

kde4_add_kcfg_files(webcamsettings_LIB_SRCS webcamconfiguration.kcfgc)

kde4_add_plugin( webcamsettings  ${webcamsettings_LIB_SRCS} )
target_link_libraries( webcamsettings ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${OpenCV_LIBS} simonvision)

install( TARGETS webcamsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore )
install( FILES simonwebcamconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore )

set(simonjsonconnector_LIB_SRCS
  simonjsonconnector.cpp
  jsonnotificationreceiver.cpp
)

set(simonjsonconnector_LIB_HDRS
  simonjsonconnector_export.h
  simonjsonconnector.h
)

kde4_add_library(simonjsonconnector SHARED ${simonjsonconnector_LIB_SRCS})
target_link_libraries(simonjsonconnector ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

set_target_properties(simonjsonconnector
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonjsonconnector_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/jsonconnector
  COMPONENT simoncoredevel
)
 
install(TARGETS simonjsonconnector DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)

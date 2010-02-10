find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DTMPDIR="${CMAKE_INSTALL_PREFIX}/tmp")
add_definitions(-DPRGDIR="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DWITH_GUI_SUPPORT)
include_directories(.
        ${PROJECT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/src/semnetgui
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtgui
        ${QT_INCLUDE_DIR})

set(headers maingui.h)

set(sources main.cpp maingui.cpp)

set(data gda.app cleanup.xpm   filenew.xpm   filesave.xpm  generalization.xpm  start.png
compound.xpm  fileopen.xpm  gda.xpm       mini-gda.xpm        start.xpm)

qt_wrap_cpp(moclib mocsrc ${headers})

add_executable(gda ${sources} ${mocsrc})


target_link_libraries(gda semnetgui core qtgui qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS gda DESTINATION bin)
install(FILES ${data} DESTINATION share/data/application)

add_custom_command(TARGET gda POST_BUILD
                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/src/share/data/application
                   COMMAND cp -pR ${data} ${PROJECT_BINARY_DIR}/src/share/data/application
		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

add_definitions(-DTMPDIR="${CMAKE_INSTALL_PREFIX}/tmp")
add_definitions(-DPRGDIR="${CMAKE_INSTALL_PREFIX}")
include_directories(.
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtgui
        ${QT_INCLUDE_DIR})

set(sources GadBool.cpp WildcardReplace.cpp MainGui.cpp main.cpp
  GadWorkspace.cpp GadText.cpp GadProcess.cpp GadObject.cpp GadModule.cpp
  GadIO.cpp GadGuiObjectTable.cpp GadGui.cpp GadEnum.cpp ClassDict.cpp )


set(headers ClassDict.h GadEnum.h GadIO.h GadObject.h GadText.h MainGui.h
  GadBool.h GadGui.h GadModule.h GadProcess.h GadWorkspace.h WildcardReplace.h)

set(data gad.app)

qt_wrap_cpp(moclib mocsrc ${headers})

add_executable(gad ${sources} ${mocsrc})


target_link_libraries(gad qtgui qtparser  ${QT_LIBRARIES})

install(TARGETS gad DESTINATION bin)
install(FILES ${data} DESTINATION share/data/application)

add_custom_command(TARGET gad POST_BUILD
                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/share/data/application
                   COMMAND cp -pR ${data} ${PROJECT_BINARY_DIR}/share/data/application
		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

install(FILES modules.mod DESTINATION share/data/modules)

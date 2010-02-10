find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DPRGDIR="${CMAKE_INSTALL_PREFIX}")

include_directories(.
#  ${PROJECT_SOURCE_DIR}/src/bottomup
  ${CMAKE_SOURCE_DIR}/pfm/libpfm
  ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
  ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtgui
  ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
  ${QT_INCLUDE_DIR}
  ) 

set(headers gnode.h maingui.h mapviewer.h propertyeditor.h
  propertyitem.h treeeditor.h treeitem.h)

set(sources gnode.cpp maingui.cpp mapviewer.cpp plm_view.cpp
  propertyeditor.cpp propertyitem.cpp treeeditor.cpp treeitem.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

set(data plm_view.mod)

set(appdata plm_view.app)

add_executable(plm_view ${sources} ${mocsrc})

target_link_libraries(plm_view gaimage qtgui qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS plm_view DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)
install(FILES ${appdata} DESTINATION share/data/application)

add_custom_command(TARGET plm_view POST_BUILD
                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/src/share/data/application
                   COMMAND cp -pR ${appdata} ${PROJECT_BINARY_DIR}/src/share/data/application
		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


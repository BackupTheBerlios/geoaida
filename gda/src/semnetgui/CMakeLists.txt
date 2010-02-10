find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DTMPDIR="${CMAKE_INSTALL_PREFIX}/tmp")
add_definitions(-DWITH_GUI_SUPPORT)
include_directories(.
        ${PROJECT_SOURCE_DIR}/src/core
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtgui
        ${QT_INCLUDE_DIR})

set(headers formbasenode.h
           formeditwidget.h
           formoperand.h
           formoperator.h
           formviewwidget.h
           mapviewer.h
           propertyeditor.h
           propertyitem.h
           taskviewer.h
           treeeditor.h
           treeitem.h)

set(sources 
           formbasenode.cpp
           formeditwidget.cpp
           formoperand.cpp
           formoperator.cpp
           formviewwidget.cpp
           mapviewer.cpp
           propertyeditor.cpp
           propertyitem.cpp
           taskviewer.cpp
           treeeditor.cpp
           treeitem.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

add_library(semnetgui ${sources} ${mocsrc})

install(TARGETS semnetgui DESTINATION lib)

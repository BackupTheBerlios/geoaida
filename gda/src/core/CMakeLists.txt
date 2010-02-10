find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DTMPDIR="${CMAKE_INSTALL_PREFIX}/tmp")
add_definitions(-DWITH_GUI_SUPPORT)
add_definitions(-DDEBUG_MSG)

include_directories(.
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
	${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtgui
        ${QT_INCLUDE_DIR})

set(headers analysis.h
            attribute.h
            cleanup.h
            fatalerror.h
            geoimage.h
            geoimagecache.h
            geoimagelist.h
	    filelist.h
            gnode.h
            inode.h
            judge.h
            operator.h
            operatorlist.h
            semnet.h
            snode.h
            task.h)

set(sources 
	   analysis.cpp
           attribute.cpp
           cleanup.cpp
           fatalerror.cpp
           geoimage.cpp
           geoimagecache.cpp
           geoimagelist.cpp
	   filelist.cpp
           gnode.cpp
           inode.cpp
           judge.cpp
           operator.cpp
           operatorlist.cpp
           semnet.cpp
           snode.cpp
           task.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

add_library(core ${sources} ${mocsrc})

install(TARGETS core DESTINATION lib)

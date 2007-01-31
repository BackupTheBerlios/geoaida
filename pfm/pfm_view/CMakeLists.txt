find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR}/libpfm ${QT_INCLUDE_DIR})

set(headers combislider.h 
	doubleslider.h 
	myspinbox.h 
	paintwidget.h 
	picwin.h 
	plot2d.h)

set(sources combislider.cpp 
	doubleslider.cpp 
	main.cpp
	myspinbox.cpp 
	paintwidget.cpp 
	picwin.cpp 
	plot2d.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

add_executable(pfm_view ${sources} ${mocsrc})

target_link_libraries(pfm_view pfm ${QT_LIBRARIES})

install(TARGETS pfm_view DESTINATION bin)

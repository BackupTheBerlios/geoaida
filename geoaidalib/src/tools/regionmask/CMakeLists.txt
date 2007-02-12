find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(.
        ${PROJECT_SOURCE_DIR}/src/bottomup
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
        ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
	${QT_INCLUDE_DIR}
	)

set(sources regionmask.cpp)

set(data regionmask.mod)

add_executable(regionmask ${sources})
target_link_libraries(regionmask gaimage qtparser pfm pnm ${QT_LIBRARIES})

add_executable(watch_pipe watch_pipe.c)

install(TARGETS regionmask watch_pipe DESTINATION bin)
install(FILES ga_regionmask_pipe DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)


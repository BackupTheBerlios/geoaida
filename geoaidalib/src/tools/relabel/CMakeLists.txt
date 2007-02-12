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

set(sources relabel.cpp)

set(data relabel.mod)

add_executable(relabel ${sources})
target_link_libraries(relabel gaimage qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS relabel DESTINATION bin)
#install(FILES ${data} DESTINATION share/data/modules)


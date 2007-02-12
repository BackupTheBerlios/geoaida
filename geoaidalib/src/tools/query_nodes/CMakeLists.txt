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

set(sources query_nodes.cpp)

set(data query_nodes.mod)

add_executable(query_nodes ${sources})
target_link_libraries(query_nodes gaimage qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS query_nodes DESTINATION bin)
#install(FILES ${data} DESTINATION share/data/modules)


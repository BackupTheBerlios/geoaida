find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(.
        ${PROJECT_SOURCE_DIR}/src/bottomup
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
        ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
	${QT_INCLUDE_DIR}
	)

macro(bu_tool name extra_sources)
  set(sources ${name}.cpp ${extra_sources})

  set(data ${name}.op)

  add_executable(${name} ${sources} ${mocsrc})


  target_link_libraries(${name} bottomup gaimage qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS ${name} DESTINATION bin)
install(FILES ${data} DESTINATION share/data/operators)

#add_custom_command(TARGET ${name} POST_BUILD
#                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/src/share/data/application
#                   COMMAND cp -a ${data} ${PROJECT_BINARY_DIR}/src/share/data/application
#		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

endmacro(bu_tool)


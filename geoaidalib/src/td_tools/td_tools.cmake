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

macro(td_tool name extra_sources)
  set(sources ${name}.cpp ${extra_sources})

  set(data ${name}.op)

  add_executable(${name}_tool OUTPUT_NAME ${name} ${sources} ${mocsrc})


  target_link_libraries(${name}_tool gaimage pfm pnm ${QT_LIBRARIES})

  install(TARGETS ${name}_tool DESTINATION bin)
  install(FILES ${data} DESTINATION share/data/operators)
endmacro(td_tool)

macro(mod_tool name extra_sources)
  set(sources ${name}.cpp ${extra_sources})

  set(data ${name}.mod)

  add_executable(${name}_mod OUTPUT_NAME ${name} ${sources} ${mocsrc})


  target_link_libraries(${name}_mod gaimage pfm pnm ${QT_LIBRARIES})

  install(TARGETS ${name}_mod DESTINATION bin)
  install(FILES ${data} DESTINATION share/data/modules)
endmacro(mod_tool)

macro(td_tool_script name)
  install(FILES ${name}.op DESTINATION share/data/operators)
  install(PROGRAMS ${name} DESTINATION bin)
endmacro(td_tool_script)


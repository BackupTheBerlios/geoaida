find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(.
  ${PROJECT_SOURCE_DIR}/src/bottomup
  ${CMAKE_SOURCE_DIR}/pfm/libpfm
  ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
  ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
  ${QT_INCLUDE_DIR}
  )

set(name ga_td_symbolize)
set(sources ${name}.cpp)

set(data ${name}.mod)

add_executable(${name} ${sources})

target_link_libraries(${name} bottomup gaimage qtparser pfm pnm ${QT_LIBRARIES})

install(TARGETS ${name} DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)


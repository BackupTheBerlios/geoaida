include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_gaussian.cpp)

set(data gi_gaussian.mod)

add_executable(gi_gaussian ${sources})

target_link_libraries(gi_gaussian gaimage pfm pnm)

install(TARGETS gi_gaussian DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)

include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_arithmetic.cpp)

set(data gi_arithmetic.mod)

add_executable(gi_arithmetic ${sources})

target_link_libraries(gi_arithmetic gaimage pfm pnm)

install(TARGETS gi_arithmetic DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)

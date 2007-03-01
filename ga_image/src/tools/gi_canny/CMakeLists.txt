include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_canny.cpp)

set(data gi_canny.mod)

add_executable(gi_canny ${sources})

target_link_libraries(gi_canny gaimage pfm pnm)

install(TARGETS gi_canny DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)

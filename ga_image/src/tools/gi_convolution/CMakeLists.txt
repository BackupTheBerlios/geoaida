include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_convolution.cpp)

set(data gi_convolution.mod)

add_executable(gi_convolution ${sources})

target_link_libraries(gi_convolution gaimage pfm pnm fftw3)

install(TARGETS gi_convolution DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)

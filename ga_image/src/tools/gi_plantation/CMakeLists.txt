include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_plantation.cpp thresholding.cpp segmentation.cpp meanDistance.cpp)

set(data gi_plantation.mod)
set(treemasks treemasks/treemask0.ppm)

add_executable(gi_plantation ${sources})

target_link_libraries(gi_plantation gaimage pfm pnm)

install(TARGETS gi_plantation DESTINATION bin)
install(FILES ${treemasks} DESTINATION bin/treemasks)
install(FILES ${data} DESTINATION share/data/modules)

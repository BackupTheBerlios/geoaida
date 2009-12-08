add_definitions(${QT_DEFINITIONS})

include_directories(.
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
)

set(sources main.cpp)

add_executable(label2regiondescr ${sources})
target_link_libraries(label2regiondescr gaimage pfm pnm)


install(TARGETS label2regiondescr DESTINATION bin)


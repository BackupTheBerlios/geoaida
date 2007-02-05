include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_resample.cpp)

set(data gi_resample.mod)

add_executable(gi_resample ${sources})

target_link_libraries(gi_resample gaimage pfm pnm)

install(TARGETS gi_resample DESTINATION bin)
install(FILES ${data} DESTINATION share/data/modules)

#add_custom_command(TARGET gi_resample POST_BUILD
#                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/src/share/data/modules
#                   COMMAND cp -a ${data} ${PROJECT_BINARY_DIR}/src/share/data/modules
#		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

include_directories(.
        ${PROJECT_SOURCE_DIR}/src/gaimage
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
)

set(sources gi_merge.cpp)

#set(data gi_merge.mod)

add_executable(gi_merge ${sources})

target_link_libraries(gi_merge gaimage pfm pnm)

install(TARGETS gi_merge DESTINATION bin)
#install(FILES ${data} DESTINATION share/data/modules)

#add_custom_command(TARGET gi_merge POST_BUILD
#                   COMMAND mkdir -p ${PROJECT_BINARY_DIR}/src/share/data/modules
#                   COMMAND cp -a ${data} ${PROJECT_BINARY_DIR}/src/share/data/modules
#		   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

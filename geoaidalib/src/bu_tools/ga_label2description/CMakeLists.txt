include(../bu_tools.cmake)

set(sources ga_label2description.cpp)
add_executable(ga_label2description ${sources})
target_link_libraries(ga_label2description bottomup gaimage qtparser pfm pnm ${QT_LIBRARIES})
install(TARGETS ga_label2description DESTINATION bin)



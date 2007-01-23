set(sources pfm.c)
add_library(pfm STATIC ${sources})
install(TARGETS pfm DESTINATION lib)

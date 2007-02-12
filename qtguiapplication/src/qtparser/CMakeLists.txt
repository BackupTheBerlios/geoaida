find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/src/qtparser ${QT_INCLUDE_DIR})

set(headers MLParser.h)

set(sources MLParser.cpp)

qt_wrap_cpp(moclib mocsrc ${headers})

add_library(qtparser ${sources} ${mocsrc})

install(TARGETS qtparser DESTINATION lib)
install(FILES ${headers} DESTINATION include)
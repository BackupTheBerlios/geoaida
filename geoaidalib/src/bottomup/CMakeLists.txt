find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(.
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
        ${CMAKE_SOURCE_DIR}/qtguiapplication/src/qtparser
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
	${QT_INCLUDE_DIR}
	)
set(headers bottomup.h
           garelation.h
           gasensor.h
           neighbour.h
           node.h
           nodelist.h
           point2d.h
           regionsensor.h
           stack.h
           stackelem.h
           stackelemnodelist.h
           stackelemnumber.h
           stackelemstack.h
           stackelemstring.h
)

set(sources  bottomup.cpp
           garelation.cpp
           gasensor.cpp
           neighbour.cpp
           node.cpp
           nodelist.cpp
           point2d.cpp
           regionsensor.cpp
           stack.cpp
           stackelem.cpp
           stackelemnodelist.cpp
           stackelemnumber.cpp
           stackelemstack.cpp
           stackelemstring.cpp
)

add_library(bottomup STATIC ${sources})
install(TARGETS bottomup DESTINATION lib)
install(FILES ${headers} DESTINATION include)

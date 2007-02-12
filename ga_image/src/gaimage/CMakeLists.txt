include_directories(.
        ${CMAKE_SOURCE_DIR}/pfm/libpfm)

set(headers gaall.h
           gaarray2dt.h
           gaconvert.h
           gadefines.h
           gadowindowt.h
           gadraw.h
           gadrawt.h
           gagradient.h
           gagradientt.h
           gahough.h
           gaimage.h
           gaimagebase.h
           gaimaget.h
           gakirsch.h
           garegion.h
           garegionsplittert.h
           garegiont.h
           gashareddata.h
           gashareddatat.h
           gasplit2regionst.h
           gaimaget.hpp
           gavariance.h
           gavectorizet.h
)

set(templateheaders
           garegionsplittert.hpp
           gasplit2regionst.hpp
           gavectorizet.hpp
)

set(sources gadraw.cpp 
           gagradient.cpp
           gahough.cpp
           gaimage.cpp
           garegion.cpp
           gavariance.cpp
)

add_library(gaimage STATIC ${sources})
install(TARGETS gaimage DESTINATION lib)
install(FILES ${headers} DESTINATION include)

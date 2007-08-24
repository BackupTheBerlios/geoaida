
include_directories(.
        ${CMAKE_SOURCE_DIR}/pfm/libpfm)

set(headers gaall.h
           gaarray2dt.h
           gacanny.h
           gaconvert.h
           gaconvolution.h
           gadefines.h
           gadowindowt.h
           gadraw.h
           gadrawt.h
	   gafft.h
           gagaussian.h
           gagradient.h
           gagradientt.h
           gahough.h
           gaimage.h
           gaimagebase.h
           gaimaget.h
           gakirsch.h
	   gapainter.h
           gapoint.h
           garegion.h
           garegionsplittert.h
           garegiont.h
           gashareddata.h
           gashareddatat.h
           gasplit2regionst.h
           garegionsplitter.h
           gaimaget.hpp
           gavariance.h
           gavectorizet.h
)

set(templateheaders
           garegionsplittert.hpp
           gasplit2regionst.hpp
           gavectorizet.hpp
)

set(sources gacanny.cpp
           gaconvolution.cpp
           gadraw.cpp 
					 gafft.cpp
           gagaussian.cpp
           gagradient.cpp
           gahough.cpp
           gaimage.cpp
					 gapainter.cpp			
           garegion.cpp
           gavariance.cpp					 

)

add_library(gaimage STATIC ${sources})
install(TARGETS gaimage DESTINATION lib)
install(FILES ${headers} DESTINATION include)

add_executable(painttest paint.cpp)
target_link_libraries(painttest gaimage pfm pnm fftw3)

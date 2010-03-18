cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(OTB)  
IF(OTB_FOUND)  
  INCLUDE(${OTB_USE_FILE})  
ELSE(OTB_FOUND)  
  MESSAGE(FATAL_ERROR  
      "Cannot build OTB project without OTB.  Please set OTB_DIR.")  
ENDIF(OTB_FOUND) 

 
ADD_LIBRARY(otbgeo geooperations.cpp )  
TARGET_LINK_LIBRARIES(otbgeo OTBCommon)
install(TARGETS otbgeo
  DESTINATION lib)
install(FILES geooperations.h DESTINATION include/otbgeo)

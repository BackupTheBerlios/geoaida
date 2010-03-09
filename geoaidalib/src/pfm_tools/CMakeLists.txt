find_package(Qt3 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(.
        ${CMAKE_SOURCE_DIR}/pfm/libpfm
        ${CMAKE_SOURCE_DIR}/ga_image/src/gaimage
	)

macro(pfm_tool name extra_sources)
  set(sources ${name}.c ${extra_sources})

  set(data ${name}.mod)

  add_executable(${name} ${sources})


  target_link_libraries(${name} gaimage pfm pnm ${QT_LIBRARIES})

  install(TARGETS ${name} DESTINATION bin)
  install(FILES ${data} DESTINATION share/data/modules)
endmacro(pfm_tool)

macro(pfm_tool_cpp name extra_sources)
  set(sources ${name}.cpp ${extra_sources})

  set(data ${name}.mod)

  add_executable(${name} ${sources})


  target_link_libraries(${name} gaimage pfm pnm ${QT_LIBRARIES})

  install(TARGETS ${name} DESTINATION bin)
  install(FILES ${data} DESTINATION share/data/modules)
endmacro(pfm_tool_cpp)

macro(pfm_tool_plain name extra_sources)
  set(sources ${name}.c ${extra_sources})

#  set(data ${name}.mod)

  add_executable(${name} ${sources})


  target_link_libraries(${name} gaimage pfm pnm ${QT_LIBRARIES})

  install(TARGETS ${name} DESTINATION bin)
#  install(FILES ${data} DESTINATION share/data/modules)
endmacro(pfm_tool_plain)


pfm_tool_plain(ers3band2ppm "")
pfm_tool_plain(pfmgentiles "")
pfm_tool_plain(ppmcutregion "")
pfm_tool_plain(pxminfo "")
pfm_tool_plain(pfm2xyz "")
pfm_tool_plain(pfmheader "")
pfm_tool_plain(pgmgentiles "")
pfm_tool_plain(points2pfm "")
pfm_tool_plain(ppmgentiles "")
pfm_tool_plain(tri2pfm "")
pfm_tool_plain(pfminsertgeoinfo "")
pfm_tool_plain(ppmhalfframe "")

pfm_tool(pfm2pgm "")
pfm_tool(pfmmask "")
pfm_tool(pgmcutregion "")
pfm_tool(plm2pgm "")
pfm_tool(plmdump "")
pfm_tool_cpp(pbm2plm "")
pfm_tool(plm2pbm "")
pfm_tool(pbmblow "")
pfm_tool(pgm2pfm "")
pfm_tool(pfm2pfm "")
pfm_tool(pfmcutregion "")
pfm_tool(pgm2pgm "")
pfm_tool(ppm2ppm "")

pfm_tool_cpp(pfmcutgeoregion "")
pfm_tool_cpp(pfmfilter "")
pfm_tool_cpp(pfmgenblanklabel "")
pfm_tool_cpp(pfmgeoadd "")
pfm_tool_cpp(pfmhist "")
pfm_tool_cpp(pfmlog "")
pfm_tool_cpp(pfmmax "")
pfm_tool_cpp(pfmmaxdiff "PFMArray.cpp;PFMGradAbs;PFMGradAngle")
pfm_tool_cpp(pfmmaxpic "")
pfm_tool_cpp(pfmmedian "")
pfm_tool_cpp(pfmmin "")
pfm_tool_cpp(pfmminpic "")
pfm_tool_cpp(pfmmult "")
pfm_tool_cpp(pfmsubstract "")
pfm_tool_cpp(pfmthreshhold "")
pfm_tool_cpp(pfmvariance "")
pfm_tool_cpp(plmmask "")
pfm_tool_cpp(ppm2hsv "")
pfm_tool_cpp(ppmgrad "")

install(PROGRAMS plm2tiff DESTINATION bin)

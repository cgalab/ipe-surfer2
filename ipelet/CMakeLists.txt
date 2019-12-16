project (ipesurfer2 VERSION "1.00" DESCRIPTION "ipesurfer2lib")

add_library(ipesurfer2 SHARED
  src/ipesurfer2.cpp
  )
set_target_properties(ipesurfer2 PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(ipesurfer2 PROPERTIES PUBLIC_HEADER inc/libipesurfer2.h)

target_compile_definitions(ipesurfer2 PRIVATE ELPP_CUSTOM_COUT=std::cerr)

target_include_directories(ipesurfer2 PRIVATE inc)
target_include_directories(ipesurfer2 PRIVATE ../surfer2/surf/include)
target_include_directories(ipesurfer2 PRIVATE ../surfer2/surf/src)

target_include_directories(ipesurfer2 PRIVATE src)

target_link_libraries(ipesurfer2 "-fPIC -lipe" surflib)

include(GNUInstallDirs)

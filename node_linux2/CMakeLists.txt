cmake_minimum_required(VERSION 3.17)
project(node_linux CXX)



list(APPEND library_source
   _.h
   _.cpp
   appindicator.cpp
   appindicator.h
   factory_exchange.cpp
   factory_exchange.h
   framework.cpp
   framework.h
   #application.cpp
   #application.h
   node.cpp
   node.h
   )


list(APPEND library_references
   acme
   node_ansios
   apex
   aqua
   aura
   )


if (MSVC)
   set(LIBCXX_TARGETING_MSVC ON)
else ()
   set(LIBCXX_TARGETING_MSVC OFF)
endif ()


add_library(node_linux SHARED ${library_source} )

target_precompile_headers(node_linux PRIVATE framework.h)

set_target_properties(node_linux PROPERTIES LINKER_LANGUAGE CXX)


if (MSVC)
   target_compile_options(node_linux PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(node_linux PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(node_linux PRIVATE "/SUBSYSTEM:WINDOWS")
   target_link_libraries(node_linux PRIVATE ${library_references})
   add_dependencies(node_linux draw2d_gdiplus)
elseif (LINUX)
   target_compile_options(node_linux PRIVATE -fpermissive)
   target_compile_options(node_linux PRIVATE ${PKGCONFIG_CFLAGS})
   #target_link_options(node_linux PRIVATE -Wl,-rpath=$ORIGIN)
   target_link_libraries(node_linux PRIVATE ${library_references} ${PKGCONFIG_LIBRARIES})
   #target_link_libraries(node_linux PRIVATE asound)
   target_link_options(node_linux PRIVATE -Wl,-z,defs)
endif ()




target_include_directories(node_linux PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(node_linux PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(node_linux PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_definitions(node_linux PRIVATE _NODE_LINUX_LIBRARY)




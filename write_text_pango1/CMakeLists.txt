cmake_minimum_required(VERSION 3.17)
project(write_text_pango CXX)



list(APPEND library_references
        acme
        apex
        aqua
        aura)


list(APPEND library_source
        _.cpp
        _.h
        factory_exchange.cpp
        font_department.cpp
        font_department.h
        framework.cpp
        framework.h
        write_text.cpp
        write_text.h)




add_library(write_text_pango SHARED ${library_source})

target_precompile_headers(write_text_pango PRIVATE framework.h)

set_target_properties(write_text_pango PROPERTIES LINKER_LANGUAGE CXX)

if (MSVC)
    target_compile_options(write_text_pango PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(write_text_pango PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(write_text_pango PRIVATE "/IMPLIB:${LIBRARY_OUTPUT_PATH}/write_text_pango.lib")
    target_link_options(write_text_pango PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/write_text_pango.def")
    target_link_libraries(write_text_pango PRIVATE Gdiplus ${library_references})
elseif(LINUX)
    pkg_check_modules(PKGCONFIG pangocairo)
    target_compile_options(write_text_pango PRIVATE ${PKGCONFIG_CFLAGS})
    #target_link_options(write_text_pango PRIVATE -Wl,-rpath=$ORIGIN)
    target_link_libraries(write_text_pango PRIVATE ${PKGCONFIG_LIBRARIES} ${library_references})
    target_link_options(write_text_pango PRIVATE -Wl,-z,defs)
    message("PKGCONFIG_LIBRARIES is " ${PKGCONFIG_LIBRARIES})
endif()




target_include_directories(write_text_pango PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(write_text_pango PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(write_text_pango PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_definitions(write_text_pango PRIVATE _WRITE_TEXT_PANGO_LIBRARY)




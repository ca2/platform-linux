cmake_minimum_required(VERSION 3.17)
project(windowing_xcb CXX)



list(APPEND library_source
   _.h
   _.cpp
   #application.cpp
   #application.h
   buffer.cpp
   buffer.h
   cursor.cpp
   cursor.h
   #copydesk.cpp
   #copydesk.h
   debug.cpp
   display.cpp
   display.h
   display_lock.cpp
   display_lock.h
   display_xrender.cpp
   enum_display_monitors.cpp
   factory_exchange.cpp
   factory_exchange.h
   framework.cpp
   framework.h
   #gdk.cpp
   #gdk.h
   #gnome_direct.cpp
   icon.cpp
   icon.h
   keyboard.cpp
   keyboard.h
   #node.cpp
   #node.h
   sn.cpp
   sn.h
   user.cpp
   window.cpp
   window.h
   windowing.cpp
   windowing_wm.cpp
   windowing_xcb.cpp
   windowing.h
   xcb_windowing.cpp
   text_composition_client.cpp 
   text_composition_client.h
        windowing_xcb.cpp
        windowing_xcb.h
        xcb_icccm.cpp
   xi.cpp
   )


list(APPEND library_references
   acme
   apex
   aqua
   aura
   )



add_library(windowing_xcb SHARED ${library_source})

target_precompile_headers(windowing_xcb PRIVATE framework.h)

set_target_properties(windowing_xcb PROPERTIES LINKER_LANGUAGE CXX)


if (MSVC)
   target_compile_options(windowing_xcb PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(windowing_xcb PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(windowing_xcb PRIVATE "/SUBSYSTEM:WINDOWS")
   target_link_libraries(windowing_xcb PRIVATE ${library_references})
   add_dependencies(windowing_xcb draw2d_gdiplus)
elseif (LINUX)
   #pkg_check_modules(PKGCONFIG xcb xrender xi)
   pkg_check_modules(PKGCONFIG xcb xcb-render xcb-image xkbcommon-x11 xcb-icccm)
   target_compile_options(windowing_xcb PRIVATE -fpermissive)
   target_compile_options(windowing_xcb PRIVATE ${PKGCONFIG_CFLAGS})
   #target_link_options(windowing_xcb PRIVATE -Wl,-rpath=$ORIGIN)
   target_link_libraries(windowing_xcb PRIVATE ${library_references} ${PKGCONFIG_LIBRARIES})
   #target_link_libraries(windowing_xcb PRIVATE asound)
   target_link_options(windowing_xcb PRIVATE -Wl,-z,defs)
   add_dependencies(windowing_xcb draw2d_cairo)
endif ()



target_link_directories(windowing_xcb PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_directories(windowing_xcb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../archive/storage-${PLATFORM_NAME}/library/x64/basis)
target_link_directories(windowing_xcb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../archive/storage-${PLATFORM_NAME}/third/library/x64/basis)


target_include_directories(windowing_xcb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(windowing_xcb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(windowing_xcb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_definitions(windowing_xcb PRIVATE _MUSIC_MIDI_ALSA_LIBRARY)




add_definitions(-D_HAS_EXCEPTIONS=0 -DNOMINMAX -DUNICODE)

#FIXME: Make sure these are the same as the Windows feature defines once we get CMake working on Windows.
WEBKIT_OPTION_BEGIN()
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DRAG_SUPPORT PUBLIC ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_GEOLOCATION PUBLIC ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_LEGACY_VENDOR_PREFIXES PUBLIC OFF)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_PICTURE_SIZES PUBLIC OFF)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_STREAMS_API PUBLIC ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_VIEW_MODE_CSS_MEDIA PUBLIC OFF)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(USE_SYSTEM_MALLOC PUBLIC ON)
WEBKIT_OPTION_END()

include_directories("$ENV{WEBKIT_LIBRARIES}/include")
if (${MSVC_CXX_ARCHITECTURE_ID} STREQUAL "X86")
    link_directories("$ENV{WEBKIT_LIBRARIES}/lib32")
else ()
    link_directories("$ENV{WEBKIT_LIBRARIES}/lib64")
endif ()

if (MSVC)
    add_definitions(
        /wd4018 /wd4068 /wd4099 /wd4100 /wd4127 /wd4138 /wd4146 /wd4180 /wd4189 /wd4201 /wd4244 /wd4251 /wd4267 /wd4275 /wd4288
        /wd4291 /wd4305 /wd4309 /wd4344 /wd4355 /wd4389 /wd4396 /wd4481 /wd4503 /wd4505 /wd4510 /wd4512 /wd4530 /wd4610 /wd4702
        /wd4706 /wd4800 /wd4819 /wd4951 /wd4952 /wd4996 /wd6011 /wd6031 /wd6211 /wd6246 /wd6255 /wd6387
    )
    if (NOT ${CMAKE_GENERATOR} MATCHES "Ninja")
        add_definitions(/MP)
    endif ()
    if (NOT ${CMAKE_CXX_FLAGS} STREQUAL "")
        string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # Disable C++ exceptions
        string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # Disable RTTI
    endif ()

    foreach (flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        # Use the multithreaded static runtime library instead of the default DLL runtime.
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")

        # No debug runtime, even in debug builds.
        string(REGEX REPLACE "/MTd" "/MT" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "/D_DEBUG" "" ${flag_var} "${${flag_var}}")
    endforeach ()
endif ()

set(PORT Win)
set(JavaScriptCore_LIBRARY_TYPE SHARED)
set(WTF_LIBRARY_TYPE SHARED)
set(ICU_LIBRARIES libicuuc libicuin)

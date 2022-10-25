set(VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
string(TIMESTAMP BUILD_TIME "%Y%m%d%H%M%S")
set(third_party_dir "${CMAKE_SOURCE_DIR}/third_party")

list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/spdlog-1.10.0/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/rapidjson-1.1.0/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/libuv-1.44.1/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/v8-10.3/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/mariadb-connector-1.0.1/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/simpleini-4.19/include")
list(APPEND CMAKE_INCLUDE_DIR "${third_party_dir}/breakpad/include")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("current platform: Linux")
    set(PLATFORM_NAME "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message("current platform: Windows")
    set(PLATFORM_NAME "Windows")

    add_compile_definitions(V8_COMPRESS_POINTERS) # _ITERATOR_DEBUG_LEVEL=0 V8_ENABLE_SANDBOX

    list(APPEND CMAKE_LIB_DIR "${third_party_dir}/spdlog-1.10.0/lib/windows")
    list(APPEND CMAKE_LIB_DIR "${third_party_dir}/libuv-1.44.1/lib/windows")
    list(APPEND CMAKE_LIB_DIR "${third_party_dir}/v8-10.3/lib/windows")
    list(APPEND CMAKE_LIB_DIR "${third_party_dir}/mariadb-connector-1.0.1/lib/windows")
    list(APPEND CMAKE_LIB_DIR "${third_party_dir}/breakpad/lib/windows")

    set(mariadbcpp_dll "${third_party_dir}/mariadb-connector-1.0.1/lib/windows/mariadbcpp.dll")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(lib_suffix "d")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")

    else()
        set(lib_suffix "r")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()

    list(APPEND target_link_lib "spdlog_${lib_suffix}")
    list(APPEND target_link_lib "uv_${lib_suffix}")
    list(APPEND target_link_lib "v8_monolith_${lib_suffix}")
    list(APPEND target_link_lib "mariadbcpp")
    list(APPEND target_link_lib "exception_handler_${lib_suffix}" "crash_generation_client_${lib_suffix}" "common_${lib_suffix}")
    list(APPEND target_link_lib "ws2_32" "iphlpapi" "userenv")
    list(APPEND target_link_lib "winmm" "dbghelp")
else()
    message("other platform: ${CMAKE_SYSTEM_NAME}")
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

include_directories(${CMAKE_INCLUDE_DIR})
link_directories(${CMAKE_LIB_DIR})

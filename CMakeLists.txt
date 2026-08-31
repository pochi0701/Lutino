cmake_minimum_required(VERSION 3.20)

project(lutino LANGUAGES C CXX)

set(LUTINO_DEFAULT_INSTALL_PREFIX "/var/www/html9")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
   OR (NOT DEFINED LUTINO_INSTALL_PREFIX_VERSION
       AND CMAKE_INSTALL_PREFIX STREQUAL "/usr/local/lutino"))
  set(CMAKE_INSTALL_PREFIX "${LUTINO_DEFAULT_INSTALL_PREFIX}" CACHE PATH "Lutino install directory" FORCE)
endif()
set(LUTINO_INSTALL_PREFIX_VERSION 1 CACHE INTERNAL "Lutino install prefix version")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

set(LUTINO_CORE_SOURCES
  source/ltn_main.cpp
  source/ltn_param.cpp
  source/ltn_listen.cpp
  source/ltn_http.cpp
  source/ltn_send_file.cpp
  source/ltn_proxy.cpp
  source/ltn_cgi.cpp
  source/ltn_tools.cpp
  source/ltn_script.cpp
  source/ltn_String.cpp
  source/ltn_transport.cpp
  source/ltn_tls.cpp
  source/ssdp.cpp
  source/TinyJS.cpp
  source/TinyJS_Functions.cpp
  source/TinyJS_MathFunctions.cpp
  source/ltn_db.cpp
  source/utf8tbl.cpp
  source/dregex.cpp
  source/alt_time.cpp
)

file(GLOB LUTINO_BEARSSL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/source/bearssl/*.cpp"
)

list(FILTER LUTINO_BEARSSL_SOURCES EXCLUDE REGEX ".*/ltn_bearssl_all\\.cpp$")

add_executable(lutino
  ${LUTINO_CORE_SOURCES}
  ${LUTINO_BEARSSL_SOURCES}
)

target_compile_definitions(lutino PRIVATE
  BR_AES_X86NI=0
  BR_SSE2=0
  BR_RDRAND=0
  BR_POWER8=0
)

target_compile_options(lutino PRIVATE
  -Wall
  -pthread
)

target_link_libraries(lutino PRIVATE Threads::Threads)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/InstallLutinoConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/InstallLutinoConfig.cmake"
  @ONLY
)

install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/InstallLutinoConfig.cmake")
install(TARGETS lutino RUNTIME DESTINATION ".")
install(DIRECTORY html/ DESTINATION "html")
install(DIRECTORY database/ DESTINATION "database")
install(DIRECTORY skin/ DESTINATION "skin")
install(DIRECTORY system/ DESTINATION "system")

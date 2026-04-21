cmake_minimum_required(VERSION 3.20)

project(lutino LANGUAGES C CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local/lutino" CACHE PATH "Lutino install root" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

set(LUTINO_DOCUMENT_ROOT "" CACHE PATH "Document root written to installed ltn.conf. Empty = <install-prefix>/html")
set(LUTINO_SKIN_ROOT "" CACHE PATH "Skin root written to installed ltn.conf. Empty = <install-prefix>/skin")
set(LUTINO_SYSTEM_ROOT "" CACHE PATH "System asset root written to installed ltn.conf. Empty = <install-prefix>/system")
set(LUTINO_DATABASE_ROOT "" CACHE PATH "Database asset install root. Empty = <install-prefix>/database")
set(LUTINO_WORK_ROOT "" CACHE PATH "Work root written to installed ltn.conf. Empty = <install-prefix>/work")

if(LUTINO_DOCUMENT_ROOT STREQUAL "")
  set(LUTINO_DOCUMENT_ROOT_INSTALL_DEST "html")
else()
  set(LUTINO_DOCUMENT_ROOT_INSTALL_DEST "${LUTINO_DOCUMENT_ROOT}")
endif()

if(LUTINO_SKIN_ROOT STREQUAL "")
  set(LUTINO_SKIN_ROOT_INSTALL_DEST "skin")
else()
  set(LUTINO_SKIN_ROOT_INSTALL_DEST "${LUTINO_SKIN_ROOT}")
endif()

if(LUTINO_SYSTEM_ROOT STREQUAL "")
  set(LUTINO_SYSTEM_ROOT_INSTALL_DEST "system")
else()
  set(LUTINO_SYSTEM_ROOT_INSTALL_DEST "${LUTINO_SYSTEM_ROOT}")
endif()

if(LUTINO_DATABASE_ROOT STREQUAL "")
  set(LUTINO_DATABASE_ROOT_INSTALL_DEST "database")
else()
  set(LUTINO_DATABASE_ROOT_INSTALL_DEST "${LUTINO_DATABASE_ROOT}")
endif()

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

target_include_directories(lutino PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/source"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/bearssl"
)

target_compile_definitions(lutino PRIVATE
  linux
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

install(TARGETS lutino RUNTIME DESTINATION ".")
install(FILES README.md LICENSE DESTINATION ".")
install(DIRECTORY html/ DESTINATION "${LUTINO_DOCUMENT_ROOT_INSTALL_DEST}")
install(DIRECTORY database/ DESTINATION "${LUTINO_DATABASE_ROOT_INSTALL_DEST}")
install(DIRECTORY system/ DESTINATION "${LUTINO_SYSTEM_ROOT_INSTALL_DEST}")
install(DIRECTORY skin/ DESTINATION "${LUTINO_SKIN_ROOT_INSTALL_DEST}")
install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/InstallLutinoConfig.cmake")

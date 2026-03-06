cmake_minimum_required(VERSION 3.5)
project(FOSD)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
if(UNIX)
add_compile_options(-fPIC)
endif()
set(CMAKE_PREFIX_PATH "/usr/local/dseapp;/usr/local/rtidds")

include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_definitions(
        -DRTI_ENDIAN_BIG
    )
else()
    add_definitions(
        -DRTI_ENDIAN_LITTLE
    )
endif()

   find_package(dseapp CONFIG REQUIRED)


   find_package(rtidds CONFIG REQUIRED)

add_definitions(
-DRTI_UNIX
)
include_directories(
${dseapp_INCLUDE_DIR}
${rtidds_INCLUDE_DIR}
)
link_directories(
../lib
)

add_executable(FOSD
main.cpp
)
set_property(TARGET FOSD PROPERTY CXX_STANDARD 11)
set_property(TARGET FOSD PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(FOSD
-lFOSD
XmlParser
DseAuth
DsePrometheus
SimpleThreadServer
ServerBase
EsInsert
DseDdsManagerRti
nddscpp
nddsc
nddscore
pthread
)

install (
    TARGETS
    FOSD
    DESTINATION
    .
)
install (
    FILES
    ../lib/libFOSD.so
    DESTINATION
    .
)
install (
    DIRECTORY
    doc
    OPTIONAL DESTINATION
    .
)

install (
    FILES
    ../dseappconfig/FOSD.xml
    DESTINATION
    dseappconfig
)
install (
    DIRECTORY
    doc
    OPTIONAL DESTINATION
    .
)




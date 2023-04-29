cmake_minimum_required(VERSION 3.25)
project("screensaver-gdbus")

add_executable(test
src/main.cpp
)

add_library(screensaver-dbus
src/screensaver-dbus.cpp
)
install(TARGETS screensaver-dbus DESTINATION lib)

if(CONNECTION_RESET)
    target_compile_definitions(screensaver-dbus PRIVATE CONNECTION_RESET)
endif()

target_link_libraries(test PRIVATE screensaver-dbus)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED IMPORTED_TARGET dbus-1)
target_include_directories(screensaver-dbus PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(screensaver-dbus INTERFACE PkgConfig::DBUS)

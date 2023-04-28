cmake_minimum_required(VERSION 3.25)
project("screensaver-gdbus")

add_executable(screensaver-dbus
screensaver-dbus.cpp
)

install(TARGETS screensaver-dbus DESTINATION bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED IMPORTED_TARGET dbus-1)
target_link_libraries(screensaver-dbus PkgConfig::DBUS)

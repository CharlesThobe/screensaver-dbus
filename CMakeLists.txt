cmake_minimum_required(VERSION 3.25)
project("screensaver-gdbus")

option(TEST_BUILD "Generate testing executable and library" OFF)

add_library(screensaver-dbus
src/screensaver-dbus.cpp
)

if(TEST_BUILD)
    target_compile_definitions(screensaver-dbus PRIVATE "TEST_BUILD")
    add_executable(test
    src/main.cpp
    )
    target_link_libraries(test PRIVATE screensaver-dbus)
else()
    install(TARGETS screensaver-dbus DESTINATION lib)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)
target_include_directories(screensaver-dbus PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(screensaver-dbus PRIVATE ${DBUS_LINK_LIBRARIES})

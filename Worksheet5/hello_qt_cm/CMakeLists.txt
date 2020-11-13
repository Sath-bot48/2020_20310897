cmake_minimum_required( VERSION 2.8.11 )
# Name of the project
project( hello_qt )
# The CMake build process might generate some new files in the current
# directory. This makes sure they can be found.
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
# Find the Qt widgets package. This locates the relevant include and
# lib directories, and the necessary static libraries for linking.
find_package( Qt5Widgets )
# Same as previously: define the executable and it's sources.
add_executable( hello_qt hello_qt.cpp )
# Tell CMake that the executable depends on the Qt::Widget libraries.
target_link_libraries( hello_qt Qt5::Widgets )
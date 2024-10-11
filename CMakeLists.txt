cmake_minimum_required(VERSION 3.16)
project(InfineonTask)

# CMake standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

if(Qt6_FOUND)
    message(STATUS "Qt version: ${Qt6_VERSION}")
else()
    message(STATUS "Qt not found. Initialize download")
    include(ExternalProject)

  # Set the Qt version you want to install
  set(QT_VERSION "6.6.3")

  # Define the Qt project
  ExternalProject_Add(
      QtProject
      PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/qt
      GIT_REPOSITORY https://code.qt.io/qt/qt5.git
      GIT_TAG v${QT_VERSION}
      CONFIGURE_COMMAND ./configure -prefix <INSTALL_DIR> -opensource -confirm-license -nomake tests -nomake examples
      BUILD_COMMAND make
      INSTALL_COMMAND make install
      BUILD_IN_SOURCE 1
  )

  # Set the path to the Qt installation
  ExternalProject_Get_Property(QtProject install_dir)
  set(QT_ROOT ${install_dir})
endif()


set(ALL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries 
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
)

# Set the destination directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


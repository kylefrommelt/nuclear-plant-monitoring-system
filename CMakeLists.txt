cmake_minimum_required(VERSION 3.16)
project(NuclearPlantMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific configuration
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/PlantMonitor.cpp
    src/ModbusHandler.cpp
    src/SocketManager.cpp
    src/SecurityManager.cpp
    src/DataProcessor.cpp
    src/SensorInterface.cpp
)

# Header files
set(HEADERS
    include/PlantMonitor.h
    include/ModbusHandler.h
    include/SocketManager.h
    include/SecurityManager.h
    include/DataProcessor.h
    include/SensorInterface.h
    include/ISensorReader.h
    include/IDataProcessor.h
    include/ISecurityManager.h
)

# Main executable
add_executable(NuclearPlantMonitor ${SOURCES} ${HEADERS})

# Windows socket library
if(WIN32)
    target_link_libraries(NuclearPlantMonitor ws2_32)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(NuclearPlantMonitor PRIVATE /W4 /WX)
else()
    target_compile_options(NuclearPlantMonitor PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Enable testing
enable_testing()
add_subdirectory(tests)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND) 
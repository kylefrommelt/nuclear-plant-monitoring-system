# Unit testing configuration for Nuclear Plant Monitor

# Find or provide a simple testing framework
# In production, you might use Google Test or Catch2

# Create a simple test runner
add_executable(TestRunner
    test_main.cpp
    SecurityManagerTest.cpp
    DataProcessorTest.cpp
    ModbusHandlerTest.cpp
)

# Link against the main project libraries
target_link_libraries(TestRunner
    ${CMAKE_PROJECT_NAME}
)

# Include directories
target_include_directories(TestRunner PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(TestRunner ws2_32)
endif()

# Add test discovery
add_test(NAME SecurityManagerTests COMMAND TestRunner security)
add_test(NAME DataProcessorTests COMMAND TestRunner dataprocessor)
add_test(NAME ModbusHandlerTests COMMAND TestRunner modbus)
add_test(NAME AllTests COMMAND TestRunner all)

# Test properties
set_tests_properties(SecurityManagerTests PROPERTIES
    PASS_REGULAR_EXPRESSION "PASSED.*SecurityManager"
)

set_tests_properties(DataProcessorTests PROPERTIES
    PASS_REGULAR_EXPRESSION "PASSED.*DataProcessor"
)

set_tests_properties(ModbusHandlerTests PROPERTIES
    PASS_REGULAR_EXPRESSION "PASSED.*ModbusHandler"
) 
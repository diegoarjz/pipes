set(UNIT_TEST_SOURCES
    "main.cpp"
    #    "test_task.cpp"
    "test_task_graph.cpp"
    )

add_executable(unit_tests ${UNIT_TEST_SOURCES})

set_target_properties(
    unit_tests
    PROPERTIES 
        COMPILE_FLAGS ${COMPILE_FLAGS} "-Wno-gnu-zero-variadic-macro-arguments"
)

target_compile_features(
    unit_tests
    PRIVATE
        cxx_std_14
)

target_compile_options(
    unit_tests
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    unit_tests
    PUBLIC
        $<INSTALL_INTERFACE:pipes>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    unit_tests
    PRIVATE
        pipes
        pthread
        gmock
        gtest
)

add_test(NAME unit_tests COMMAND unit_tests)


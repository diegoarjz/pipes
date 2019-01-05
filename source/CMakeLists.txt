add_subdirectory(debug)

set(PIPES_SOURCES
        "task.cpp"
        "task_execution.cpp"
        "task_graph.cpp"
        "scheduler.cpp"
        "worker_thread.cpp"
    )

add_library(
    pipes
    "${PIPES_SOURCES}"
    )

target_compile_features(
    pipes
    PRIVATE
        cxx_std_14
)

target_compile_options(
    pipes
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_link_libraries(
    pipes
    PRIVATE
        debug_pipes
)

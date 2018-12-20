set(PIPES_SOURCES
        "task.cpp"
        "task_execution.cpp"
        "task_graph.cpp"
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


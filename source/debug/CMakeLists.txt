set(DEBUG_SOURCES
    "graph_dot_writer.cpp"
    )

add_library(debug_pipes
    ${DEBUG_SOURCES})

target_compile_features(
    debug_pipes
    PRIVATE
        cxx_std_14
)

target_compile_options(
    debug_pipes
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

####################
# Graph Topologies #
####################

add_executable(graph_topologies "graph_topologies.cpp")

target_compile_features(
    graph_topologies
    PRIVATE
        cxx_std_14
)

target_compile_options(
    graph_topologies
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    graph_topologies
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
)

target_link_libraries(
    graph_topologies
    PRIVATE
        pipes
)

##############
# Vector Sum #
##############

add_executable(vector_sum "vector_sum.cpp")

target_compile_features(
    vector_sum
    PRIVATE
        cxx_std_14
)

target_compile_options(
    vector_sum
    PRIVATE
        -Wall
        -Werror
        -pedantic-errors
)

target_include_directories(
    vector_sum
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
)

target_link_libraries(
    vector_sum
    PRIVATE
        pipes
)


file(COPY "graph_topologies.py" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

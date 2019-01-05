import deregress

GRAPH_NUM = 0
def next_graph():
    global GRAPH_NUM
    graph_num = GRAPH_NUM
    GRAPH_NUM += 1
    return graph_num

@deregress.test
def test_simple_dependency(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--graph-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_merged_dependency(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--dep=c,b") \
            .arg("--graph-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_forked_dependency(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--dep=a,c") \
            .arg("--graph-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_cyclic_dependency(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--dep=b,c") \
            .arg("--dep=c,a") \
            .arg("--graph-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

#######################
# Topological Sorting #
#######################

@deregress.test
def test_merged_dependency_topological_sort(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--dep=c,b") \
            .arg("--topological-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_forked_dependency_topological_sort(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=a,b") \
            .arg("--dep=a,c") \
            .arg("--topological-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

###################
# Task Scheduling #
###################

@deregress.test
def test_forked_dependency_scheduling(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=0,1") \
            .arg("--dep=0,2") \
            .arg("--scheduled-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_merged_dependency_scheduling(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=0,1") \
            .arg("--dep=2,1") \
            .arg("--scheduled-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)

@deregress.test
def test_diamong_dependency_scheduling(test):
    graph_out_file = "graph_{}.dot".format(next_graph())
    test.runner.executable("./graph_topologies") \
            .arg("--dep=0,1") \
            .arg("--dep=1,3") \
            .arg("--dep=0,2") \
            .arg("--dep=2,3") \
            .arg("--scheduled-out={}".format(graph_out_file)) \
            .run()

    test.tester.file_should_exist(graph_out_file)
    test.tester.file_hash_should_match(graph_out_file)


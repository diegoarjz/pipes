#include <gtest/gtest.h>
#include <task_graph.cpp>

using namespace pipes;

class TestTaskGraph : public ::testing::Test
{
protected:
	virtual void SetUp() { m_graph = TaskGraph(); }

	virtual void TearDown() {}

	TaskGraph m_graph;
};

TEST_F(TestTaskGraph, when_creating_tasks_should_be_accessible_through_iteration)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	TaskGraphVertexIteratorPair iters = TasksIterator(m_graph);

	ASSERT_EQ(std::distance(iters.first, iters.second), 2);
	ASSERT_EQ(*iters.first, task1);
	iters.first++;
	ASSERT_EQ(*iters.first, task2);
}

TEST_F(TestTaskGraph, when_retrieving_a_task_from_a_graph_should_return_the_referenced_task)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	TaskPtr task = GetTask(m_graph, task1);
	ASSERT_EQ(task->GetId(), task1);
	task = GetTask(m_graph, task2);
	ASSERT_EQ(task->GetId(), task2);
}

TEST_F(TestTaskGraph, when_creating_a_dependency_should_add_the_dependency_to_the_task_list_of_dependents)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	auto dependents = GetDependentsOfTask(m_graph, task1);
	ASSERT_EQ(dependents.size(), 1);
	ASSERT_EQ(dependents.front(), task2);
}

TEST_F(TestTaskGraph, when_creating_a_dependency_should_add_the_dependency_to_the_task_list_of_dependencies)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	auto dependencies = GetDependenciesOfTask(m_graph, task2);
	ASSERT_EQ(dependencies.size(), 1);
	ASSERT_EQ(dependencies.front(), task1);
}

TEST_F(TestTaskGraph, when_adding_a_dependency_creating_a_cycle_should_return_CycleDetected)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);
	auto task3 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	CreateDependency(m_graph, task2, task3);
	DependencyCreated created = CreateDependency(m_graph, task3, task1);
	ASSERT_EQ(created, DependencyCreated::CycleDetected);
}

TEST_F(TestTaskGraph, when_creating_a_new_deppendency_should_return_Created)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	DependencyCreated created = CreateDependency(m_graph, task1, task2);
	ASSERT_EQ(created, DependencyCreated::Created);
}

TEST_F(TestTaskGraph, when_creating_a_duplicate_deppendency_should_return_AlreadyExists)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	DependencyCreated created = CreateDependency(m_graph, task1, task2);
	ASSERT_EQ(created, DependencyCreated::AlreadyExists);
}

TEST_F(TestTaskGraph, when_destroying_a_dependency_should_remove_dependency_from_the_task_list_of_dependencies)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	DestroyDependency(m_graph, task1, task2);
	auto dependencies = GetDependenciesOfTask(m_graph, task2);
	ASSERT_EQ(dependencies.size(), 0);
}

TEST_F(TestTaskGraph, when_destroying_a_dependency_should_remove_dependency_from_the_task_list_of_dependents)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	DestroyDependency(m_graph, task1, task2);
	auto dependencies = GetDependentsOfTask(m_graph, task1);
	ASSERT_EQ(dependencies.size(), 0);
}

TEST_F(TestTaskGraph, when_checking_for_the_existence_of_an_dependency_of_two_dependant_tasks_should_return_true)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	CreateDependency(m_graph, task1, task2);
	ASSERT_TRUE(DependencyExists(m_graph, task1, task2));
}

TEST_F(TestTaskGraph, when_checking_for_the_existence_of_an_dependency_of_two_non_dependant_tasks_should_return_false)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);

	ASSERT_FALSE(DependencyExists(m_graph, task1, task2));
}

class TestTaskGraphTopologicalSorting : public ::testing::Test
{
protected:
	virtual void SetUp() { m_graph = TaskGraph(); }

	virtual void TearDown() {}

	TaskGraph m_graph;
};

TEST_F(TestTaskGraphTopologicalSorting, when_a_graph_only_has_a_single_vertex_should_return_that_vertex)
{
	auto task = CreateTask(m_graph);
	auto verts = TopologicalSort(m_graph);

	ASSERT_EQ(verts.size(), 1);
	ASSERT_EQ(verts.front(), task);
}

TEST_F(TestTaskGraphTopologicalSorting, when_a_graph_has_a_dependency_should_return_the_dependency_before_the_dependent)
{
	auto task1 = CreateTask(m_graph);
	auto task2 = CreateTask(m_graph);
	CreateDependency(m_graph, task1, task2);

	auto verts = TopologicalSort(m_graph);

	ASSERT_EQ(verts.size(), 2);
	ASSERT_EQ(verts.front(), task1);
	ASSERT_EQ(verts.back(), task2);
}

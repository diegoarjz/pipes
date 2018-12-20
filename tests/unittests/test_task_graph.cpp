#include <task_graph.cpp>

#include <gtest/gtest.h>

using namespace pipes;

class TestTaskGraph : public ::testing::Test
{
protected:
	virtual void SetUp() { m_graph = std::make_shared<TaskGraph>(); }

	virtual void TearDown() { m_graph = nullptr; }

	std::shared_ptr<TaskGraph> m_graph;
};

TEST_F(TestTaskGraph, when_creating_a_task_should_return_a_task) { ASSERT_NE(m_graph->CreateTask(), nullptr); }

TEST_F(TestTaskGraph, when_creating_a_task_should_add_it_to_the_list_of_nodes)
{
	auto task = m_graph->CreateTask();
	auto taskList = m_graph->GetTasks();

	ASSERT_EQ(taskList.size(), 1);
	ASSERT_EQ(taskList.front(), task);
}

TEST_F(TestTaskGraph, when_destroying_a_task_should_remove_it_from_the_list_of_nodes)
{
	auto task = m_graph->CreateTask();
	m_graph->DestroyTask(task);
	auto taskList = m_graph->GetTasks();

	ASSERT_EQ(taskList.size(), 0);
}

TEST_F(TestTaskGraph, when_creating_a_dependency_should_add_the_dependency_to_the_task_list_of_dependents)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	m_graph->CreateDependency(task1, task2);
	auto dependents = m_graph->GetDependentsOfTask(task1);
	ASSERT_EQ(dependents.size(), 1);
	ASSERT_EQ(dependents.front(), task2);
}

TEST_F(TestTaskGraph, when_creating_a_dependency_should_add_the_dependency_to_the_task_list_of_dependencies)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	m_graph->CreateDependency(task1, task2);
	auto dependencies = m_graph->GetDependenciesOfTask(task2);
	ASSERT_EQ(dependencies.size(), 1);
	ASSERT_EQ(dependencies.front(), task1);
}

TEST_F(TestTaskGraph, when_creating_a_new_deppendency_should_return_Created)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	TaskGraph::DependencyCreated created = m_graph->CreateDependency(task1, task2);
	ASSERT_EQ(created, TaskGraph::DependencyCreated::Created);
}

TEST_F(TestTaskGraph, when_creating_a_duplicate_deppendency_should_return_AlreadyExists)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	m_graph->CreateDependency(task1, task2);
	TaskGraph::DependencyCreated created = m_graph->CreateDependency(task1, task2);
	ASSERT_EQ(created, TaskGraph::DependencyCreated::AlreadyExists);
}

TEST_F(TestTaskGraph, when_destroying_a_dependency_should_remove_dependency_from_the_task_list_of_dependencies)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	m_graph->CreateDependency(task1, task2);
	m_graph->DestroyDependency(task1, task2);
	auto dependencies = m_graph->GetDependenciesOfTask(task2);
	ASSERT_EQ(dependencies.size(), 0);
}

TEST_F(TestTaskGraph, when_destroying_a_dependency_should_remove_dependency_from_the_task_list_of_dependents)
{
	auto task1 = m_graph->CreateTask();
	auto task2 = m_graph->CreateTask();

	m_graph->CreateDependency(task1, task2);
	m_graph->DestroyDependency(task1, task2);
	auto dependencies = m_graph->GetDependentsOfTask(task1);
	ASSERT_EQ(dependencies.size(), 0);
}

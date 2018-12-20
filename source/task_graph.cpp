#include "task_graph.h"

#include "task.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include <iostream>
#include <typeinfo>

namespace pipes
{
class TaskGraph::Impl
{
public:
	using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS, TaskPtr>;

	TaskPtr CreateTask()
	{
		auto v = boost::add_vertex(m_graph);
		auto task = std::make_shared<Task>(v);
		m_graph[v] = task;

		m_tasks.push_back(task);
		return task;
	}

	void DestroyTask(TaskPtr task)
	{
		boost::remove_vertex(task->GetId(), m_graph);
		m_tasks.remove(task);
	}

	TaskGraph::DependencyCreated CreateDependency(TaskPtr upstreamTask, TaskPtr downstreamTask)
	{
		auto edge = boost::add_edge(upstreamTask->GetId(), downstreamTask->GetId(), m_graph);
		if (edge.second)
		{
			return TaskGraph::DependencyCreated::Created;
		}
		return TaskGraph::DependencyCreated::AlreadyExists;
	}

	void DestroyDependency(TaskPtr upstreamTask, TaskPtr downstreamTask)
	{
		boost::remove_edge(upstreamTask->GetId(), downstreamTask->GetId(), m_graph);
	}

	const std::list<TaskPtr>& GetTasks() const { return m_tasks; }

	std::list<TaskPtr> GetDependenciesOfTask(TaskPtr task)
	{
		std::list<TaskPtr> dependencies;
		typename boost::graph_traits<Graph>::in_edge_iterator iter, end;

		auto inEdges = boost::in_edges(task->GetId(), m_graph);
		for (boost::tie(iter, end) = inEdges; iter != end; ++iter)
		{
			dependencies.push_back(m_graph[boost::source(*iter, m_graph)]);
		}
		return dependencies;
	}

	std::list<TaskPtr> GetDependentsOfTask(TaskPtr task)
	{
		std::list<TaskPtr> dependencies;
		typename boost::graph_traits<Graph>::out_edge_iterator iter, end;

		auto inEdges = boost::out_edges(task->GetId(), m_graph);
		for (boost::tie(iter, end) = inEdges; iter != end; ++iter)
		{
			dependencies.push_back(m_graph[boost::target(*iter, m_graph)]);
		}
		return dependencies;
	}

private:
	static TaskId next_task_id() { return s_nextId++; }
	Graph m_graph;
	std::list<TaskPtr> m_tasks;
	static TaskId s_nextId;
};

TaskGraph::TaskId TaskGraph::Impl::s_nextId = 0;

TaskGraph::TaskGraph() : m_implementation(std::make_unique<TaskGraph::Impl>()) {}
TaskGraph::~TaskGraph() { m_implementation = nullptr; }

TaskPtr TaskGraph::CreateTask() { return m_implementation->CreateTask(); }
void TaskGraph::DestroyTask(TaskPtr task) { m_implementation->DestroyTask(task); }

TaskGraph::DependencyCreated TaskGraph::CreateDependency(TaskPtr upstreamTask, TaskPtr downstreamTask)
{
	return m_implementation->CreateDependency(upstreamTask, downstreamTask);
}
void TaskGraph::DestroyDependency(TaskPtr upstreamTask, TaskPtr downstreamTask)
{
	m_implementation->DestroyDependency(upstreamTask, downstreamTask);
}

const std::list<TaskPtr>& TaskGraph::GetTasks() const { return m_implementation->GetTasks(); }
std::list<TaskPtr> TaskGraph::GetDependenciesOfTask(TaskPtr task)
{
	return m_implementation->GetDependenciesOfTask(task);
}
std::list<TaskPtr> TaskGraph::GetDependentsOfTask(TaskPtr task) { return m_implementation->GetDependentsOfTask(task); }
}  // namespace pipes

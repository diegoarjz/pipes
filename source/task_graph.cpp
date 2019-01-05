#include "task_graph.h"

#include "task.h"

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/topological_sort.hpp>

#include <iostream>
#include <typeinfo>

namespace pipes
{
class CycleFoundException : public std::exception
{
};

class CycleDetectorVisistor : public boost::default_dfs_visitor
{
public:
	void back_edge(boost::graph_traits<TaskGraph>::edge_descriptor edge, const TaskGraph &graph) const
	{
		throw CycleFoundException();
	}
};

TaskVertexId CreateTask(TaskGraph &graph)
{
	auto v = boost::add_vertex(graph);
	auto task = std::make_shared<Task>(v);
	graph[v] = task;

	return v;
}

void DestroyTask(TaskGraph &graph, const TaskVertexId &taskVertexId) { boost::remove_vertex(taskVertexId, graph); }

TaskPtr GetTask(const TaskGraph &graph, const TaskVertexId &taskVertexId) { return graph[taskVertexId]; }

DependencyCreated CreateDependency(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId)
{
	auto edge = boost::add_edge(upstreamId, downstreamId, graph);

	try
	{
		// Check if adding the edge creates a cycle in the graph
		CycleDetectorVisistor visitor;
		boost::depth_first_search(graph, boost::visitor(visitor));
	}
	catch (CycleFoundException)
	{
		// if it does, remove it
		boost::remove_edge(upstreamId, downstreamId, graph);
		return DependencyCreated::CycleDetected;
	}

	if (edge.second)
	{
		return DependencyCreated::Created;
	}
	return DependencyCreated::AlreadyExists;
}

void DestroyDependency(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId)
{
	boost::remove_edge(upstreamId, downstreamId, graph);
}

std::list<TaskVertexId> GetDependenciesOfTask(const TaskGraph &graph, const TaskVertexId &taskId)
{
	std::list<TaskVertexId> dependencies;
	typename boost::graph_traits<TaskGraph>::in_edge_iterator iter, end;

	auto inEdges = boost::in_edges(taskId, graph);
	for (boost::tie(iter, end) = inEdges; iter != end; ++iter)
	{
		dependencies.push_back(boost::source(*iter, graph));
	}
	return dependencies;
}

std::list<TaskVertexId> GetDependentsOfTask(const TaskGraph &graph, const TaskVertexId &taskId)
{
	std::list<TaskVertexId> dependencies;
	typename boost::graph_traits<TaskGraph>::out_edge_iterator iter, end;

	auto inEdges = boost::out_edges(taskId, graph);
	for (boost::tie(iter, end) = inEdges; iter != end; ++iter)
	{
		dependencies.push_back(boost::target(*iter, graph));
	}
	return dependencies;
}

bool DependencyExists(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId)
{
	bool exists = !boost::add_edge(upstreamId, downstreamId, graph).second;
	if (!exists)
	{
		boost::remove_edge(upstreamId, downstreamId, graph);
	}
	return exists;
}

TaskGraphVertexIteratorPair TasksIterator(TaskGraph &graph) { return boost::vertices(graph); }

std::list<TaskVertexId> TopologicalSort(const TaskGraph &graph)
{
	std::list<TaskVertexId> tasks;
	boost::topological_sort(graph, std::front_inserter(tasks));
	return tasks;
}

}  // namespace pipes

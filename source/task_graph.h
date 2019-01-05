#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include <list>
#include <memory>

namespace pipes
{
class Task;
using TaskPtr = std::shared_ptr<Task>;

using TaskGraph = boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS, TaskPtr>;
using TaskVertexId = typename TaskGraph::vertex_descriptor;
using TaskGraphVertexIterator = boost::graph_traits<TaskGraph>::vertex_iterator;
using TaskGraphVertexIteratorPair = std::pair<TaskGraphVertexIterator, TaskGraphVertexIterator>;

enum class DependencyCreated
{
	Created,
	CycleDetected,
	AlreadyExists
};

TaskVertexId CreateTask(TaskGraph &graph);
void DestroyTask(TaskGraph &graph, const TaskVertexId &taskVertexId);
TaskPtr GetTask(const TaskGraph &graph, const TaskVertexId &taskVertexId);
DependencyCreated CreateDependency(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId);
void DestroyDependency(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId);
std::list<TaskVertexId> GetDependenciesOfTask(const TaskGraph &graph, const TaskVertexId &taskId);
std::list<TaskVertexId> GetDependentsOfTask(const TaskGraph &graph, const TaskVertexId &taskId);
TaskGraphVertexIteratorPair TasksIterator(TaskGraph &graph);
bool DependencyExists(TaskGraph &graph, const TaskVertexId &upstreamId, const TaskVertexId &downstreamId);

std::list<TaskVertexId> TopologicalSort(const TaskGraph &graph);

}  // namespace pipes

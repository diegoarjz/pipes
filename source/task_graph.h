#pragma once

#include <list>
#include <memory>

namespace pipes
{
class Task;
using TaskPtr = std::shared_ptr<Task>;

class TaskGraph
{
public:
	using TaskId = unsigned long;

	enum class DependencyCreated
	{
		Created,
		AlreadyExists
	};

	TaskGraph();
	~TaskGraph();

	TaskGraph(const TaskGraph& graph) = delete;
	TaskGraph& operator=(const TaskGraph& graph) = delete;

	TaskPtr CreateTask();
	void DestroyTask(TaskPtr task);

	DependencyCreated CreateDependency(TaskPtr upstreamTask, TaskPtr downstreamTask);
	void DestroyDependency(TaskPtr upstreamTask, TaskPtr downstreamTask);

	const std::list<TaskPtr>& GetTasks() const;
	std::list<TaskPtr> GetDependenciesOfTask(TaskPtr task);
	std::list<TaskPtr> GetDependentsOfTask(TaskPtr task);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pipes

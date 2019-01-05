#pragma once

#include "task_graph.h"

#include <memory>
#include <queue>

namespace pipes
{
class Task;
using TaskPtr = std::shared_ptr<Task>;

using TaskQueue = std::queue<TaskPtr>;

class Scheduler
{
public:
	Scheduler(uint32_t threadCount);
	~Scheduler();

	uint32_t GetThreadCount() const;
	TaskQueue& GetTaskQueueForThread(uint32_t threadIndex);

	void Schedule(TaskGraph& graph);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pipes

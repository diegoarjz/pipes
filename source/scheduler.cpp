#include "scheduler.h"

#include "task.h"
#include "task_graph.h"

#include <stack>

namespace pipes
{
class Scheduler::Impl
{
public:
	Impl(uint32_t threadCount) : m_threadCount(threadCount) {}
	~Impl() {}

	uint32_t GetThreadCount() const { return m_threadCount; }
	TaskQueue& GetTaskQueueForThread(uint32_t threadIndex) { return m_threadQueues[threadIndex]; }

	void Schedule(TaskGraph& graph)
	{
		auto topologicalSort = TopologicalSort(graph);
		m_threadQueues = std::vector<TaskQueue>(m_threadCount);
		std::vector<std::stack<TaskPtr>> stacks(m_threadCount);

		uint32_t queueIndex = 0;
		while (!topologicalSort.empty())
		{
			TaskVertexId taskId = topologicalSort.back();
			topologicalSort.pop_back();

			TaskPtr task = GetTask(graph, taskId);

			if (!stacks[queueIndex].empty() && !DependencyExists(graph, taskId, stacks[queueIndex].top()->GetId()))
			{
				queueIndex = (queueIndex + 1) % m_threadCount;
			}

			stacks[queueIndex].push(task);
		}

		for (uint32_t i = 0; i < m_threadCount; ++i)
		{
			while (!stacks[i].empty())
			{
				m_threadQueues[i].push(stacks[i].top());
				stacks[i].pop();
			}
		}
	}

private:
	uint32_t m_threadCount;
	std::vector<TaskQueue> m_threadQueues;
};  // namespace pipes

Scheduler::Scheduler(uint32_t threadCount) : m_implementation(std::make_unique<Impl>(threadCount)) {}
Scheduler::~Scheduler() {}

uint32_t Scheduler::GetThreadCount() const { return m_implementation->GetThreadCount(); }
TaskQueue& Scheduler::GetTaskQueueForThread(uint32_t threadIndex)
{
	return m_implementation->GetTaskQueueForThread(threadIndex);
}

void Scheduler::Schedule(TaskGraph& graph) { m_implementation->Schedule(graph); }
}  // namespace pipes

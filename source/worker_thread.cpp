#include "worker_thread.h"
#include "task.h"
#include "task_execution.h"

#include <chrono>
#include <thread>

#include <iostream>

namespace pipes
{
class WorkerThread::Impl
{
public:
	Impl(uint32_t threadId, TaskQueue &taskQueue) : m_threadId(threadId), m_taskQueue(taskQueue) {}

	void Start() { m_thread = std::thread(WorkerThread::Impl::dispatch, this); }

	void Join()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	uint32_t GetThreadId() const { return m_threadId; }

private:
	void run()
	{
		while (!m_taskQueue.empty())
		{
			TaskPtr taskToRun = m_taskQueue.front();

			auto execution = taskToRun->GetTaskExecution();
			std::chrono::high_resolution_clock::time_point start;
			std::chrono::high_resolution_clock::time_point end;
			if (execution != nullptr)
			{
				start = std::chrono::high_resolution_clock::now();
				execution->Execute();
				end = std::chrono::high_resolution_clock::now();

				auto elapsed = end - start;
				taskToRun->AddExecutionTime(elapsed);
			}

			m_taskQueue.pop();
		}
	}

	static void dispatch(void *ptr)
	{
		if (ptr == nullptr)
		{
			return;
		}

		static_cast<WorkerThread::Impl *>(ptr)->run();
	}

	uint32_t m_threadId;
	TaskQueue m_taskQueue;
	std::thread m_thread;
};

WorkerThread::WorkerThread(uint32_t threadId, TaskQueue &taskQueue)
    : m_implementation(std::make_unique<WorkerThread::Impl>(threadId, taskQueue))
{
}

WorkerThread::~WorkerThread() { m_implementation = nullptr; }

void WorkerThread::Start() { m_implementation->Start(); }
void WorkerThread::Join() { m_implementation->Join(); }
uint32_t WorkerThread::GetThreadId() const { return m_implementation->GetThreadId(); }
}  // namespace pipes

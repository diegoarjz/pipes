#pragma once

#include "scheduler.h"

namespace pipes
{
class WorkerThread
{
public:
	WorkerThread(uint32_t threadId, TaskQueue &taskQueue);
	~WorkerThread();

	void Start();
	void Join();

	uint32_t GetThreadId() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};

using WorkerThreadPtr = std::shared_ptr<WorkerThread>;
}  // namespace pipes

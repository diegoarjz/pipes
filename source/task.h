#pragma once

#include <chrono>
#include <memory>
#include <string>

namespace pipes
{
class TaskExecution;
using TaskExecutionPtr = std::shared_ptr<TaskExecution>;

class Task
{
public:
	using TaskId = unsigned long;

	Task(TaskId id);
	Task(TaskId id, const std::string &name);
	virtual ~Task();

	TaskId GetId() const;

	void SetTaskExecution(TaskExecutionPtr execution);
	TaskExecutionPtr GetTaskExecution() const;

	void SetName(const std::string &name);
	const std::string &GetName() const;

	void AddExecutionTime(std::chrono::high_resolution_clock::duration elapsed);
	std::chrono::high_resolution_clock::duration GetExecutionTime() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pipes

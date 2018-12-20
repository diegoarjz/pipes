#pragma once

#include <memory>

namespace pipes
{
class Task
{
public:
	using TaskId = unsigned long;

	Task(TaskId id);
	virtual ~Task();

	TaskId GetId() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pipes

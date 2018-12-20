#include "task.h"

namespace pipes
{
class Task::Impl
{
public:
	Impl(Task::TaskId id) : m_id(id) {}

	TaskId GetId() const { return m_id; }

private:
	Task::TaskId m_id;
};

Task::Task(TaskId id) : m_implementation(std::make_unique<Task::Impl>(id)) {}
Task::~Task() {}

Task::TaskId Task::GetId() const { return m_implementation->GetId(); }
}  // namespace pipes

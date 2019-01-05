#include "task.h"

#include "task_execution.h"

namespace pipes
{
class Task::Impl
{
public:
	Impl(Task::TaskId id) : m_id(id) {}
	Impl(Task::TaskId id, const std::string &name) : m_id(id), m_name(name) {}

	TaskId GetId() const { return m_id; }

	void SetTaskExecution(TaskExecutionPtr execution) { m_taskExecution = execution; }
	TaskExecutionPtr GetTaskExecution() const { return m_taskExecution; }

	void SetName(const std::string &name) { m_name = name; }
	const std::string &GetName() const { return m_name; }

	void AddExecutionTime(std::chrono::high_resolution_clock::duration elapsed) { m_duration = elapsed; }
	std::chrono::high_resolution_clock::duration GetExecutionTime() const { return m_duration; }

private:
	Task::TaskId m_id;
	std::string m_name;
	TaskExecutionPtr m_taskExecution;
	std::chrono::high_resolution_clock::duration m_duration;
};

Task::Task(TaskId id) : m_implementation(std::make_unique<Task::Impl>(id)) {}
Task::Task(TaskId id, const std::string &name) : m_implementation(std::make_unique<Task::Impl>(id, name)) {}
Task::~Task() {}

Task::TaskId Task::GetId() const { return m_implementation->GetId(); }
void Task::SetTaskExecution(TaskExecutionPtr execution) { m_implementation->SetTaskExecution(execution); }
TaskExecutionPtr Task::GetTaskExecution() const { return m_implementation->GetTaskExecution(); }

void Task::SetName(const std::string &name) { m_implementation->SetName(name); }
const std::string &Task::GetName() const { return m_implementation->GetName(); }

void Task::AddExecutionTime(std::chrono::high_resolution_clock::duration elapsed)
{
	m_implementation->AddExecutionTime(elapsed);
}

std::chrono::high_resolution_clock::duration Task::GetExecutionTime() const
{
	return m_implementation->GetExecutionTime();
}
}  // namespace pipes

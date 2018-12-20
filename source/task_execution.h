#pragma once

namespace pipes
{
class TaskExecution
{
public:
	TaskExecution();
	virtual ~TaskExecution();

	virtual void Execute() = 0;

private:
};
}  // namespace pipes

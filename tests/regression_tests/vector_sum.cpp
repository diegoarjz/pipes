#include <debug/graph_dot_writer.h>
#include <scheduler.h>
#include <task.h>
#include <task_execution.h>
#include <task_graph.h>
#include <worker_thread.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

class Sum : public pipes::TaskExecution
{
public:
	static std::mutex sMutex;

	Sum(std::vector<uint32_t> &vec, uint32_t left, uint32_t right) : m_vec(vec), m_left(left), m_right(right), m_sum(0)
	{
	}

	void Execute() override
	{
		if (m_partialSums.size() == 0)
		{
			for (uint32_t i = m_left; i <= m_right && i < m_vec.size(); ++i)
			{
				m_sum += m_vec[i];
			}
		}
		else
		{
			for (auto sum : m_partialSums)
			{
				m_sum += sum->GetSum();
			}
		}

		std::chrono::high_resolution_clock::duration w(std::chrono::milliseconds(rand() % 1000));
		std::this_thread::sleep_for(w);
	}

	void AddPartialSum(std::shared_ptr<Sum> partial) { m_partialSums.push_back(partial); }

	uint32_t GetSum() const { return m_sum; }

private:
	std::vector<uint32_t> &m_vec;
	uint32_t m_left;
	uint32_t m_right;

	uint32_t m_sum;
	std::vector<std::shared_ptr<Sum>> m_partialSums;
};

std::mutex Sum::sMutex;

struct Arguments
{
	Arguments() : threadCount(4), vecSize(1000), dependencyDepth(2) {}

	uint32_t threadCount;
	uint32_t vecSize;
	uint32_t dependencyDepth;

	std::string outGraphFile;
	std::string outTopologicalSort;
	std::string outScheduled;
};

Arguments ParseCommandLine(int argc, char *argv[]);

void CreateDependencyTree(pipes::TaskGraph &graph, std::vector<uint32_t> &vec, uint32_t depth, uint32_t maxDepth,
                          uint32_t leftIndex, uint32_t rightIndex, pipes::TaskPtr parentTask)
{
	if (depth == maxDepth || leftIndex == rightIndex)
	{
		return;
	}

	uint32_t midIndex = (leftIndex + rightIndex) / 2;

	pipes::TaskVertexId left = pipes::CreateTask(graph);
	pipes::TaskVertexId right = pipes::CreateTask(graph);

	pipes::CreateDependency(graph, left, parentTask->GetId());
	pipes::CreateDependency(graph, right, parentTask->GetId());

	auto leftExecution = std::make_shared<Sum>(vec, leftIndex, midIndex);
	auto rightExecution = std::make_shared<Sum>(vec, midIndex + 1, rightIndex);

	auto parentExecution = std::dynamic_pointer_cast<Sum>(parentTask->GetTaskExecution());
	pipes::GetTask(graph, left)->SetTaskExecution(leftExecution);
	pipes::GetTask(graph, right)->SetTaskExecution(rightExecution);

	CreateDependencyTree(graph, vec, depth + 1, maxDepth, leftIndex, midIndex, pipes::GetTask(graph, left));
	CreateDependencyTree(graph, vec, depth + 1, maxDepth, midIndex + 1, rightIndex, pipes::GetTask(graph, right));
}

int main(int argc, char *argv[])
{
	Arguments args = ParseCommandLine(argc, argv);

	std::vector<uint32_t> nums;
	for (uint32_t i = 0; i < args.vecSize; ++i)
	{
		nums.push_back(1);
	}

	pipes::TaskGraph graph;
	auto parentExecution = std::make_shared<Sum>(nums, 0, args.vecSize);
	pipes::TaskVertexId finalTask = pipes::CreateTask(graph);
	pipes::GetTask(graph, finalTask)->SetTaskExecution(parentExecution);
	pipes::GetTask(graph, finalTask)->SetName("Root");
	CreateDependencyTree(graph, nums, 0, args.dependencyDepth, 0, args.vecSize - 1, pipes::GetTask(graph, finalTask));

	pipes::Scheduler scheduler(args.threadCount);
	scheduler.Schedule(graph);

	if (!args.outGraphFile.empty())
	{
		pipes::debug::write_task_graph(graph, args.outGraphFile);
	}

	if (!args.outTopologicalSort.empty())
	{
		auto topological = TopologicalSort(graph);
		pipes::debug::write_topological_sort(graph, topological, args.outTopologicalSort);
	}

	if (!args.outScheduled.empty())
	{
		pipes::debug::write_scheduling(graph, scheduler, args.outScheduled);
	}

	std::vector<pipes::WorkerThreadPtr> workerThreads;

	auto start = std::chrono::high_resolution_clock::now();
	for (uint32_t i = 0; i < args.threadCount; ++i)
	{
		workerThreads.push_back(std::make_shared<pipes::WorkerThread>(i, scheduler.GetTaskQueueForThread(i)));

		workerThreads.back()->Start();
	}

	for (uint32_t i = 0; i < args.threadCount; ++i)
	{
		workerThreads[i]->Join();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = end - start;
	auto uSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

	assert(parentExecution->GetSum() == args.vecSize);
	std::cout << "Final sum: " << parentExecution->GetSum() << std::endl;
	std::cout << "Took " << uSeconds << " microseconds" << std::endl;

	return 0;
}

std::string get_value_from_command_line_option(uint32_t *index, char *argv[])
{
	std::string argument = argv[*index];
	std::size_t pos = argument.find_first_of('=');
	std::string value;
	if (pos != std::string::npos)
	{
		value = argument.substr(pos + 1);
	}
	else
	{
		value = argv[*index];
		*index += 1;
	}
	return value;
}

Arguments ParseCommandLine(int argc, char *argv[])
{
	Arguments args;

	for (uint32_t i = 1; i < argc; ++i)
	{
		std::string argument = argv[i];

		if (argument.find("--dependency-dept") == 0)
		{
			args.dependencyDepth = std::atoi(get_value_from_command_line_option(&i, argv).c_str());
			continue;
		}

		if (argument.find("--vec-size") == 0)
		{
			args.vecSize = std::atoi(get_value_from_command_line_option(&i, argv).c_str());
			continue;
		}

		if (argument.find("--graph-out") == 0)
		{
			args.outGraphFile = get_value_from_command_line_option(&i, argv);
			continue;
		}

		if (argument.find("--topological-out") == 0)
		{
			args.outTopologicalSort = get_value_from_command_line_option(&i, argv);
			continue;
		}

		if (argument.find("--scheduled-out") == 0)
		{
			args.outScheduled = get_value_from_command_line_option(&i, argv);
			continue;
		}

		if (argument.find("--schedule-thread-count") == 0)
		{
			args.threadCount = std::atoi(get_value_from_command_line_option(&i, argv).c_str());
			continue;
		}
	}

	return args;
}

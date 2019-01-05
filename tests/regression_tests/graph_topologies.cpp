#include <debug/graph_dot_writer.h>
#include <scheduler.h>
#include <task_graph.h>

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Arguments
{
	Arguments() : threadCount(4) {}

	std::set<std::string> tasks;
	std::vector<std::pair<std::string, std::string>> dependencies;

	std::string outGraphFile;
	std::string outTopologicalSort;
	std::string outScheduled;
	uint32_t threadCount;
};

Arguments ParseCommandLine(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	Arguments args = ParseCommandLine(argc, argv);

	std::map<std::string, pipes::TaskVertexId> taskMap;
	pipes::TaskGraph graph;
	for (std::string task : args.tasks)
	{
		taskMap[task] = CreateTask(graph);
	}

	for (auto dependency : args.dependencies)
	{
		CreateDependency(graph, taskMap[dependency.first], taskMap[dependency.second]);
	}

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
		pipes::Scheduler scheduler(args.threadCount);
		scheduler.Schedule(graph);
		pipes::debug::write_scheduling(graph, scheduler, args.outScheduled);
	}

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

		if (argument.find("--dep") == 0)
		{
			std::string value = get_value_from_command_line_option(&i, argv);
			std::size_t pos = value.find_first_of(',');
			std::string dependency = value.substr(0, pos);
			std::string dependent = value.substr(pos + 1);

			args.tasks.insert(dependency);
			args.tasks.insert(dependent);
			args.dependencies.push_back(std::make_pair(dependency, dependent));

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

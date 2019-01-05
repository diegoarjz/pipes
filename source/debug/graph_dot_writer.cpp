#include "graph_dot_writer.h"

#include "../task.h"

#include <fstream>

namespace pipes
{
namespace debug
{
void write_task_graph(TaskGraph &graph, const std::string &file_path)
{
	std::ofstream out_file(file_path);

	out_file << "digraph {" << std::endl;
	out_file << "\trankdir=LR;" << std::endl;

	for (auto iters = TasksIterator(graph); iters.first != iters.second; ++iters.first)
	{
		auto dependentTasks = GetDependentsOfTask(graph, *iters.first);
		for (auto dependent : dependentTasks)
		{
			out_file << "t_" << *iters.first << " -> t_" << dependent << "\n";
		}
	}

	out_file << "}";

	out_file.close();
}

void write_topological_sort(TaskGraph &graph, const std::list<TaskVertexId> &taskList, const std::string &file_path)
{
	std::ofstream out_file(file_path);

	out_file << "digraph {" << std::endl;
	out_file << "\trankdir=LR;" << std::endl;

	for (auto task : taskList)
	{
		auto dependentTasks = GetDependentsOfTask(graph, task);
		for (auto dependent : dependentTasks)
		{
			out_file << "t_" << task << " -> t_" << dependent << "\n";
		}
	}

	out_file << "{ rank=same; ";
	for (auto task : taskList)
	{
		out_file << "t_" << task << " ";
	}
	out_file << "}\n";
	out_file << "}";

	out_file.close();
}

std::string write_cluster(TaskQueue q, uint32_t threadId)
{
	std::stringstream cluster;

	if (q.empty())
	{
		return "";
	}

	cluster << "start -> t_" << q.front()->GetId() << "\n";
	cluster << "subgraph cluster_" << threadId << " {\n";
	cluster << "\tedge[style=invis]\n";

	while (!q.empty())
	{
		auto task = q.front();
		q.pop();

		cluster << "t_" << task->GetId();

		if (!q.empty())
		{
			cluster << " -> ";
		}
	}
	cluster << "\nlabel = \"thread " << threadId << "\";\n}\n";

	return cluster.str();
}

void write_scheduling(TaskGraph &graph, Scheduler &scheduler, const std::string &file_path)
{
	std::ofstream out_file(file_path);

	out_file << "digraph {" << std::endl;
	out_file << "\trankdir=LR;" << std::endl;

	std::vector<TaskQueue> queues;
	std::vector<std::string> clusters;

	for (uint32_t i = 0; i < scheduler.GetThreadCount(); ++i)
	{
		queues.push_back(scheduler.GetTaskQueueForThread(i));

		clusters.push_back(write_cluster(queues.back(), i));
	}

	for (auto cluster : clusters)
	{
		out_file << cluster;
	}

	for (auto iters = TasksIterator(graph); iters.first != iters.second; ++iters.first)
	{
		auto dependentTasks = GetDependentsOfTask(graph, *iters.first);
		for (auto dependent : dependentTasks)
		{
			out_file << "t_" << *iters.first << " -> t_" << dependent << "\n";
		}
	}

	out_file << "}";
}

}  // namespace debug
}  // namespace pipes

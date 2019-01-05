#pragma once

#include "../scheduler.h"
#include "../task_graph.h"

#include <string>

namespace pipes
{
namespace debug
{
void write_task_graph(TaskGraph &graph, const std::string &file_path);
void write_topological_sort(TaskGraph &graph, const std::list<TaskVertexId> &list, const std::string &file_path);
void write_scheduling(TaskGraph &graph, Scheduler &scheduler, const std::string &file_path);
}  // namespace debug
}  // namespace pipes

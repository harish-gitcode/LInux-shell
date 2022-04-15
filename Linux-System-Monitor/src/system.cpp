#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;

Processor &System::Cpu() { return cpu_; }

vector<Process> &System::Processes()
{
    vector<int> pids = LinuxParser::Pids();
    vector<Process> temp;

    for (auto x : pids)
    {
        Process newProcess(x);
        temp.push_back(newProcess);
    }
    std::sort(temp.begin(), temp.end(),
              [](Process &a, Process &b)
              { return a.CpuUtilization() > b.CpuUtilization(); });
    processes_ = temp;
    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }
long int System::UpTime() { return LinuxParser::UpTime(); }
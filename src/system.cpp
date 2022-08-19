#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }
vector<Process>& System::Processes() {
    set<int> uniqIds;
    std::vector<int> Procids = LinuxParser::Pids();
    for (Process process : proc_) { uniqIds.insert(process.Pid());}
    for (int id: Procids) { if (uniqIds.find(id) == uniqIds.end()) { proc_.emplace_back(Process(id));}}
    for (Process& process: proc_) { process.CpuUtilization();}
    std::sort(proc_.begin(), proc_.end(), std::greater<Process>());
    return proc_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }
long int System::UpTime() { return LinuxParser::UpTime(); }

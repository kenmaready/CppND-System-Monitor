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
using std::to_string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO (Completed): Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // clear out current vector of processes before rebuilding:
    clearProcesses_();

    // get the pids from the /proc directory and then, for each, 
    // add a new Process object to the Processes vector
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        processes_.emplace_back(pid);
    }

    // sort and return:
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }

// Helper function to clear processes vector:
void System::clearProcesses_() { processes_ = {}; }
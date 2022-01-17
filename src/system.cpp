#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

// TODO: Return the system's CPU
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

// TODO (Completed): Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO (Completed): Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO (COmpleted): Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO (Completed): Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO (Completed): Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO (Completed): Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }

// new function to clear processes_ vector:
void System::clearProcesses_() { processes_ = {}; }
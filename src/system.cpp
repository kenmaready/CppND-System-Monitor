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
using std::vector;
using std::cout;
using std::endl;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO (Completed): Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // get the pids from the /proc directory and then, for each, 
    // add a new Process object to the Processes vector
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        Process process;
        process.pid_ = pid;

        // get the user:
        process.user_ = LinuxParser::User(pid);
        process.command_ = LinuxParser::Command(pid);
        process.cpuutilization_ = LinuxParser::CpuUtilization(pid);

        processes_.emplace_back(process);
    }

    auto sort_by_utilization = [](Process a, Process b){ return a.CpuUtilization() > b.CpuUtilization(); };
    std::sort(processes_.begin(), processes_.end(), sort_by_utilization);

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
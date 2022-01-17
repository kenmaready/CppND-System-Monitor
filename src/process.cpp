#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO (COMPLETED): Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuutilization_; }

// TODO (Completed): Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
int Process::Ram() { return ram_; }

// TODO (Completed): Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
    // sort by cpu utilization (use "V" so that vector is in descending order):
    return cpuutilization_ > a.cpuutilization_; 
}
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor for Process class based on pid:
Process::Process(int pid): pid_(pid) {
    pid_ = pid;

    // get the information to complete the Process object 
    // from the proc files:
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
    cpuutilization_ = LinuxParser::CpuUtilization(pid);
    ram_ = LinuxParser::Ram(pid);
    uptime_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpuutilization_; }

//  Return the command that generated this process
//  Truncate first if necessary
string Process::Command() {

    if ((int)(command_.length()) > MAX_COMMAND_LENGTH_) {
        return command_.substr(0, MAX_COMMAND_LENGTH_) + "...";
    } else return command_; 
}

// Return this process's memory utilization
int Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    // sort by cpu utilization (use "V" so that vector is in descending order):
    return cpuutilization_ > a.cpuutilization_; 
}
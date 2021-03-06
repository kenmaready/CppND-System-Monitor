#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Target Keys
const std::string kPrettyName{"PRETTY_NAME"};
const std::string kMemTotal{"MemTotal:"};
const std::string kMemFree{"MemFree:"};
const std::string kProcesses("processes");
const std::string kProcsRunning("procs_running");
const std::string kVmRSS("VmRSS:");
const std::string kUid("Uid:");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<unsigned long int> CpuUtilization();
unsigned long long int Jiffies(const std::vector<unsigned long int>&);
unsigned long long int ActiveJiffies(const std::vector<unsigned long int>&);
unsigned long int ActiveJiffies(int pid);
unsigned long long int IdleJiffies(const std::vector<unsigned long int>&);

// Processes
std::string Command(int pid);
int Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
float CpuUtilization(int pid);
};  // namespace LinuxParser

#endif
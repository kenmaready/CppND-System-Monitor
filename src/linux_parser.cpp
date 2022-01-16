#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

float kHertz = sysconf(_SC_CLK_TCK);

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal {};
  float memFree {};

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if (stream.is_open()) {
    int counter = 0;
    string line, key, value;
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = stof(value);
        counter++;
      } else if (key == "MemFree:") {
        memFree = stof(value);
        counter++;
      }
      if (counter > 2) break;
    }
  }
  
  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string line, uptime;

    std::ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) {
      std::getline(stream, line);
      std::stringstream linestream(line);
      linestream >> uptime;
      // cout << "uptime: " << stol(uptime) << endl;
    }

  return stol(uptime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {

    while (std::getline(stream, line)) {
      std::stringstream linestream(line);
      linestream >> key >> value;
      // cout << "key: " << key << " | value: " << value;
      if (key == "processes") return stoi(value);
    }
  }

  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;

    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {

    while (std::getline(stream, line)) {
      std::stringstream linestream(line);
      linestream >> key >> value;
      // cout << "key: " << key << " | value: " << value;
      if (key == "procs_running") return stoi(value);
    }
  }

  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    getline(stream, line);
    return line;
  } 
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO (Completed): Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          return uid;
        }
      }
    }
  }

  return uid; }

// TODO (Completed): Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  // first, get the uid associated with process:
  string uid = Uid(pid);
  string value, user, x, uuid;
  string line;

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uuid) {
        if (uuid == uid) {
          return user;
        }
      }
    }
  }

  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 30492; }


// TODO: Read and return CPU utilization for a specific process
float LinuxParser::CpuUtilization(int pid) { 
  // float cpuutlization;
  std::string line, token;
  int utime, stime, cutime, cstime, starttime;
  float cpuutilization = 0.0;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    // cout << "stat: " << line << endl;
    std::istringstream linestream(line);
    int counter = 1;
    while (linestream >> token) {
      switch (counter) {
        case 14:
          utime = stof(token);
          continue;
        case 15:
          stime = stof(token);
          continue;
        case 16:
          cutime = stof(token);
          continue;
        case 17:
          cstime = stof(token);
          continue;
        case 22:
          starttime = stof(token);
          continue;
        default:
          continue;
      }
      counter ++;
    }

    // cout << "stats - utime:" << utime << " stime: " << stime << " cutime: " << cutime << " cstime: " << cstime << " starttime: " << starttime;

    int c_total_time = cutime + cstime;
    int total_time = utime + stime + c_total_time;
    float seconds = total_time - (starttime / kHertz);
    cpuutilization = (total_time / kHertz) / seconds;

    return cpuutilization;
  }



  return 0.4; 
}
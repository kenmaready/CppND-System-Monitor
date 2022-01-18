#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::getline;

// Constants
const float kHertz = sysconf(_SC_CLK_TCK);


// Read and return OS name from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;

  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kPrettyName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }

  return value;
}

// Read and return  kernel name from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;

  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal {};
  float memFree {};

  ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if (stream.is_open()) {
    int counter = 0;
    string line, key, value;
    while (getline(stream,line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == kMemTotal) {
        memTotal = stof(value);
        counter++;
      } else if (key == kMemFree) {
        memFree = stof(value);
        counter++;
      }
      if (counter > 2) break;
    }
  }

  
  return (memTotal - memFree) / memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
    string line, uptime;

    ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::stringstream linestream(line);
      linestream >> uptime;
    }

  return stol(uptime); 
}

// Read and return the number of jiffies for the system
unsigned long long int LinuxParser::Jiffies(const std::vector<unsigned long int> &jiffies) { 
  return ActiveJiffies(jiffies) + IdleJiffies(jiffies); 
}

// Read and return the number of active jiffies for a PID
unsigned long int LinuxParser::ActiveJiffies(int pid) { 
  string line, token;
  long int jiffies = 0;

  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    int counter = 1;
    while (linestream >> token) {
      if (counter >= 14 && counter <= 15) {
        jiffies += stoi(token);
      } else if (counter > 17) break;
      counter ++;
    }
  }

  return jiffies; 
}

// Read and return the number of active jiffies for the system
unsigned long long int LinuxParser::ActiveJiffies(const std::vector<unsigned long int> & jiffies) { 
  // active = user + nice + system + ird + softirq + steal:
  unsigned long long int active = jiffies.at(0) + jiffies.at(1) + jiffies.at(2) + jiffies.at(5) + jiffies.at(6) + jiffies.at(7);
  
  return active; 
}

//  Read and return the number of idle jiffies for the system
unsigned long long int LinuxParser::IdleJiffies(const std::vector<unsigned long int> &jiffies) {
  // idle + iowait:
  unsigned long long int idle = jiffies.at(3) + jiffies.at(4); 
  return idle;
}

// Read and return CPU utilization
std::vector<unsigned long int> LinuxParser::CpuUtilization() { 
  std::vector<unsigned long int> jiffies;
  string line, token;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::stringstream linestream(line);
    while (linestream >> token) {
      if (token == "cpu") continue;
      else jiffies.emplace_back(stol(token));
    }
  }
  
  return jiffies; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;

  ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;
      if (key == kProcesses) return stoi(value);
    }
  }

  return 0; 
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;

    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {

    while (std::getline(stream, line)) {
      std::stringstream linestream(line);
      linestream >> key >> value;
      if (key == kProcsRunning) return stoi(value);
    }
  }

  return 0; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    getline(stream, line);
    return line;
  } 
  return line; 
}

// Read and return the memory used by a process
int LinuxParser::Ram(int pid) { 
  string line, key, value;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      // NOte: amusing VmRSS here instead of VmSize at suggestion of
      // first project reviewer:
      if (key == kVmRSS) {
        // convert to MB and return
        return stoi(value) / 1000;
      }
    }
  }
  return 0;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kUid) {
          uid = value;
          return uid;
        }
      }
    }
  }

  return uid; }

// Read and return the user associated with a process
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

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, token;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      int i {0};
      while(linestream >> token) {
        if (i==21) {
          return stol(token) / kHertz;
        }
        i++;
      }
    }
  }
  
  return 0; 
}


// Read and return CPU utilization for a specific process
float LinuxParser::CpuUtilization(int pid) { 
  long int processJiffies = ActiveJiffies(pid);
  long int systemJiffies = Jiffies(CpuUtilization());

  return (processJiffies * 1.0) / systemJiffies;
}
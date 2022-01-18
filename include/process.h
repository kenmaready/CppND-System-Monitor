#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);

  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  int Ram();                               
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
    int pid_;
    std::string user_;
    std::string command_;
    float cpuutilization_;
    int ram_;
    long int uptime_;
    int MAX_COMMAND_LENGTH_ = 50;

  friend class System;
};

#endif
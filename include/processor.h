#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <iostream>

using std::cout;
using std::endl;

class Processor {
 public:
  float Utilization();  

 private:
    // currentJiffies must be initialized to vector of zeros    
    std::vector<unsigned long int> currentJiffies_ = std::vector<unsigned long int>(10);
    float currentUtilization_;
};

#endif
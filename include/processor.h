#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <iostream>

using std::cout;
using std::endl;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    // initialize to vector of zeros    
    std::vector<unsigned long int> currentJiffies_ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float currentUtilization_;
};

#endif
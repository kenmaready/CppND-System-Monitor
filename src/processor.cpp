#include "processor.h"
#include "linux_parser.h"

using LinuxParser::ActiveJiffies;
using LinuxParser::Jiffies;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    // store "previous current" jiffy vector in variable for use in determinig
    // incremental changes to jiffies (tof better current measure of SPI usage):
    std::vector<unsigned long int> prevJiffies = currentJiffies_;

    // get new jiffies
    currentJiffies_ = LinuxParser::CpuUtilization();
    
    // get incremental change in total and active jiffies
    unsigned long long int deltaActive = ActiveJiffies(currentJiffies_) - ActiveJiffies(prevJiffies);
    unsigned long long int deltaTotal = Jiffies(currentJiffies_) - Jiffies(prevJiffies);    
    
    // divide incremenetal active by incremenetal total to get current usage
    float currentUtilization_ = (deltaActive * 1.0) / deltaTotal;

    return currentUtilization_; 
}
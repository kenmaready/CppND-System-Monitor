#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Helper function to display tim in HH:MM:SS format
// Could also use cstd::hrono library as an alternative
string Format::ElapsedTime(long seconds) {
    int kMinute = 60;
    int kHour = 60 * kMinute;
    int hours, minutes;

    hours = seconds / kHour;
    seconds = seconds % kHour;
    minutes = seconds / kMinute;
    seconds = seconds % kMinute;
    return (hours >= 10 ? "" : "0") + to_string(hours) + ":" + (minutes >= 10 ? "" : "0") + to_string(minutes) + ":" + (seconds >= 10 ? "" : "0") + to_string(seconds); 
}
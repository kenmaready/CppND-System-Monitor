#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int kMinute = 60;
    int kHour = 60 * kMinute;
    int hours, minutes;

    hours = seconds / kHour;
    seconds = seconds % kHour;
    minutes = seconds / kMinute;
    seconds = seconds % kMinute;
    return to_string(hours) + ":" + (minutes >= 10 ? "" : "0") + to_string(minutes) + ":" + (seconds >= 10 ? "" : "0") + to_string(seconds); 
}
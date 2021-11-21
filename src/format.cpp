#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600L;
    seconds -= hours * 3600L;
    long minutes = seconds / 60L;
    seconds -= minutes * 60L;

    std::stringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours << 
    std::setw(1) << ":" << 
    std::setw(2) << std::setfill('0') << minutes << 
    std::setw(1) << ":" << 
    std::setw(2) << std::setfill('0') << seconds;
    return stream.str();
}
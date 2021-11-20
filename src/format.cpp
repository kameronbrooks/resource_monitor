#include <string>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long sec = seconds % 60L;
    long minutes = (seconds / 60L) % 60L;
    long hours = (seconds / 3600L);
    std::stringstream stream;
    stream << hours << ":" << minutes << ":" << sec ;
    return stream.str();
}
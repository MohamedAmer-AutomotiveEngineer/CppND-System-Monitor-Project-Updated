#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    unsigned char hour = 0, minute = 0, second = 0;
    hour = seconds / 3600;
    minute = (seconds - 3600*hour)/60;
    second = (seconds - (3600*hour) - (60*minute));
    return (std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second));
}

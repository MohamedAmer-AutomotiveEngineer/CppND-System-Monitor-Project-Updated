#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    unsigned char hour, minutes;
    hour = seconds / 3600;
    minutes = (seconds - 3600*hour)/60;
    seconds = (seconds - (3600*hour) - (60*minutes));
    return (std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds));
}

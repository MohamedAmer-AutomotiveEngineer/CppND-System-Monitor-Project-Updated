#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    unsigned char hour, minutes;
    string hour_s, minutes_s, seconds_s;
    seconds = seconds % (24 * 3600); 
    hour = seconds; // 3600
    seconds %= 3600;
    minutes = seconds; // 60
    seconds %= 60;
    return (std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds));
}

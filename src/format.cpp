#include <string>
#include "format.h"
using std::string;

/* helper function to get correct printing of time data */
void itoa(int num, char * str) 
{
    str[0] = (num / 10) + '0';
    str[1] = (num % 10) + '0';
    str[2] = '\0'; // Append string terminator 
}
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    unsigned char hour = 0, minute = 0, second = 0;
    char str[3];
    string buffer;
    hour = seconds / 3600;
    itoa(hour, str);
    buffer.append(str);
    buffer.push_back(':');
    minute = (seconds - 3600*hour)/60;
    itoa(minute, str);
    buffer.append(str);
    buffer.push_back(':');
    second = (seconds - (3600*hour) - (60*minute));
    itoa(second, str);
    buffer.append(str);
    return buffer;
}

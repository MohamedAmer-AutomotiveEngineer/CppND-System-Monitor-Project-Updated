#include <string>
#include "format.h"
using std::string;

/* helper function from https://www.geeksforgeeks.org/ */
// This function swaps values pointed by xp and yp 
void swap(char *xp, char *yp) 
{ 
    char temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}
/* helper function from https://www.geeksforgeeks.org/ */
void reverse(char str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    {
        swap((str+start), (str+end)); 
        start++; 
        end--; 
    } 
}
/* helper function from https://www.geeksforgeeks.org/ */
void itoa(int num, char * str) 
{
    char i = 0;
    for(i = 0; i < 2; i++)
    {
        str[i] = '0';
    }
    str[i] = '\0';
    i = 0;
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % 10; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/10; 
    }
    str[i] = '\0'; // Append string terminator 
    // Reverse the string 
    reverse(str, i);
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
    minute = (seconds - 3600*hour)/60;
    itoa(minute, str);
    buffer.append(str);
    second = (seconds - (3600*hour) - (60*minute));
    itoa(second, str);
    buffer.append(str);
    return buffer;
}

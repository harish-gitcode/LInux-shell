#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds)
{
    long mins = seconds / 60;
    seconds %= 60;
    long hours = mins / 60;
    mins %= 60;

    string time = to_string(hours) + ":" + to_string(mins) + ":" + to_string(seconds);
    return time;
}
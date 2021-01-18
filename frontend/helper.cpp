#include "helper.h"

class helperData : public QSharedData
{
public:

};



string helper::timestamp_converter(string timestamp){
    int y,M,d,h,m;
    float s;
    timestamp.erase(0,1);
    timestamp.erase(timestamp.end() - 1, timestamp.end());
    sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);
    tm time;
    time.tm_year = y - 1900; // Year 2
    time.tm_mon = M - 1;     // 0-11
    time.tm_mday = d;        // 1-31
    time.tm_hour = h;        // 0-23
    time.tm_min = m;         // 0-59
    string _time = to_string(time.tm_hour) + ":" + to_string(time.tm_min);
    return _time;
}

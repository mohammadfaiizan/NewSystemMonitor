#include <string>
#include <iomanip>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    long hours, min, sec, remSec;
    hours = seconds/3600;
    remSec = seconds%3600;
    min = remSec/60;
    remSec = remSec%60;
    sec = remSec;
    std::stringstream strSt;
  
  	strSt << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << min << ":" << std::setw(2) << std::setfill('0') << sec;
  	return strSt.str();
}
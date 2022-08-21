#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int id) :
    _id(id)
{
    _user = LinuxParser::User(_id);
    _cmd = LinuxParser::Command(_id);
}


int Process::Pid() { return _id; }
float Process::CpuUtilization() { 
    _cpuUtil = (1.0*LinuxParser::ActiveJiffies(_id))/(LinuxParser::UpTime() - LinuxParser::UpTime(_id));
    return (_cpuUtil);}
string Process::Command() { return _cmd; }
string Process::Ram() { return LinuxParser::Ram(_id); }
string Process::User() { return _user; }
long int Process::UpTime() { return (LinuxParser::UpTime() - LinuxParser::UpTime(_id)); }
bool Process::operator>(Process const& a) const { return this->_cpuUtil > a._cpuUtil; }

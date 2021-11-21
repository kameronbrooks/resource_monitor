#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    _pid = pid;
    _previousJiffies = 0;
    _previousCpuJiffies = 0;
}

int Process::Pid() { 
    return _pid; 
}

float Process::CpuUtilization() {

    auto proc_jiffies = LinuxParser::ActiveJiffies(_pid);
    auto system_jiffies = LinuxParser::Jiffies();

    auto proc_delta = proc_jiffies - _previousJiffies;
    auto system_delta = system_jiffies - _previousCpuJiffies;

    _previousJiffies = proc_jiffies;
    _previousCpuJiffies = system_jiffies;

    _cpuUtilization = (float)proc_delta / (float)system_delta;

    return _cpuUtilization;
}

string Process::Command() { 
    return LinuxParser::Command(_pid); 
}

string Process::Ram() { 
    return LinuxParser::Ram(_pid);
}

string Process::User() { 
    return LinuxParser::User(_pid);
}

long int Process::UpTime() { 
    return LinuxParser::UpTime(_pid);
}

bool Process::operator<(Process const& a) const { 
    return _cpuUtilization > a._cpuUtilization;
}
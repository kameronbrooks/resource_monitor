#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System() {
    
}


Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() { 
    std::vector<Process> temp;
    for(auto pid: LinuxParser::Pids()) {
        bool found = false;
        for(auto& proc: processes_) {
            if(proc.Pid() == pid) {
                if(proc.IsValid())
                    temp.emplace_back(proc);
                found = true;
                break;
            }
        }
        if(!found) {
            Process proc(pid);
            if(proc.IsValid())
                temp.emplace_back(pid);
        }
    }
    std::sort(temp.begin(), temp.end());
    processes_ = temp;
    return processes_; 
}


std::string System::Kernel() { return LinuxParser::Kernel(); }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }


long int System::UpTime() { return LinuxParser::UpTime(); }
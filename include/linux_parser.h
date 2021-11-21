#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

std::vector<std::string> string_split(std::string in, char delim);

template<class T>
bool search_kvp(std::string const &filename, std::string const & key, T& val) {
  std::string line, k;

  std::ifstream filestream(filename);
  if(!filestream.is_open())
    return false;
  
  while(std::getline(filestream, line)) {
    std::stringstream sstream(line);
    sstream >> k;
    if(k == key) {
      sstream >> val;
      return true;
    }
  }
  return false;
}
template<class T>
bool get_file_val(std::string const &filename, T& val) {
  std::string line;

  std::ifstream filestream(filename);
  if(!filestream.is_open())
    return false;
  std::getline(filestream, line);
  std::stringstream sstream(line);
  sstream >> val;
  return true;
  
}
// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();


// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<long> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif
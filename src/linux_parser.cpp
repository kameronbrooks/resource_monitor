#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::vector<std::string> LinuxParser::string_split(std::string in, char delim) {
  int start = 0;
  int end = 0;
  int len = in.size();
  std::vector<std::string> output;

  while(end < len) {
    if(in[end] == delim) {
      if(end == start)
        output.push_back("");
      else
        output.push_back(in.substr(start, end-start));
      start = end+1;
    }
    ++end;
  }

  return output;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  long memTotal = 0;
  long memFree = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kMeminfoFilename;
  std::getline(filestream, line);
  std::stringstream sstream(line);
  sstream >> key >> memTotal;

  std::getline(filestream, line);
  sstream.str(line);
  sstream >> key >> memFree;
  
  
  return (float)(memTotal-memFree) / (float)memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kUptimeFilename;
  
  std::string line;
  std::string key;
  long uptime = 0;
  std::getline(filestream, line);
  std::stringstream sstream(line);
  sstream >> uptime;
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::vector<long> data = CpuUtilization();
  long sum = 0;
  for(long i: data) {
    sum += i;
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/stat");
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + "/" + std::to_string(pid) + "/stat";
  std::stringstream buffer;
  buffer << filestream.rdbuf();
  std::string intBuffer[25];
  
  for(int i= 0; i < 25; ++i) {
    buffer >> intBuffer[i];
  }
  return std::stol(intBuffer[13]) + std::stol(intBuffer[14]) + std::stol(intBuffer[15]) + std::stol(intBuffer[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<long> data = CpuUtilization();
  return data[CPUStates::kUser_] + data[CPUStates::kSystem_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<long> data = CpuUtilization();
  return data[CPUStates::kIdle_] + data[CPUStates::kIOwait_];
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kStatFilename;
  
  std::string line;
  std::string temp;
  std::vector<long> output(10);

  while(std::getline(filestream, line)) {
    std::stringstream sstream(line);
    sstream >> temp;
    for(int i = 0; i < 10; ++i) {
      sstream >> output[i];
    }
  }

  return output;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kStatFilename;
  
  std::string line;
  std::string key;
  int totalProcs = 0;
  while(std::getline(filestream, line)) {
    std::stringstream sstream(line);
    sstream >> key;
    if (key == "processes") {
      sstream >> totalProcs;
      return totalProcs;
    }
  }
  return totalProcs;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kStatFilename;
  
  std::string line;
  std::string key;
  int totalProcs = 0;
  while(std::getline(filestream, line)) {
    std::stringstream sstream(line);
    sstream >> key;
    if (key == "procs_running") {
      sstream >> totalProcs;
      return totalProcs;
    }
  }
  return totalProcs;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/stat");
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kUptimeFilename;
  std::stringstream buffer;
  buffer << filestream.rdbuf();
  std::string temp;
  std::string com;
  buffer >> temp >> com;
  return com;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/status");
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + kUptimeFilename;
  std::string line;
  std::string key;
  std::string val;
  while(std::getline(filestream,line)) {
    std::stringstream sstream(line);
    sstream >> key >> val;
    if(key=="VmSize:") {
      int mb = std::stoi(val) / 1024;
      return std::to_string(mb);
    }

  }
  return val;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/status");
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + "/" + std::to_string(pid) + "/status";
  std::string line;
  std::string key;
  std::string val;
  while(std::getline(filestream,line)) {
    std::stringstream sstream(line);
    sstream >> key >> val;
    if(key=="Uid:")
      return val;

  }
  return val;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(!filestream.is_open())
    throw "Failed to open file: " + kPasswordPath;
  std::string line;
  while(std::getline(filestream, line)) {
    std::vector<std::string> tokens = string_split(line, ':');
    if(tokens.size() < 3)
      continue;
    
    if(uid == tokens[2])
      return tokens[0];
    
  }
  throw "User not found";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/stat");
  if(!filestream.is_open())
    throw "Failed to open file: " + kProcDirectory + "/" + std::to_string(pid) + "/stat";
  std::stringstream buffer;
  buffer << filestream.rdbuf();

  std::string temp;
  for(int i= 0; i < 22; ++i)
    buffer >> temp;

  return UpTime() - std::stol(temp) / 100;
}

#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
  System system;
  NCursesDisplay::Display(system);
  /*
  while(true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(auto& proc: system.Processes()) {
      proc.CpuUtilization();
    }
  }
  */
  //std::cout << system.Cpu().Utilization() << std::endl;
  //std::cout << LinuxParser::ActiveJiffies(1061) << std::endl;
}
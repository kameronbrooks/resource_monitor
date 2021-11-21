#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
  System system;
  NCursesDisplay::Display(system);
  
}
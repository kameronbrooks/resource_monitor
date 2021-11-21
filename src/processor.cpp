#include "processor.h"
#include "linux_parser.h"
#include <iostream>

Processor::Processor() : _previous(10) {
    for(auto & i: _previous) {
        i = 0;
    }
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    auto values = LinuxParser::CpuUtilization();

    long idle = values[LinuxParser::kIdle_] + values[LinuxParser::kIOwait_];
    long prev_idle = _previous[LinuxParser::kIdle_] + _previous[LinuxParser::kIOwait_];

    long non_idle = values[LinuxParser::kUser_] + values[LinuxParser::kNice_] + values[LinuxParser::kSystem_] + values[LinuxParser::kIRQ_] + values[LinuxParser::kSoftIRQ_] + values[LinuxParser::kSteal_];
    long prev_non_idle = _previous[LinuxParser::kUser_] + _previous[LinuxParser::kNice_] + _previous[LinuxParser::kSystem_] + _previous[LinuxParser::kIRQ_] + _previous[LinuxParser::kSoftIRQ_] + _previous[LinuxParser::kSteal_];

    long total = idle + non_idle;
    long prev_total = prev_idle + prev_non_idle;

    long total_delta = total - prev_total;
    long idle_delta = idle - prev_idle;



    float utilization = (float)(total_delta - idle_delta) / (float)(total_delta);
    _previous = values;

    return utilization;

}
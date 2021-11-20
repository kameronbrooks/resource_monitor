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

    int idle = values[LinuxParser::kIdle_] + values[LinuxParser::kIOwait_];
    int prev_idle = _previous[LinuxParser::kIdle_] + _previous[LinuxParser::kIOwait_];

    int non_idle = values[LinuxParser::kUser_] + values[LinuxParser::kNice_] + values[LinuxParser::kSystem_] + values[LinuxParser::kIRQ_] + values[LinuxParser::kSoftIRQ_] + values[LinuxParser::kSteal_];
    int prev_non_idle = _previous[LinuxParser::kUser_] + _previous[LinuxParser::kNice_] + _previous[LinuxParser::kSystem_] + _previous[LinuxParser::kIRQ_] + _previous[LinuxParser::kSoftIRQ_] + _previous[LinuxParser::kSteal_];

    int total = idle + non_idle;
    int prev_total = prev_idle + prev_non_idle;

    float utilization = (float)(non_idle - prev_non_idle) / (float)(total - prev_total);
    _previous = values;

    return utilization;

}
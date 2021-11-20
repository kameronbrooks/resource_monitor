#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
public:
    float Utilization();  // TODO: See src/processor.cpp

    Processor();

    // TODO: Declare any necessary private members
private:
    std::vector<long> _previous;
};

#endif
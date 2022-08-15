#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization

Processor::Processor() :
PIdle_(LinuxParser::IdleJiffies()),
PNonIdle_(LinuxParser::ActiveJiffies()),
PTotal_(PIdle_ + PNonIdle_) {}



float Processor::Utilization() { 
  const long currIdle = LinuxParser::IdleJiffies(); 
    const long currNonIdle = LinuxParser::ActiveJiffies(); 
    const long currTotal = currIdle + currNonIdle;

    const long deltaIdle = currIdle - PIdle_;
    const long deltaTotal = currTotal - PTotal_;

    PIdle_ = currIdle;
    PNonIdle_ = currNonIdle;
    PTotal_ = currTotal;
    
    const float PerCPU = deltaTotal == 0 ? 0.0f : (deltaTotal - deltaIdle)*1.0/deltaTotal;
    return PerCPU; 
}
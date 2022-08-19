#include "processor.h"
#include "linux_parser.h"

Processor::Processor() :
PIdle_(LinuxParser::IdleJiffies()),
PNonIdle_(LinuxParser::ActiveJiffies()),
PTotal_(PIdle_ + PNonIdle_) {}

float Processor::Utilization() { 
  	long currIdle = LinuxParser::IdleJiffies(); 
    long currNonIdle = LinuxParser::ActiveJiffies(); 
    long currTotal = currIdle + currNonIdle;
    long diffIdle = currIdle - PIdle_;
    long diffTotal = currTotal - PTotal_;
    PIdle_ = currIdle;
    PNonIdle_ = currNonIdle;
    PTotal_ = currTotal;
    return diffTotal == 0 ? 0.0f : (diffTotal - diffIdle)*1.0/diffTotal;
}

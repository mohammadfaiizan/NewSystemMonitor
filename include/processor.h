#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();
 private:
  	long PTotal_;
  	long PIdle_;
    long PNonIdle_;
};

#endif
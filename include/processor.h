#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();
  
 private:
  	long PIdle_;
    long PNonIdle_;
    long PTotal_;
};

#endif

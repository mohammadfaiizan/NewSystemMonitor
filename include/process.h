#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(const int id);
  int Pid();  							   
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();  
  bool operator>(Process const& a) const;

  
 private:
  int _id;
  std::string _user;
  std::string _cmd;
  float _cpuUtil;
};

#endif

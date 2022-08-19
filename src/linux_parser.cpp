#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() { 
  std::string path(kProcDirectory + kMeminfoFilename);
  float MemTotal;
  float MemFree;
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      std::string key;
      linestream >> key;
      if (key == "MemTotal:"){
      	linestream >> MemTotal;
      	}
      if (key == "MemFree:"){
      	linestream >> MemFree;
      	}
      }
    }
    return MemTotal = (MemTotal-MemFree)/MemTotal;
}


long LinuxParser::UpTime() { 
  long uptimeSecond;
  std::string path(kProcDirectory + kUptimeFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
    filestream >> uptimeSecond;
  } 
  return uptimeSecond;
}


long LinuxParser::ActiveJiffies(int pid) { 
  long totTime = 0;
  std::string path(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    int pos = 13;
    std::string garbage;
    for (int i = 0; i < pos; i++) {
      if (!(filestream >> garbage)) {
        return 10000;
      }
    }
    long userTimeTicks, kernelTimeTicks = 0, userChildrenTimeTicks = 0, kernelChildrenTimeTicks = 0;
    if (filestream >> userTimeTicks >> kernelTimeTicks >> userChildrenTimeTicks >> kernelChildrenTimeTicks) {
        totTime = (userTimeTicks + kernelTimeTicks + userChildrenTimeTicks + kernelChildrenTimeTicks);
    }
  }  
  return totTime/sysconf(_SC_CLK_TCK);
}


long LinuxParser::ActiveJiffies() { 
  std::vector <long> JifCPU = CpuUtilization();
  return JifCPU[kSoftIRQ_] + JifCPU[kSteal_] + JifCPU[kUser_] + JifCPU[kNice_] + JifCPU[kSystem_] + 
         JifCPU[kIRQ_]; }


long LinuxParser::IdleJiffies() { 
  std::vector <long> JifCPU = CpuUtilization();
  return JifCPU[kIOwait_] + JifCPU[kIdle_]; 
}


vector<long> LinuxParser::CpuUtilization() { 
  std::vector<long> JifCPU;
  std::string path(kProcDirectory + kStatFilename);
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    std::string cpu;
    filestream >> cpu;
    if (cpu == "cpu") {
      for (int i = 0; i < CPUStates::Last; i++) {
        long value;
        filestream >> value;
        JifCPU.push_back(value);
      }
    }
  }
  return JifCPU;
}


int LinuxParser::TotalProcesses() {   
  int value;
  std::string path(kProcDirectory + kStatFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    while (std::getline(filestream, line)){
    	std::istringstream linestream(line);
      std::string key;
      linestream >> key;
      if (key == "processes"){
      	linestream >> value;
      	}
      }
  	}
  	return value;
  }   


int LinuxParser::RunningProcesses() { 
  int value;
  std::string path(kProcDirectory + kStatFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    while (std::getline(filestream, line)){
    	std::istringstream linestream(line);
      std::string key;
      linestream >> key;
      if (key == "procs_running"){
      	linestream >> value;
      	}
      }
  	}
  return value;
}


string LinuxParser::Command(int pid) {
  std::string cmd = "";
  std::string path(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    if (std::getline(filestream, line)){
    	cmd = line;
      	}
    }
  return cmd;
}


string LinuxParser::Ram(int pid) {
  int value;
  std::string path(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    while (std::getline(filestream, line)){
    	std::istringstream linestream(line);
      	std::string key;
      	linestream >> key;
      		if (key == "VmSize:"){
      		linestream >> value;
      		}
      	}
  	}
  
  return std::to_string(value/1000);
}

string LinuxParser::Uid(int pid) { 
  std::string userId = "0000";
  std::string path(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::ifstream filestream(path);
  if(filestream.is_open()){
  	std::string line;
    while (std::getline(filestream, line)){
    	std::istringstream linestream(line);
      	std::string key;
      	linestream >> key;
      		if (key == "Uid:"){
      		linestream >> userId;
      		}
      	}
  	}
  return userId;
}


string LinuxParser::User(int pid) {
  std::string userId = Uid(pid);
  string userName = "";
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    std::string line;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::string currentId;
      std::string xField;
      if (linestream >> userName >> xField >> currentId) {
        if (currentId == userId) {
          return userName;
        }
      }
    }
  }
  return userName;
}


long LinuxParser::UpTime(int pid) { 
long upTimePid = 0;
  std::string path(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    const int position = 22;
    std::string v;
    for (int i = 0; i < position; i++) {
      if (!(filestream >> v)) {
        return 10000;
      }
    }
    upTimePid = std::stol(v);
  }
  return upTimePid/sysconf(_SC_CLK_TCK);
}

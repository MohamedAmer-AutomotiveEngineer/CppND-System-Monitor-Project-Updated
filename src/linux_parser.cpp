#include <dirent.h>
#include <unistd.h>
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
  string os, version, kernel;
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
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
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
  string line;
  string key;
  string value;
  float memTotal, memFree, buffers;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal =  stol(value, nullptr, 10);
        }
        else if (key == "MemFree:") {
          memFree =  stol(value, nullptr, 10);
        }
        else if (key == "Buffers:") {
          buffers =  stol(value, nullptr, 10);
        }
        else
        {
          break;
        }
      }
    }
    return (1.000 - (memFree / (memTotal - buffers)));
  }
  return 0;
}

long LinuxParser::UpTime() {
  string upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return stol(upTime, nullptr, 10);
}

float LinuxParser::CpuUtilization() {
  string line;
  string key;
  string userTime_s,          niceTime_s, systemTime_s, idleTime_s, iowaitTime_s, irqTime_s, softirqTime_s, stealTime_s;
  unsigned long int userTime, niceTime,   systemTime,   idleTime,   iowaitTime,   irqTime,   softirqTime,   stealTime;
  float Idle = 0.0f, NonIdle = 0.0f, Total = 0.0f, totald = 0.0f, idled = 0.0f, CPU_Percentage = 0.0f;
  static float PrevIdle = 0.0f, PrevTotal = 0.0f;
  vector<string> cpu_utlization;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> userTime_s >> niceTime_s >> systemTime_s >> idleTime_s >> iowaitTime_s >> irqTime_s >> softirqTime_s >> stealTime_s;
    if (key == "cpu") {
      userTime       = stol(userTime_s, nullptr, 10);
      niceTime       = stol(niceTime_s, nullptr, 10);
      systemTime     = stol(systemTime_s, nullptr, 10);
      idleTime       = stol(idleTime_s, nullptr, 10);
      iowaitTime     = stol(iowaitTime_s, nullptr, 10);
      irqTime        = stol(irqTime_s, nullptr, 10);
      softirqTime    = stol(softirqTime_s, nullptr, 10);
      stealTime      = stol(stealTime_s, nullptr, 10);

      /* following formulas per htop */
      Idle = idleTime + iowaitTime;                                                    
      NonIdle = userTime + niceTime + systemTime + irqTime + softirqTime + stealTime;
      Total = Idle + NonIdle;
      totald = Total - PrevTotal;
      idled = Idle - PrevIdle;
      CPU_Percentage = (totald - idled)/totald;
      PrevIdle = Idle;
      PrevTotal = Total;
      return CPU_Percentage;
    }
  }
  return 0.0f;
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value, nullptr, 10);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value, nullptr, 10);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string cmdline;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdline);
    return cmdline;
  }
  return " ";
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string(std::stoi(value)/1000);
        }
      }
    }
  }
  return "0";
}

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "0";
}

string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (key == Uid(pid)) {
        return value;
      }
    }
  }
  return "0";
}

long LinuxParser::UpTime(int pid) {
  string upTime, temp;
  string line;
  long int starttime;
  unsigned char index = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (++index < 22) { linestream >> temp; }
    linestream >> upTime;
    starttime = stol(upTime, nullptr, 10);
    return (starttime / sysconf(_SC_CLK_TCK));
  }
  return 0;
}
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() {
  string uTime, sTime, cuTime, csTime, temp;
  string line;
  long int starttime;
  unsigned char index = 0;
  unsigned long int totalTime, seconds;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(this->pid_) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (++index < 14) { linestream >> temp; }
    linestream >> uTime >> sTime >> cuTime >> csTime;
    /* below formulas are from stackoverflow mentioned post in the project by david */
    totalTime = stol(uTime, nullptr, 10) + stol(sTime, nullptr, 10);
    totalTime = totalTime + stol(cuTime, nullptr, 10) + stol(csTime, nullptr, 10);
    seconds = stol(uTime, nullptr, 10) - (stol(sTime, nullptr, 10) / sysconf(_SC_CLK_TCK));
    return (((float)totalTime / (float)sysconf(_SC_CLK_TCK)) / (float)seconds);
  }
  return 0.0f;
}

string Process::Command() { return LinuxParser::Command(this->pid_); }

string Process::Ram() { return LinuxParser::Ram(this->pid_); }

string Process::User() { return LinuxParser::User(this->pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) {
    int pid = a->Pid();
    return (pid < this->pid_);
}

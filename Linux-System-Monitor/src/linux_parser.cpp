#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::getline;
using std::ifstream;
using std::istringstream;
using std::stof;
using std::string;
using std::to_string;
using std::vector;
std::ofstream log_("logLP.txt"); 

string LinuxParser::OperatingSystem()
{
  string value = {};
  string key;
  string line;
  string name;
  ifstream file(kOSPath);

  while (getline(file, line))
  {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    istringstream reader(line);
    reader >> key >> value;
    if (key == "PRETTY_NAME")
    {
      std::replace(value.begin(), value.end(), '_', ' ');
      return value;
    }
  }
  return value;
}

string LinuxParser::Kernel()
{
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization()
{
  ifstream file(kProcDirectory + kMeminfoFilename);
  string line;
  string value;
  string key;
  string kb;

  long MemTotal;
  long MemAvailable;

  while (getline(file, line))
  {
    istringstream readLine(line);
    readLine >> key >> value >> kb;
    if (key == "MemTotal:")
      MemTotal = std::stol(value);

    if (key == "MemAvailable:")
    {
      MemAvailable = std::stol(value);
      break;
    }
  }
  return (MemTotal - MemAvailable) / (float)MemTotal;
}

long LinuxParser::UpTime()
{
  ifstream file(kProcDirectory + kUptimeFilename);
  string l;
  file >> l;
  long time = 0;

  try
  {
    time = std::stol(l);
  }
  catch (std::invalid_argument &arg)
  {
    return 0;
  }

  return time;
}

vector<string> LinuxParser::CpuUtilization()
{
  vector<string> Utilisation = {};
  string line;
  string cpu, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
  ifstream file(kProcDirectory + kStatFilename);
  getline(file, line);

  istringstream readLine(line);
  readLine >> cpu >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9 >> p10;
  Utilisation = {p1, p2, p3, p4, p5, p6, p7, p8, p9, p10};
  return Utilisation;
}

float LinuxParser::CpuUtilization(int pid)
{
  ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  int i = 1;
  string val;
  long time = 0;

  while (i <= 17)
  {
    file >> val;
    if (i == 14 || i == 15 || i == 16 || i == 17)
      try
      {
        log_<<val<<" ";
        time += std::stol(val);
      }
      catch (std::invalid_argument &arg)
      {
        time += 0;
      }

    i++;
    
  }log_<<pid<<"\n";


  time /= sysconf(_SC_CLK_TCK);

  long up = LinuxParser::UpTime(pid);
  return (float)time / up;
}

int LinuxParser::TotalProcesses()
{
  ifstream file(kProcDirectory + kStatFilename);
  string word;
  while (file >> word)
  {
    if (word == "processes")
    {
      file >> word;
      break;
    }
  }
  return std::stoi(word);
}

int LinuxParser::RunningProcesses()
{
  ifstream file(kProcDirectory + kStatFilename);
  string word;
  while (file >> word)
  {
    if (word == "procs_running")
    {
      file >> word;
      break;
    }
  }
  return std::stoi(word);
}

string LinuxParser::Command(int pid)
{
  ifstream file(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string val;
  file >> val;
  return val;
}

std::string LinuxParser::Ram(int pid)
{
  ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  string val = "0";

  while (file >> val)
  {
    if (val == "RssAnon:")
      break;
  }
  file >> val;

  long long ram = 0;
  try
  {
    ram = std::stoll(val);
    ram /= (long long)1024;
    val = to_string(ram);
  }
  catch (std::invalid_argument &arg)
  {
    return "0";
  }
  return val;
}

string LinuxParser::Uid(int pid)
{
  ifstream file(kProcDirectory + to_string(pid) + kStatusFilename);
  string val;
  while (file >> val)
  {
    if (val == "Uid:")
    {
      file >> val;
      return val;
    }
  }

  return "";
}

string LinuxParser::User(int pid)
{

  string line;
  string user = "Unknown";
  string pwd;
  string code;
  string uid = LinuxParser::Uid(pid);
  ifstream file(kPasswordPath);
  while (getline(file, line))
  {
    std::replace(line.begin(), line.end(), ':', ' ');
    istringstream read(line);
    read >> user >> pwd >> code;
    if (code == uid)
      return user;
  }

  return user;
}

long LinuxParser::UpTime(int pid)
{
  ifstream file(kProcDirectory + to_string(pid) + kStatFilename);
  string val;
  long time = 0;
  int i = 1;
  while (i <= 22)
  {
    file >> val;
    i++;
  }

  try
  {
    time = std::stol(val);
  }
  catch (std::invalid_argument &arg)
  {
    return time / sysconf(_SC_CLK_TCK);
  }

  time /= sysconf(_SC_CLK_TCK);
  time = LinuxParser::UpTime() - time;
  return time;
}

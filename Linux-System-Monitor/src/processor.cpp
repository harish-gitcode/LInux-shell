#include "processor.h"
#include "linux_parser.h"
#include<vector>
#include<string>
float Processor::Utilization(){
    std::vector<std::string> values=LinuxParser::CpuUtilization();
    std::vector<long> cpuValues;

    for(auto x: values)
    cpuValues.push_back(std::stol(x));

    long newtime=0;
    for(auto x: cpuValues)
    newtime+=x;

    long newidle=cpuValues[LinuxParser::kIdle_]+cpuValues[LinuxParser::kIOwait_];

    long tempNewtime=newtime-time;
    long tempidle=newidle-idle;

    idle=newidle;
    time=newtime;

    return (float)(tempNewtime-tempidle)/tempNewtime;



}
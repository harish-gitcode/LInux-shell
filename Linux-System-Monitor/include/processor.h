#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor
{
public:
  float Utilization();

private:
  long idle{0};
  long time{0};
};

#endif
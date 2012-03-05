#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include "../../environment/utils.h"
namespace wmdf
{
  class SimulatorUtils
  {
  public:
    SimulatorUtils(void);
    virtual ~SimulatorUtils(void);
    static bool isDoubleEqual(double d1,double d2);
    static int GetRandomValue(int min,int max); 
  };
  
  bool SimulatorUtils::isDoubleEqual(double d1,double d2)
  {
    return (d1-d2)<0.01&&(d1-d2)>-0.01;
  }

  int wmdf::SimulatorUtils::GetRandomValue(int min,int max)
  {
    srand((unsigned)time(0));
    clock_t seed = clock();
    int value = rand();
    for(int i=0;i<seed%10;i++)
    {
      value = rand();
    }
    Utils::SleepTime(1);
    return value%(max-min)+ min;
  }
}

#endif

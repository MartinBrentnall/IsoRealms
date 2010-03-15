#ifndef TIME_LIMIT_H
#define TIME_LIMIT_H

#include "../IIntegerValue.h"

class TimeLimit:public IIntegerValue {
  private:
  int cMilliseconds;

  public:

  /****************************\
   * Implements IIntegerValue *
  \****************************/
  IIntegerValue& operator+=(const int&);
  void addIntegerValueListener(IIntegerValueListener*);
  void removeIntegerValueListener(IIntegerValueListener*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
};

#endif

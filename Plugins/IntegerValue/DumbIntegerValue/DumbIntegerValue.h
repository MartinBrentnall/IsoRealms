#ifndef DUMB_INTEGER_VALUE_H
#define DUMB_INTEGER_VALUE_H

#include "../IIntegerValue.h"

class DumbIntegerValue:public IIntegerValue {
  private:
  std::vector<IIntegerValueListener*> cListeners;
  int cValue;

  public:
  DumbIntegerValue();

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

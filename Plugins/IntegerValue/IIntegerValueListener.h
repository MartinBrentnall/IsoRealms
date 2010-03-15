#ifndef I_INTEGER_VALUE_LISTENER_H
#define I_INTEGER_VALUE_LISTENER_H

class IIntegerValueListener {
  public:
  virtual void valueChanged(int value) = 0;
};

#endif

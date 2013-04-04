#ifndef I_STRING_LISTENER_H
#define I_STRING_LISTENER_H

class IStringListener {
  public:
  virtual void valueChanged(const std::string&) = 0;
};

#endif

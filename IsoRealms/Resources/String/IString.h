#ifndef I_STRING_H
#define I_STRING_H

#include <string>

#include <IsoRealms/Resources/IResource.h>

class IString:public IResource {
  public:
  virtual std::string getValue() = 0;
  virtual void setValue(const std::string&) = 0;
};

#endif

#ifndef TIME_STRING_H
#define TIME_STRING_H

#include <iomanip>
#include <sstream>

#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/String/IString.h>
#include <IsoRealms/SingleResourceTypeModule.h>

class TimeString:public IString {
  private:
  IInteger* cValue;
  
  public:
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /**********************\
   * Implements IString *
  \**********************/
  std::string getValue();
  void setValue(const std::string&);
  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif

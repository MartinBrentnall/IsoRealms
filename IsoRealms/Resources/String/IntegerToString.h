#ifndef INTEGER_TO_STRING_H
#define INTEGER_TO_STRING_H

#include <string>

#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Utils.h>

#include "IString.h"

class IntegerToString:public IString {
  private:
  IInteger* cValue;

  public:
  IntegerToString(IInteger*);
  IInteger* getInteger();

  /**********************\
   * Implements IString *
  \**********************/
  std::string getValue();
  void setValue(const std::string&);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
};

#endif

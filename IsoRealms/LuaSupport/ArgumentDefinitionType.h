#ifndef ARGUMENT_DEFINITION_TYPE_H
#define ARGUMENT_DEFINITION_TYPE_H

#include "../Persistence/DOMNodeWrapper.h"
#include "ArgumentDefinition.h"
#include "IArgumentDefinitionType.h"

template <class T> class ArgumentDefinitionType:public IArgumentDefinitionType {
  private:
  std::string cType;
  
  public:
  void setType(std::string type) {
    if (type == "") {
      std::cout << "Custom type cannot be empty" << std::endl;
      exit(1);
    }
    if (cType != "") {
      std::cout << "Custom type has already been set" << std::endl;
      exit(1);
    }
    cType = type;
  }
    
  IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper* node, IResourceAccessor* resources) {
    return new ArgumentDefinition<T>(node, resources, &cType);
  }
};

#endif
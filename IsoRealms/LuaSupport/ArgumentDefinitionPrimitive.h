#ifndef ARGUMENT_DEFINITION_PRIMITIVE_H
#define ARGUMENT_DEFINITION_PRIMITIVE_H

#include <string>

#include "IArgumentDefinition.h"
#include "IArgumentSource.h"
#include "LuaArgument.h"

template <class T> class ArgumentDefinitionPrimitive:public IArgumentDefinition {
  private:
  const std::string* cType;
  std::string cName;
  IArgumentSource* cDefaultValue;
  
  public:
  ArgumentDefinitionPrimitive(DOMNodeWrapper* node, IResourceAccessor* resources, const std::string* type) {
    cType = type;
    cName = node->getAttribute("name");
    cDefaultValue = resources->getArgument(node);
  }
    
  /******************************\
   * Implements IArgumentSource *
  \******************************/
  std::string getName() {
    return cName;
  }
  
  ILuaFunctionArgument* createDefaultArgumentValue() {
    if (cDefaultValue == NULL) {
      std::cout << "No value or default value specified for argument \"" << cName << "\"" << std::endl;
      exit(1);
    }
    return cDefaultValue->createArgument(true);
  }
  
  std::string getCode(const std::string& function, unsigned int arg) {
    std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
    mFunction += "  _" + cName + " = arg\n";
    mFunction += "  " + cName + " = arg:getValue()\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }
  
  std::string getCleanup() {
    std::string mFunction = "  _" + cName + ":setValue(" + cName + ")\n";
    mFunction += "  _" + cName + " = nil\n";
    mFunction += "  " + cName + " = nil\n";
    return mFunction;
  }
  
  void save(DOMNodeWriter* node) {
    node->addAttribute("type", *cType);
    node->addAttribute("name", cName);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    save(node);
    if (cDefaultValue != NULL) {
      cDefaultValue->save(node, resourceLocator);
    }
  }  
};

#endif

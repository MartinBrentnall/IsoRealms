#ifndef ARGUMENT_DEFINITION_H
#define ARGUMENT_DEFINITION_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

#include "IArgumentDefinition.h"
#include "IArgumentSource.h"
#include "LuaArgument.h"

template <class T> class ArgumentDefinition:public IArgumentDefinition {
  private:
  const std::string* cType;
  std::string cName;
  IArgumentSource* cDefaultValue;

  public:
  ArgumentDefinition(DOMNodeWrapper* node, IResourceAccessor* resources, const std::string* type) {
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
    mFunction += "  " + cName + " = arg\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }

  std::string getCleanup() {
    return "  " + cName + " = nil\n";
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

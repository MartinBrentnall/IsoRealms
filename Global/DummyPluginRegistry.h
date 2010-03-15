#ifndef DUMMY_PLUGIN_REGISTRY_H
#define DUMMY_PLUGIN_REGISTRY_H

#include <dlfcn.h>
#include <map>
#include <string>

#include "DOMNodeWrapper.h"
#include "Hacks.h"
#include "InitException.h"
#include "System.h"

class IPlugin;

class DummyPluginRegistry {
  private:
  static std::map<std::string, IPlugin*> cDummyPlugins;  

  public:
  static IPlugin* getDummyPlugin(const std::string&);
  static bool isDummyPlugin(IPlugin*);  
};

typedef IPlugin* createPlugin(DOMNodeWrapper*);
typedef void destroyPlugin(IPlugin*);

#endif

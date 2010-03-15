#include "DummyPluginRegistry.h"

std::map<std::string, IPlugin*> DummyPluginRegistry::cDummyPlugins;  

bool DummyPluginRegistry::isDummyPlugin(IPlugin* instance) {
  for (std::map<std::string, IPlugin*>::iterator i = cDummyPlugins.begin(); i != cDummyPlugins.end(); i++) {
    if (instance == i->second) {
      return true;
    }
  }
  return false;
}

IPlugin* DummyPluginRegistry::getDummyPlugin(const std::string& type) {
  IPlugin* mDummyPlugin = cDummyPlugins[type];
  if (mDummyPlugin == NULL) {
    std::string mPluginLocation = System::getConfigurationResource("Plugins/" + type + "/dummy");
    void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!mPluginSO) {
      // TODO: This causes segmentation fault
      throw InitException("Cannot load library: " + std::string(dlerror()));
    }
    createPlugin* createPluginFunction = cast_voidptr_to_funcptr<createPlugin*>(dlsym(mPluginSO, "create"));
    const char* mDlsymError = dlerror();
    if (mDlsymError) {
      throw InitException("Cannot load symbol: " + std::string(mDlsymError));
    }
    mDummyPlugin = createPluginFunction(NULL);
    cDummyPlugins[type] = mDummyPlugin;
  }
  return mDummyPlugin;
}


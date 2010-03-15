#ifndef I_PLUGIN_REGISTRY_ACCESSOR_H
#define I_PLUGIN_REGISTRY_ACCESSOR_H

class IPluginRegistryAccessor {
  public:
  virtual PluginRegistry* getPluginRegistry() = 0;
};

#endif

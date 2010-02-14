#ifndef I_PLUGIN_REGISTRY_H
#define I_PLUGIN_REGISTRY_H

#include <string>

#include "IPlugin.h"

class IPluginRegistry {
  public:

  /**
   * Retrieve a logic factory instance of the specified type and name.
   * 
   * @param string&  The logic type.
   * @param string&  The logic instance name.
   * @returns  The instance of the logic.
   */
  virtual IPlugin* getPlugin(std::string&, std::string&) = 0;

  /**
   * Retrieve the plug-in instance name
   */
  virtual std::string getInstanceName(std::string, IPlugin*) = 0;
};

#endif

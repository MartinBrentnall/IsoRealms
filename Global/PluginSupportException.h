#ifndef PLUGIN_SUPPORT_EXCEPTION_H
#define PLUGIN_SUPPORT_EXCEPTION_H

#include <string>

class PluginSupportException {
  private:
  std::string cMessage;

  public:
  PluginSupportException(const std::string&);
  std::string& getMessage();
};

#endif

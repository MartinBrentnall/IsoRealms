#include "DefaultZoneContext.h"

std::string DefaultZoneContext::getName() {
  return "Default Zone Context";
}

void DefaultZoneContext::setZoneContext(IZone* context) {
  cContext = context;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->zoneContextChanged(cContext);
  }
}

IZone* DefaultZoneContext::getZoneContext() {
  return cContext;
}

void DefaultZoneContext::addZoneContextListener(IZoneContextListener* listener) {
  cListeners.push_back(listener);
}

void DefaultZoneContext::removeZoneContextListener(IZoneContextListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
    }
  }
}

extern "C" IPlugin* create() {
  return new DefaultZoneContext();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

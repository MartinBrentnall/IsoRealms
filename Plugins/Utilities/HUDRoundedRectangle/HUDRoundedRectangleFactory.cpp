#include "HUDRoundedRectangleFactory.h"

HUDRoundedRectangleFactory::HUDRoundedRectangleFactory() {
  assignDummyPlugin(&cHUD, "HUD");
}

void HUDRoundedRectangleFactory::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "HUD") {
    IHUD* mPreviousHUD = cHUD;
    if (assignPlugin(plugin, &cHUD, *socket)) {
      mPreviousHUD->unregisterHUDComponentFactory(this);
      cHUD->registerHUDComponentFactory(this);
    }
  } else {
    // TODO: Throw
  }
}

IPlugin* HUDRoundedRectangleFactory::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "HUD") {return cHUD;}
  // TODO: Throw
  return NULL;
}

IHUDGameComponent* HUDRoundedRectangleFactory::getHUDComponent(const std::string& name) {
  unsigned int mIndex = atoi(name.c_str());
  if (mIndex == cRoundedRectangles.size()) {
    HUDRoundedRectangle* mNewRoundedRectangle = new HUDRoundedRectangle();
    cRoundedRectangles.push_back(mNewRoundedRectangle);
    return mNewRoundedRectangle;
  } else if (mIndex < cRoundedRectangles.size()) {
    return cRoundedRectangles[mIndex];
  }
  // TODO: Throw
  return NULL;
}

extern "C" IPlugin* create() {
  return new HUDRoundedRectangleFactory();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

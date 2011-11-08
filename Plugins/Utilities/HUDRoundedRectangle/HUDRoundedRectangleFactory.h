#ifndef HUD_ROUNDED_RECTANGLE_FACTORY_H
#define HUD_ROUNDED_RECTANGLE_FACTORY_H

#include "../IUtilities.h"

#include "../../HUD/IHUD.h"
#include "../../HUD/IHUDComponentFactory.h"

#include "HUDRoundedRectangle.h"

class HUDRoundedRectangleFactory:public IUtilities,
                                 public IHUDComponentFactory {
  private:
  std::vector<HUDRoundedRectangle*> cRoundedRectangles;
  IHUD* cHUD;

  public:
  HUDRoundedRectangleFactory();

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent(const std::string&);
};

#endif

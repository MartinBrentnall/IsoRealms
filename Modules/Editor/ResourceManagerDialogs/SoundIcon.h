#ifndef SOUND_ICON_H
#define SOUND_ICON_H

#include <IsoRealms/GUI/LookAndFeel.h>

#include "Icon.h"

class SoundIcon:public Icon<ISound> {
  private:
  IResourceLocator* cResources;
  
  public:
  SoundIcon(IResourceBrowser<ISound>*, ISound*);
    
  void renderIcon();  
  void iconSelected();
  float getWidth();
  float getHeight();
};

#endif

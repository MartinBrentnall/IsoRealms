#ifndef FONT_ICON_H
#define FONT_ICON_H

#include <IsoRealms/GUI/LookAndFeel.h>

#include "Icon.h"

class FontIcon:public Icon<IFont> {
  public:
  FontIcon(IResourceBrowser<IFont>*, IFont*);
    
  void renderIcon();  
  float getWidth();
  float getHeight();
};

#endif

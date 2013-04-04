#ifndef COLOUR_ICON_H
#define COLOUR_ICON_H

#include "Icon.h"

class ColourIcon:public Icon<IColour> {
  public:
  ColourIcon(IResourceBrowser<IColour>*, IColour*);
    
  void renderIcon();
  float getWidth();
  float getHeight();
};

#endif

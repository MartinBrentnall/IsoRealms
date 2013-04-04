#ifndef TEXTURE_ICON_H
#define TEXTURE_ICON_H

#include "Icon.h"

class TextureIcon:public Icon<ITexture> {
  public:
  TextureIcon(IResourceBrowser<ITexture>*, ITexture*);
    
  void renderIcon();  
  float getWidth();
  float getHeight();
};

#endif

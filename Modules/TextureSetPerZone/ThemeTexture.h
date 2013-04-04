#ifndef THEME_TEXTURE_H
#define THEME_TEXTURE_H

#include <IsoRealms/Resources/Texture/ITexture.h>

class ThemeTexture:public ITexture {
  private:
  ITexture* cTexture;

  public:
  void set(ITexture*);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
};

#endif

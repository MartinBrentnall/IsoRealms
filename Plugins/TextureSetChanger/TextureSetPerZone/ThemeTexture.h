#ifndef THEME_TEXTURE_H
#define THEME_TEXTURE_H

#include <IsoRealms/ITexture.h>

class ThemeTexture:public ITexture {
  private:
  ITexture* cTexture;

  public:
  void set(ITexture*);

  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void texCoord2f(float, float);
  IColour* getColour(float, float);
  void save(DOMNodeWriter*, const std::string&);
};

#endif

#ifndef THEME_H
#define THEME_H

#include <map>

#include <IsoRealms/ITexture.h>

#include "ThemeTexture.h"

class Theme {
  private:
  std::map<ThemeTexture*, ITexture*> cTextures;

  public:
  void registerElement(ThemeTexture*, ITexture*);
  void set();
};

#endif

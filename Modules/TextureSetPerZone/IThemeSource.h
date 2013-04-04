#ifndef I_THEME_SOURCE_H
#define I_THEME_SOURCE_H

#include <string>

#include "ThemeTexture.h"

class IThemeSource {
  public:
  virtual ThemeTexture* getThemeTexture(const std::string&) = 0;
  virtual std::string getThemeElement(ThemeTexture*) = 0;
};

#endif

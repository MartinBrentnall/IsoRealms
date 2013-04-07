#ifndef I_THEME_SOURCE_H
#define I_THEME_SOURCE_H

#include <string>

#include "ThemeColour.h"
#include "ThemeTexture.h"

class IThemeSource {
  public:
  virtual ThemeTexture* getThemeTexture(const std::string&) = 0;
  virtual ThemeColour* getThemeColour(const std::string&) = 0;
  virtual std::string getThemeElement(ThemeTexture*) = 0;
  virtual std::string getThemeElement(ThemeColour*) = 0;
};

#endif

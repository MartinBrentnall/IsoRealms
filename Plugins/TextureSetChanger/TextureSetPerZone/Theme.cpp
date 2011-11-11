#include "Theme.h"

void Theme::registerElement(ThemeTexture* themeTexture, ITexture* texture) {
  cTextures[themeTexture] = texture;
}

void Theme::set() {
  for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    i->first->set(i->second);
  }
}

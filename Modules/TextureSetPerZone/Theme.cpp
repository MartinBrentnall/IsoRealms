#include "Theme.h"

Theme::Theme(IThemeSource* source) {
  cThemeSource = source;
}

void Theme::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cResources = resources;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mType = mNode->getAttribute("type");
      std::string mTexturePath = mNode->getAttribute("name");
      ITexture* mTexture = resources->getTexture(mTexturePath);
      ThemeTexture* mThemeTexture = cThemeSource->getThemeTexture(mType);
      registerElement(mThemeTexture, mTexture);
    } else if (mValueAsString == "Colour") {
      std::string mType = mNode->getAttribute("type");
      std::string mColourPath = mNode->getAttribute("name");
      IColour* mColour = resources->getColour(mColourPath);
      ThemeColour* mThemeColour = cThemeSource->getThemeColour(mType);
      registerElement(mThemeColour, mColour);
    }
  }
}

void Theme::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    std::string mThemeElement = cThemeSource->getThemeElement(i->first);
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("type", mThemeElement);
    mTextureNode->addAttribute("name", resourceLocator->getPath(i->second));
  }
  for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    std::string mThemeElement = cThemeSource->getThemeElement(i->first);
    DOMNodeWriter* mColourNode = node->addBranch("Colour");
    mColourNode->addAttribute("type", mThemeElement);
    mColourNode->addAttribute("name", resourceLocator->getPath(i->second));
  }
}

void Theme::registerElement(ThemeTexture* themeTexture, ITexture* texture) {
  cTextures[themeTexture] = texture;
}

void Theme::registerElement(ThemeColour* themeColour, IColour* colour) {
  cColours[themeColour] = colour;
}

void Theme::set() {
  for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    i->first->set(i->second);
  }
//   for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
//     i->first->set(i->second);
//   }
}

void Theme::removeTexture(ThemeTexture* texture) {
  cTextures.erase(texture);
}

void Theme::removeColour(ThemeColour* colour) {
  cColours.erase(colour);
}

IColour* Theme::getColour(ThemeColour* colour) {
  return cColours[colour];
}

void Theme::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (destroyee == i->second) {
      i->second = replacement;
      cResources->notifyChange(i->first);
    }
  }  
}

void Theme::resourceChanged(ITexture* texture) {
  for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (texture == i->second) {
      cResources->notifyChange(i->first);
    }
  }  
}

void Theme::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (destroyee == i->second) {
      i->second = replacement;
      cResources->notifyChange(i->first);
    }
  }  
}

void Theme::resourceChanged(IColour* colour) {
  for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (colour == i->second) {
      cResources->notifyChange(i->first);
    }
  }  
}


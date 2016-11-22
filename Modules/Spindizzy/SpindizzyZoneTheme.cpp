/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SpindizzyZoneTheme.h"

SpindizzyZoneTheme::SpindizzyZoneTheme(IThemeSource* source) {
  cThemeSource = source;
}

void SpindizzyZoneTheme::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cResources = resources;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mType = mNode->getAttribute("type");
      std::string mTexturePath = mNode->getAttribute("name");
      ITexture* mTexture = resources->getTexture(mTexturePath);
      SpindizzyZoneThemeTexture* mThemeTexture = cThemeSource->getThemeTexture(mType);
      registerElement(mThemeTexture, mTexture);
    } else if (mValueAsString == "Colour") {
      std::string mType = mNode->getAttribute("type");
      std::string mColourPath = mNode->getAttribute("name");
      IColour* mColour = resources->getColour(mColourPath);
      SpindizzyZoneThemeColour* mThemeColour = cThemeSource->getThemeColour(mType);
      registerElement(mThemeColour, mColour);
    }
  }
  set();
}

void SpindizzyZoneTheme::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (std::map<SpindizzyZoneThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    std::string mThemeElement = cThemeSource->getThemeElement(i->first);
    DOMNodeWriter* mTextureNode = node->addBranch("Texture");
    mTextureNode->addAttribute("type", mThemeElement);
    mTextureNode->addAttribute("name", resourceLocator->getPath(i->second));
  }
  for (std::map<SpindizzyZoneThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    std::string mThemeElement = cThemeSource->getThemeElement(i->first);
    DOMNodeWriter* mColourNode = node->addBranch("Colour");
    mColourNode->addAttribute("type", mThemeElement);
    mColourNode->addAttribute("name", resourceLocator->getPath(i->second));
  }
}

void SpindizzyZoneTheme::registerElement(SpindizzyZoneThemeTexture* themeTexture, ITexture* texture) {
  cTextures[themeTexture] = texture;
}

void SpindizzyZoneTheme::registerElement(SpindizzyZoneThemeColour* themeColour, IColour* colour) {
  cColours[themeColour] = colour;
}

void SpindizzyZoneTheme::set() {
  for (std::map<SpindizzyZoneThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    i->first->set(i->second);
  }
//   for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
//     i->first->set(i->second);
//   }
}

std::string SpindizzyZoneTheme::getName() {
  return cThemeSource->getThemeName(this);
}

std::map<std::string, ITexture*> SpindizzyZoneTheme::getTextureElements() {
  std::map<std::string, ITexture*> mElements;
  for (std::pair<SpindizzyZoneThemeTexture*, ITexture*> mTexture : cTextures) {
    std::string mName = cThemeSource->getThemeElement(mTexture.first);
    mElements[mName] = mTexture.second;
  }
  return mElements;
}

std::map<std::string, IColour*> SpindizzyZoneTheme::getColourElements() {
  std::map<std::string, IColour*> mElements;
  for (std::pair<SpindizzyZoneThemeColour*, IColour*> mColour : cColours) {
    std::string mName = cThemeSource->getThemeElement(mColour.first);
    mElements[mName] = mColour.second;
  }
  return mElements;
}

void SpindizzyZoneTheme::removeTexture(SpindizzyZoneThemeTexture* texture) {
  cTextures.erase(texture);
}

void SpindizzyZoneTheme::removeColour(SpindizzyZoneThemeColour* colour) {
  cColours.erase(colour);
}

IColour* SpindizzyZoneTheme::getColour(SpindizzyZoneThemeColour* colour) {
  return cColours[colour];
}

SpindizzyZoneThemeIcon* SpindizzyZoneTheme::createCustomResourceIcon(IResourceBrowser<ISpindizzyZoneTheme>* resourceBrowser) {
  I3DModel* mIconModel = cThemeSource->createThemeIcon();
  return new SpindizzyZoneThemeIcon(resourceBrowser, this, mIconModel);
}

void SpindizzyZoneTheme::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  for (std::map<SpindizzyZoneThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (destroyee == i->second) {
      i->second = replacement;
      cResources->notifyChange(i->first);
    }
  }  
}

void SpindizzyZoneTheme::resourceChanged(ITexture* texture) {
  for (std::map<SpindizzyZoneThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (texture == i->second) {
      cResources->notifyChange(i->first);
    }
  }  
}

void SpindizzyZoneTheme::resourcePendingDestruction(IColour* destroyee, IColour* replacement) {
  for (std::map<SpindizzyZoneThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (destroyee == i->second) {
      i->second = replacement;
      cResources->notifyChange(i->first);
    }
  }  
}

void SpindizzyZoneTheme::resourceChanged(IColour* colour) {
  for (std::map<SpindizzyZoneThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (colour == i->second) {
      cResources->notifyChange(i->first);
    }
  }  
}

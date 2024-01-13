/*
 * Copyright 2023 Martin Brentnall
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
#include "Theme.h"

#include "ThemeSet.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  Theme::Theme(IProject* project, Spindizzy* spindizzy, ThemeSet* themeSet, DOMNode& node) {
    cSpindizzy = spindizzy;
    cThemeSet  = themeSet;
  //       cThemes[mThemeName] = mTheme;
  //       createThemeResources(mNode, resources);
    
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == "Texture") {
        std::string mTextureName = mNode.getAttribute("type");
        cThemeSet->createTexture(mTextureName);
      } else if (mChildName == "Colour") {
        std::string mColourName = mNode.getAttribute("type");
        cThemeSet->createColour(project, mColourName);
      } else {
        throw ParseException("Unknown tag for Spindizzy/Theme: " + mChildName);
      }
    }
    
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == "Colour") {
        ThemeColour* mThemeColour = cThemeSet->getColour(mNode.getAttribute("type"));
        cColours.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeColour), std::forward_as_tuple(project, 1.0f, 0.0f, 1.0f));
        cColours.find(mThemeColour)->second.init(mNode.getNode("Colour"));
      } else if (mChildName == "Texture") {
        ThemeTexture* mThemeTexture = cThemeSet->getTexture(mNode.getAttribute("type"));
        cTextures.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeTexture), std::forward_as_tuple(project));
        cTextures.find(mThemeTexture)->second.init(mNode.getNode("Texture"));
      } else {
        throw ParseException("Unknown tag for Spindizzy/Theme: " + mChildName);
      }
    }

    project->init([this, &node](IAssets* assets) {
      set();
    });
  }

  void Theme::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::pair<ThemeTexture* const, Texture>& mTexture : cTextures) {
      std::string mThemeElement = cThemeSet->getElement(mTexture.first);
      DOMNodeWriter mTextureNode = node->addBranch("Texture");
      mTextureNode.addAttribute("type", mThemeElement);
      mTexture.second.save(&mTextureNode, "Name");
    }
    for (const std::pair<ThemeColour* const, Colour>& mColour : cColours) {
      std::string mThemeElement = cThemeSet->getElement(mColour.first);
      DOMNodeWriter mColourNode = node->addBranch("Colour");
      mColourNode.addAttribute("type", mThemeElement);
      mColour.second.save(&mColourNode, "Texture");
    }
  }

  void Theme::set() {
    for (std::map<ThemeTexture*, Texture>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      i->first->set(*i->second);
    }
    for (std::map<ThemeColour*, Colour>::iterator i = cColours.begin(); i != cColours.end(); i++) {
      i->first->set(&i->second);
    }
  }

  std::string Theme::getName() {
    return cThemeSet->getName(this);
  }

  std::string Theme::getElementName(ThemeTexture* texture) {
    return cThemeSet->getElement(texture);
  }

  std::string Theme::getElementName(ThemeColour* colour) {
    return cThemeSet->getElement(colour);
  }

  void Theme::hintInUse(bool inUse) {
    for (std::map<ThemeTexture*, Texture>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      i->second->hintTextureInUse(inUse);
    }
  }

  void Theme::releaseAssets(IAssets* releaser) {
    // Nothing to do.
  }

  ITexture* Theme::getTexture(ThemeTexture* themeTexture) {
    return *cTextures.find(themeTexture)->second;
  }

  void Theme::removeTexture(ThemeTexture* texture) {
    cTextures.erase(texture);
  }

  void Theme::removeColour(ThemeColour* colour) {
    cColours.erase(colour);
  }

  IColour* Theme::getColour(ThemeColour* colour) {
    return &cColours.find(colour)->second;
  }

  // TODO: Enable this
//   void Theme::assetChanged(ITexture* texture) {
//     for (std::map<ThemeTexture*, ITexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
//       if (texture == i->second) {
//         cResources->notifyChange(i->first);
//       }
//     }  
//   }

  void Theme::stateChanged(ITexture* asset) {
    std::cout << "TEXTURE CHANGED IN THEME!" << std::endl;
    // TODO: Check asset is actually used
    // TODO: Users of affected texture(s) need to be notified!
  }
  
  // TODO: Enable this
//   void Theme::assetChanged(IColour* colour) {
//     for (std::map<ThemeColour*, IColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
//       if (colour == i->second) {
//         cResources->notifyChange(i->first);
//       }
//     }  
//   }
}

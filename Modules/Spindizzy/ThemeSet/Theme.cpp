/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Theme.h"

#include "Modules/Spindizzy/Spindizzy.h"

#include "ThemeSet.h"

namespace IsoRealms::Spindizzy {
  const std::string Theme::JSON_COLOUR   = "colour";
  const std::string Theme::JSON_COLOURS  = "colours";
  const std::string Theme::JSON_ELEMENT  = "element";
  const std::string Theme::JSON_TEXTURE  = "texture";
  const std::string Theme::JSON_TEXTURES = "textures";

  Theme::Theme(IProject& project, ThemeSet& themeSet) :
            cThemeSet(themeSet) {
    std::vector<ThemeTexture*> mThemeTextures = themeSet.getThemeTextures();
    for (ThemeTexture* mThemeTexture : mThemeTextures) {
      cTextures.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeTexture), std::forward_as_tuple(themeSet.getResourceData()));
    }

    std::vector<ThemeColour*> mThemeColours = themeSet.getThemeColours();
    for (ThemeColour* mThemeColour : mThemeColours) {
      cColours.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeColour), std::forward_as_tuple(themeSet.getResourceData()));
    }
  }
  
  Theme::Theme(IProject& project, ThemeSet& themeSet, JSONObject object) :
            cThemeSet(themeSet) {
    for (JSONValue mTextureValue : object.getArray(JSON_TEXTURES)) {
      JSONObject mTextureObject = mTextureValue.getObject();
      ThemeTexture* mThemeTexture = cThemeSet.createTexture(mTextureObject.getString(JSON_ELEMENT));
      cTextures.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeTexture), std::forward_as_tuple(themeSet.getResourceData())).first->second.init(mTextureObject, JSON_TEXTURE);
    }

    for (JSONValue mColourValue : object.getArray(JSON_COLOURS)) {
      JSONObject mColourObject = mColourValue.getObject();
      ThemeColour* mThemeColour = cThemeSet.createColour(project, mColourObject.getString(JSON_ELEMENT));
      cColours.emplace(std::piecewise_construct, std::forward_as_tuple(mThemeColour), std::forward_as_tuple(themeSet.getResourceData(), 1.0f, 0.0f, 1.0f)).first->second.init(mColourObject, JSON_COLOUR);
    }

    project.init([this](IAssets& assets) {
      set();
    });
  }

  void Theme::save(JSONObject object) const {
    JSONArray mTexturesArray = object.addArray(JSON_TEXTURES);
    for (const std::pair<ThemeTexture* const, Texture>& mTexture : cTextures) {
      JSONObject mTextureObject = mTexturesArray.addObject();
      mTextureObject.addString(JSON_ELEMENT, cThemeSet.getElement(mTexture.first));
      mTexture.second.save(mTextureObject, JSON_TEXTURE);
    }
    JSONArray mColoursArray = object.addArray(JSON_COLOURS);
    for (const std::pair<ThemeColour* const, Colour>& mColour : cColours) {
      JSONObject mColourObject = mColoursArray.addObject();
      mColourObject.addString(JSON_ELEMENT, cThemeSet.getElement(mColour.first));
      mColour.second.save(mColourObject, JSON_COLOUR);
    }
  }

  void Theme::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeString(metadata.getPropertyData("ThemeName"), [this]() {return getName();}, [this](const std::string& value) {cThemeSet.setName(*this, value);}, [this](const std::string& value) {return cThemeSet.isNameAllowed(*this, value);});
    for (std::pair<ThemeTexture* const, Texture>& mTexture : cTextures) {
      owner.createPropertyAsset<Texture>(PropertyData(cThemeSet.getElement(mTexture.first), "TODO: Theme Texture Description"), mTexture.second);
    }
    for (std::pair<ThemeColour* const, Colour>& mColour : cColours) {
      owner.createPropertyAsset<Colour>(PropertyData(cThemeSet.getElement(mColour.first), "TODO: Theme Colour Description"), mColour.second);
    }
  }

  void Theme::themeTextureAdded(ThemeTexture* texture) {
    cTextures.emplace(std::piecewise_construct, std::forward_as_tuple(texture), std::forward_as_tuple(cThemeSet.getResourceData()));
  }

  void Theme::themeColourAdded(ThemeColour* colour) {
    cColours.emplace(std::piecewise_construct, std::forward_as_tuple(colour), std::forward_as_tuple(cThemeSet.getResourceData()));
  }

  void Theme::set() {
    for (std::map<ThemeTexture*, Texture>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      i->first->set(*i->second);
    }
    for (std::map<ThemeColour*, Colour>::iterator i = cColours.begin(); i != cColours.end(); i++) {
      i->first->set(*(i->second));
    }
  }

  std::string Theme::getName() {
    return cThemeSet.getName(this);
  }

  std::string Theme::getElementName(ThemeTexture* texture) {
    return cThemeSet.getElement(texture);
  }

  std::string Theme::getElementName(ThemeColour* colour) {
    return cThemeSet.getElement(colour);
  }

  void Theme::hintInUse(bool inUse) {
    for (std::map<ThemeTexture*, Texture>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      i->second->hintTextureInUse(inUse);
    }
  }

  void Theme::releaseAssets(IAssets& releaser) {
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
    return *(cColours.find(colour)->second);
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

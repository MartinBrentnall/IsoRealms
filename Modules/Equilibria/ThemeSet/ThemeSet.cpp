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
#include "ThemeSet.h"

#include "Modules/Equilibria/Equilibria.h"

namespace IsoRealms::Equilibria {
  ThemeSet::ThemeSet(Equilibria& equilibria, IResourceData& data) :
            cEquilibria(equilibria),
            cResourceData(data),
            cDefaultTheme(nullptr),
            cAnimation(0),
            cPause(0),
            cThemeIcon(0),
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  ThemeSet::ThemeSet(Equilibria& equilibria, IResourceData& data, JSONObject object) :
            ThemeSet(equilibria, data) {
    for (JSONValue mThemeValue : object.getArray(JSON_THEMES)) {
      JSONObject mThemeObject = mThemeValue.getObject();
      cThemes[mThemeObject.getString(JSON_ID)] = std::make_unique<Theme>(*this, mThemeObject);
    }

    data.getProject().init([this]() {
      setNextTheme();
    });
  }

  void ThemeSet::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    
    // Texture elements of each theme in this set.
    owner.createPropertyStruct("TextureElements", "Edit...", [this, &metadata](IPropertyMaker& owner) {
      for (std::pair<std::string const, std::unique_ptr<ThemeTexture>>& mTexture : cTextures) {
        createTextureElementProperty(owner, metadata, mTexture.second.get());
      }
      
      owner.createPropertyAdd("TextureElementAdd", "Add...",  [this, &owner, &metadata]() {
        return createTextureElementProperty(owner, metadata, createTexture(Utils::getAvailableKey(cTextures, "New Texture")));
      });
    });
    
    // Colour elements of each theme in this set.
    owner.createPropertyStruct("ColourElements", "Edit...", [this, &metadata](IPropertyMaker& owner) {
      for (std::pair<std::string const, std::unique_ptr<ThemeColour>>& mColour : cColours) {
        createColourElementProperty(owner, metadata, mColour.second.get());
      }
      
      owner.createPropertyAdd("ColourElementAdd", "Add...",  [this, &owner, &metadata]() {
        return createColourElementProperty(owner, metadata, createColour(Utils::getAvailableKey(cColours, "New Colour")));
      });
    });
    
    // Actual themes in this set.
    owner.createPropertyStruct("Themes", "Edit...", [this, &metadata](IPropertyMaker& owner) {
      for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
        Theme* mExistingTheme = mTheme.second.get();
        owner.createPropertyStruct("Theme", mTheme.first, [this, &metadata, mExistingTheme](IPropertyMaker& owner) {
          return mExistingTheme->getProperties(owner, metadata);
        });
      }

      owner.createPropertyAdd("ThemeAdd", "Add...",  [this, &owner, &metadata]() {
        std::string mNewThemeName = Utils::getAvailableKey(cThemes, "New Theme");
        Theme* mNewTheme = cThemes.emplace(mNewThemeName, std::make_unique<Theme>(*this)).first->second.get();
        return owner.createPropertyStruct("Theme", mNewThemeName, [this, &metadata, mNewTheme](IPropertyMaker& owner) {
          return mNewTheme->getProperties(owner, metadata);
        });
      });
    });
  }

  bool ThemeSet::renderIcon() {
    return false;
  }
  
  void ThemeSet::save(JSONObject object) const {
    JSONArray mThemesArray = object.addArray(JSON_THEMES);
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      JSONObject mThemeObject = mThemesArray.addObject();
      mThemeObject.addString(JSON_ID, mTheme.first);
      mTheme.second->save(mThemeObject);
    }
  }

  void ThemeSet::removed() {
    // Nothing to do.
  }

  void ThemeSet::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Equilibria/Theme Sets");
    for (const std::pair<const std::string, std::unique_ptr<ThemeTexture>>& mPair : cTextures) {
      mPair.second->registerAssets(assets, mPair.first);
    }
    for (const std::pair<const std::string, std::unique_ptr<ThemeColour>>& mPair : cColours) {
      mPair.second->registerAssets(assets, mPair.first);
    }
  }

  bool ThemeSet::hasReadOnlyReferences() const {
    return cEquilibria.hasReadOnlyResourceReferences(this);
  }

  void ThemeSet::overrideReadOnlyReferences() {
    cEquilibria.overrideReadOnlyResourceReferences(this);
  }
  
  void ThemeSet::updateEditing(unsigned int milliseconds) {
    if (cPause > 0) {
      cPause -= milliseconds;
      if (cPause <= 0) {
        cAnimation = ICON_TRANSITION_TIME;
        cPause = 0;
        cThemeIcon++;
        if (cThemeIcon == cThemes.size()) {
          cThemeIcon = 0;
        }
      }
    } else {
      cAnimation -= milliseconds;
      if (cAnimation <= 0) {
        cAnimation = 0;
        cPause = ICON_PAUSE_TIME;
      }
    }
  }
  
  ThemeTexture* ThemeSet::createTexture(const std::string& type) {
    std::map<std::string, std::unique_ptr<ThemeTexture>>::iterator i = cTextures.find(type);
    if (i == cTextures.end()) {
      ThemeTexture* mNewTexture = cTextures.emplace(type, std::make_unique<ThemeTexture>(*this)).first->second.get();
      for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
        mTheme.second->themeTextureAdded(mNewTexture);
      }
      return mNewTexture;
    }
    return i->second.get();
  }

  ThemeColour* ThemeSet::createColour(const std::string& type) {
    std::map<std::string, std::unique_ptr<ThemeColour>>::iterator i = cColours.find(type);
    if (i == cColours.end()) {
      ThemeColour* mNewColour = cColours.emplace(type, std::make_unique<ThemeColour>(*this)).first->second.get();
      for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
        mTheme.second->themeColourAdded(mNewColour);
      }
      return mNewColour;
    }
    return i->second.get();
  }

  std::vector<ThemeTexture*> ThemeSet::getThemeTextures() const {
    std::vector<ThemeTexture*> mTextures;
    for (const std::pair<std::string const, std::unique_ptr<ThemeTexture>>& mTexture : cTextures) {
      mTextures.emplace_back(mTexture.second.get());
    }
    return mTextures;
  }
  
  std::vector<ThemeColour*> ThemeSet::getThemeColours() const {
    std::vector<ThemeColour*> mColours;
    for (const std::pair<std::string const, std::unique_ptr<ThemeColour>>& mColour : cColours) {
      mColours.emplace_back(mColour.second.get());
    }
    return mColours;
  }
  
  std::string ThemeSet::getElement(ThemeTexture* themeTexture) {
    for (std::map<std::string, std::unique_ptr<ThemeTexture>>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      if (i->second.get() == themeTexture) {
        return i->first;
      }
    }
    throw ArgumentException("ERROR: ThemeSet::getElement: Specified theme texture not found in this theme set.");
  }

  std::string ThemeSet::getElement(ThemeColour* themeColour) {
    for (std::map<std::string, std::unique_ptr<ThemeColour>>::iterator i = cColours.begin(); i != cColours.end(); i++) {
      if (i->second.get() == themeColour) {
        return i->first;
      }
    }
    throw ArgumentException("ERROR: ThemeSet::getElement: Specified theme colour not found in this theme set.");
  }

  std::string ThemeSet::getName(Theme* theme) {
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      if (mTheme.second.get() == theme) {
        return mTheme.first;
      }
    }
    return "";
  }

  void ThemeSet::setName(Theme& theme, const std::string& name) {
    std::string mOldName = getName(&theme);
    cThemes.emplace(name, std::move(cThemes[mOldName]));
    cThemes.erase(mOldName);
  }

  bool ThemeSet::isNameAllowed(Theme& theme, const std::string& name) {
    std::map<std::string, std::unique_ptr<Theme>>::iterator i = cThemes.find(name);
    if (i != cThemes.end()) {
      return i->second.get() == &theme;
    }
    return true;
  }
  
  ThemeTexture* ThemeSet::getTexture(const std::string& type) {
    return cTextures[type].get();
  }

  ThemeColour* ThemeSet::getColour(const std::string& type) {
    return cColours[type].get();
  }
  
  Theme* ThemeSet::getTheme(const std::string& name) {
    std::map<std::string, std::unique_ptr<Theme>>::iterator i = cThemes.find(name);
    if (i == cThemes.end()) {
      std::cout << "WARNING: Theme \"" << name << "\" doesn't exist.  Available themes:" << std::endl;
      for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
        std::cout << "  " << mTheme.first << std::endl;
      }
      std::cout << "End of theme list" << std::endl;
      throw ArgumentException("ERROR: ThemeSet::getTheme: Theme of specified name \"" + name + "\" not found in this theme set");
    }
    return i->second.get();
  }

  std::vector<Theme*> ThemeSet::getThemes() {
    std::vector<Theme*> mThemes;
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      mThemes.push_back(mTheme.second.get());
    }
    return mThemes;
  }
  
  void ThemeSet::hintInUse(bool inUse) {
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      mTheme.second->hintInUse(inUse);
    }
  }

  float ThemeSet::getAnimation() {
    return -(static_cast<float>(cAnimation) / static_cast<float>(ICON_TRANSITION_TIME)) + 1.0f;
  }

  ITexture* ThemeSet::getPreviousTexture(ThemeTexture* texture) {
    unsigned int mCount = 0;
    unsigned int mPreviousThemeIcon = cThemeIcon == 0 ? cThemes.size() - 1 : cThemeIcon - 1;
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      if (mCount == mPreviousThemeIcon) {
        return mTheme.second->getTexture(texture);
      }
      mCount++;
    }
    return nullptr;
  }

  ITexture* ThemeSet::getCurrentTexture(ThemeTexture* texture) {
    unsigned int mCount = 0;
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      if (mCount == cThemeIcon) {
        return mTheme.second->getTexture(texture);
      }
      mCount++;
    }
    return nullptr;
  }

  IColour* ThemeSet::getPreviousColour(ThemeColour* colour) {
    unsigned int mCount = 0;
    unsigned int mPreviousThemeIcon = cThemeIcon == 0 ? cThemes.size() - 1 : cThemeIcon - 1;
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      if (mCount == mPreviousThemeIcon) {
        return mTheme.second->getColour(colour);
      }
      mCount++;
    }
    return nullptr;
  }

  IColour* ThemeSet::getCurrentColour(ThemeColour* colour) {
    unsigned int mCount = 0;
    for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
      if (mCount == cThemeIcon) {
        return mTheme.second->getColour(colour);
      }
      mCount++;
    }
    return nullptr;
  }

  void ThemeSet::setDefaultTheme(Theme* theme) {
    if (cDefaultTheme != nullptr) {
      cDefaultTheme->hintInUse(false);
    }
    cDefaultTheme = theme;
    if (cDefaultTheme != nullptr) {
      cDefaultTheme->hintInUse(true);
    }
  }

  void ThemeSet::applyDefaultTheme() {
    if (cDefaultTheme != nullptr) {
//      std::cout << "APPLYING DEFAULT THEME: " << cDefaultTheme << std::endl;
      cDefaultTheme->set();
    }
  }

  Theme* ThemeSet::getDefaultTheme() {
    return cDefaultTheme;
  }
  
  Equilibria& ThemeSet::getEquilibria() {
    return cEquilibria;
  }

  IResourceData& ThemeSet::getResourceData() {
    return cResourceData;
  }

  void ThemeSet::setNextTheme() {
    std::vector<Theme*> mThemes = getThemes();
    if (cDefaultTheme == nullptr && !mThemes.empty()) {
      setDefaultTheme(mThemes[0]);
    } else for (unsigned int i = 0; i < mThemes.size(); i++) {
      if (mThemes[i] == cDefaultTheme) {
        setDefaultTheme(i == mThemes.size() - 1 ? mThemes[0] : mThemes[i + 1]);
        return;
      }
    }
  }

  void ThemeSet::setPreviousTheme() {
    std::vector<Theme*> mThemes = getThemes();
    if (cDefaultTheme == nullptr && !mThemes.empty()) {
      setDefaultTheme(mThemes[0]);
    } else for (unsigned int i = 0; i < mThemes.size(); i++) {
      if (mThemes[i] == cDefaultTheme) {
        setDefaultTheme(i == 0 ? mThemes[mThemes.size() - 1] : mThemes[i - 1]);
        return;
      }
    }
  }
  
  void ThemeSet::createTextureElementProperty(IPropertyMaker& owner, const Metadata& metadata, ThemeTexture* element) {
    owner.createPropertyNativeString(Theme::JSON_ELEMENT, [this, element]() {
      return getElement(element);
    }, [this, element](const std::string& value) {
      
      // Find existing element and move it.
      std::string mOldName = getElement(element);
      cTextures.emplace(value, std::move(cTextures[mOldName]));
      cTextures.erase(mOldName);
    }, [this, element](const std::string& value) {

      // Check the element name is not already in use.
      for (std::pair<std::string const, std::unique_ptr<ThemeTexture>>& mTexture : cTextures) {
        if (mTexture.first == value) {
          return mTexture.second.get() == element;
        }
      }
      return true;
    }, [this, element](){
      cTextures.erase(getElement(element));
    });
  }
  
  void ThemeSet::createColourElementProperty(IPropertyMaker& owner, const Metadata& metadata, ThemeColour* element) {
    owner.createPropertyNativeString(Theme::JSON_ELEMENT, [this, element]() {
      return getElement(element);
    }, [this, element](const std::string& value) {
      
      // Find existing element and move it.
      std::string mOldName = getElement(element);
      cColours.emplace(value, std::move(cColours[mOldName]));
      cColours.erase(mOldName);
    }, [this, element](const std::string& value) {
      for (std::pair<std::string const, std::unique_ptr<ThemeColour>>& mColour : cColours) {
        if (mColour.first == value) {
          return mColour.second.get() == element;
        }
      }
      return true;
    }, [this, element](){

      // Check the element name is not already in use.
      cColours.erase(getElement(element));
    });
  }
}

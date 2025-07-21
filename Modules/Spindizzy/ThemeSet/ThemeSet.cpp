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

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const unsigned int ThemeSet::ICON_TRANSITION_TIME = 500;
  const unsigned int ThemeSet::ICON_PAUSE_TIME      = 1000;

  const std::string ThemeSet::JSON_ID     = "id";
  const std::string ThemeSet::JSON_THEMES = "themes";

  ThemeSet::ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cResourceData(data),
            cDefaultTheme(nullptr),
            cAnimation(0),
            cPause(0),
            cThemeIcon(0),
            cLuaBinding(project, this) {
  }

  ThemeSet::ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            ThemeSet(project, spindizzy, data) {
    for (JSONObject mThemeObject : object.getArray(JSON_THEMES)) {
      cThemes[mThemeObject.getString(JSON_ID)] = std::make_unique<Theme>(project, *this, mThemeObject);
    }

    project.init([this](IAssets& assets) {
      setNextTheme();
    });
  }

  std::vector<std::unique_ptr<IProperty>> ThemeSet::getProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    
    // Texture elements of each theme in this set.
    mProperties.emplace_back(std::make_unique<PropertyStruct>(owner, owner.getPropertyData("TextureElements"), "Edit...", [this, &owner]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::pair<std::string const, std::unique_ptr<ThemeTexture>>& mTexture : cTextures) {
        mProperties.emplace_back(createTextureElementProperty(owner, mTexture.second.get()));
      }
      
      mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("Element"), "Add...",  [this, &owner]() {
        return createTextureElementProperty(owner, createTexture(Utils::getAvailableKey(cTextures, "New Texture")));
      }));
      return mProperties;
    }));
    
    // Colour elements of each theme in this set.
    mProperties.emplace_back(std::make_unique<PropertyStruct>(owner, owner.getPropertyData("ColourElements"), "Edit...", [this, &owner]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::pair<std::string const, std::unique_ptr<ThemeColour>>& mColour : cColours) {
        mProperties.emplace_back(createColourElementProperty(owner, mColour.second.get()));
      }
      
      mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("Element"), "Add...",  [this, &owner]() {
        return createColourElementProperty(owner, createColour(cSpindizzy.getProject(), Utils::getAvailableKey(cColours, "New Colour")));
      }));
      return mProperties;
    }));
    
    // Actual themes in this set.
    mProperties.emplace_back(std::make_unique<PropertyStruct>(owner, owner.getPropertyData("Themes"), "Edit...", [this, &owner]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (const std::pair<const std::string, std::unique_ptr<Theme>>& mTheme : cThemes) {
        Theme* mExistingTheme = mTheme.second.get();
        mProperties.emplace_back(std::make_unique<PropertyStruct>(owner, PropertyData(mTheme.first, "TODO"), "Edit...", [this, &owner, mExistingTheme]() {
          return mExistingTheme->getProperties(owner);
        }));
      }

      mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("Theme"), "Add...",  [this, &owner]() {
        std::string mNewThemeName = Utils::getAvailableKey(cThemes, "New Theme");
        Theme* mNewTheme = cThemes.emplace(mNewThemeName, std::make_unique<Theme>(cSpindizzy.getProject(), *this)).first->second.get();
        return std::make_unique<PropertyStruct>(owner, PropertyData(mNewThemeName, "TODO"), "Edit...", [this, &owner, mNewTheme]() {
          return mNewTheme->getProperties(owner);
        });
      }));
      return mProperties;
    }));
    return mProperties;
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

  void ThemeSet::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Spindizzy Theme Sets");
    for (const std::pair<const std::string, std::unique_ptr<ThemeTexture>>& mPair : cTextures) {
      mPair.second->registerAssets(assets, mPair.first);
    }
    for (const std::pair<const std::string, std::unique_ptr<ThemeColour>>& mPair : cColours) {
      mPair.second->registerAssets(assets, mPair.first);
    }
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

  ThemeColour* ThemeSet::createColour(IProject& project, const std::string& type) {
    std::map<std::string, std::unique_ptr<ThemeColour>>::iterator i = cColours.find(type);
    if (i == cColours.end()) {
      ThemeColour* mNewColour = cColours.emplace(type, std::make_unique<ThemeColour>(project, *this)).first->second.get();
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

  bool ThemeSet::setName(Theme& theme, const std::string& name) {
    std::map<std::string, std::unique_ptr<Theme>>::iterator i = cThemes.find(name);
    if (i != cThemes.end()) {
      return i->second.get() == &theme;
    }

    std::string mOldName = getName(&theme);
    cThemes.emplace(name, std::move(cThemes[mOldName]));
    cThemes.erase(mOldName);
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
  
  Spindizzy& ThemeSet::getSpindizzy() {
    return cSpindizzy;
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
  
  std::unique_ptr<IProperty> ThemeSet::createTextureElementProperty(IPropertyOwner& owner, ThemeTexture* element) {
    return std::make_unique<PropertyNativeString>(PropertyData("Element", "TODO"), [this, element]() {
      return getElement(element);
    }, [this, element](const std::string& value) {
      
      // Check the element name is not already in use.
      for (std::pair<std::string const, std::unique_ptr<ThemeTexture>>& mTexture : cTextures) {
        if (mTexture.first == value) {
          return mTexture.second.get() == element;
        }
      }          
      
      // Find existing element and move it.
      std::string mOldName = getElement(element);
      cTextures.emplace(value, std::move(cTextures[mOldName]));
      cTextures.erase(mOldName);
      return true;
    }, [this, element](){
      cTextures.erase(getElement(element));
    });
  }
  
  std::unique_ptr<IProperty> ThemeSet::createColourElementProperty(IPropertyOwner& owner, ThemeColour* element) {
    return owner.createPropertyNativeString("Element", [this, element]() {
      return getElement(element);
    }, [this, element](const std::string& value) {
      
      // Check the element name is not already in use.
      for (std::pair<std::string const, std::unique_ptr<ThemeColour>>& mColour : cColours) {
        if (mColour.first == value) {
          return mColour.second.get() == element;
        }
      }          
      
      // Find existing element and move it.
      std::string mOldName = getElement(element);
      cColours.emplace(value, std::move(cColours[mOldName]));
      cColours.erase(mOldName);
      return true;
    }, [this, element](){
      cColours.erase(getElement(element));
    });
  }
}

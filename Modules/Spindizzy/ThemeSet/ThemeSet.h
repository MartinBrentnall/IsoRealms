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
#pragma once

#include <map>

#include "IsoRealms.h"

#include "Theme.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  class ThemeSet {
    public:
    ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void getProperties(PropertyMaker& owner);
    bool renderIcon();
    void hintInUse(bool inUse);
    void save(JSONObject object) const;
    void registerAssets(ResourceAssetRegistry& assets);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);
    
    ThemeTexture* createTexture(const std::string& type);
    ThemeColour* createColour(IProject& project, const std::string& type);
    std::vector<ThemeTexture*> getThemeTextures() const;
    std::vector<ThemeColour*> getThemeColours() const;
    std::string getElement(ThemeTexture* themeTexture);
    std::string getElement(ThemeColour* themeColour);
    std::string getName(Theme* theme);
    void setName(Theme& theme, const std::string& name);
    bool isNameAllowed(Theme& theme, const std::string& name);
    ThemeTexture* getTexture(const std::string& type);
    ThemeColour* getColour(const std::string& type);
    Theme* getTheme(const std::string& name);
    std::vector<Theme*> getThemes();
    void setDefaultTheme(Theme* theme);
    void applyDefaultTheme();
    Theme* getDefaultTheme();
    Spindizzy& getSpindizzy();
    IResourceData& getResourceData();

    void setNextTheme();
    void setPreviousTheme();

    float getAnimation();
    ITexture* getPreviousTexture(ThemeTexture* texture);
    ITexture* getCurrentTexture(ThemeTexture* texture);
    IColour* getPreviousColour(ThemeColour* colour);
    IColour* getCurrentColour(ThemeColour* colour);

    private:
    static const unsigned int ICON_TRANSITION_TIME;
    static const unsigned int ICON_PAUSE_TIME;

    // JSON strings
    static const std::string JSON_ID;
    static const std::string JSON_THEMES;

    // External interfaces.
    Spindizzy& cSpindizzy;
    IResourceData& cResourceData;

    // Definition data.
    std::map<std::string, std::unique_ptr<Theme>> cThemes;
    std::map<std::string, std::unique_ptr<ThemeTexture>> cTextures;
    std::map<std::string, std::unique_ptr<ThemeColour>> cColours;
    Theme* cDefaultTheme;

    // Editor icon animation
    int cAnimation;
    int cPause;
    unsigned int cThemeIcon;

    LuaBinding<ThemeSet> cLuaBinding;
    
    void createTextureElementProperty(PropertyMaker& owner, ThemeTexture* element);
    void createColourElementProperty(PropertyMaker& owner, ThemeColour* element);
  };
}

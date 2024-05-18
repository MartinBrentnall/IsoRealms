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
#pragma once

#include <map>

#include "IsoRealms/ResourceDefinition.h"

#include "Theme.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  class ThemeSet : public IIconAnimator {
    public:
    ThemeSet(IProject* project, Spindizzy* spindizzy);
    ThemeSet(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data);
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);
    bool renderIcon();
    void hintInUse(bool inUse);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    
    ThemeTexture* createTexture(const std::string& type);
    ThemeColour* createColour(IProject* project, const std::string& type);
    std::string getElement(ThemeTexture* themeTexture);
    std::string getElement(ThemeColour* themeColour);
    std::string getName(Theme* theme);
    ThemeTexture* getTexture(const std::string& type);
    ThemeColour* getColour(const std::string& type);
    Theme* getTheme(const std::string& name);
    std::vector<Theme*> getThemes();
    void setDefaultTheme(Theme* theme);
    void applyDefaultTheme();
    Theme* getDefaultTheme();

    void setNextTheme();
    void setPreviousTheme();

    /****************************\
     * Implements IIconAnimator *
    \****************************/
    float getAnimation() override;
    ITexture* getPreviousTexture(ThemeTexture*) override;
    ITexture* getCurrentTexture(ThemeTexture*) override;
    IColour* getPreviousColour(ThemeColour*) override;
    IColour* getCurrentColour(ThemeColour*) override;

    private:
    static const unsigned int ICON_TRANSITION_TIME;
    static const unsigned int ICON_PAUSE_TIME;

    // JSON strings
    static const std::string JSON_ID;
    static const std::string JSON_THEMES;

    std::map<std::string, std::unique_ptr<Theme>> cThemes;
    std::map<std::string, std::unique_ptr<ThemeTexture>> cTextures;
    std::map<std::string, std::unique_ptr<ThemeColour>> cColours;
    Theme* cDefaultTheme;

    // Icon animation
    int cAnimation;
    int cPause;
    unsigned int cThemeIcon;

    LuaBinding<ThemeSet> cLuaBinding;
  };
}

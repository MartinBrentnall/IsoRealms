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
    ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    ThemeSet(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);
    bool renderIcon();
    void hintInUse(bool inUse);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    
    ThemeTexture* createTexture(const std::string& type);
    ThemeColour* createColour(IProject& project, const std::string& type);
    std::vector<ThemeTexture*> getThemeTextures() const;
    std::vector<ThemeColour*> getThemeColours() const;
    std::string getElement(ThemeTexture* themeTexture);
    std::string getElement(ThemeColour* themeColour);
    std::string getName(Theme* theme);
    bool setName(Theme& theme, const std::string& name);
    ThemeTexture* getTexture(const std::string& type);
    ThemeColour* getColour(const std::string& type);
    Theme* getTheme(const std::string& name);
    std::vector<Theme*> getThemes();
    void setDefaultTheme(Theme* theme);
    void applyDefaultTheme();
    Theme* getDefaultTheme();
    Spindizzy& getSpindizzy();

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

    // External interfaces.
    Spindizzy& cSpindizzy;

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
    
    std::unique_ptr<IProperty> createTextureElementProperty(ThemeTexture* element);
    std::unique_ptr<IProperty> createColourElementProperty(ThemeColour* element);
  };
}

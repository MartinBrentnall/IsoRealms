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

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Types.h"

#include "ThemeColour.h"
#include "ThemeTexture.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class ThemeSet;
  
  class Theme : public IStateListener<ITexture*> {
    private:
    static const std::string JSON_COLOUR;
    static const std::string JSON_COLOURS;
    static const std::string JSON_ELEMENT;
    static const std::string JSON_TEXTURE;
    static const std::string JSON_TEXTURES;

    ThemeSet* cThemeSet;
    std::map<ThemeTexture*, Texture> cTextures;
    std::map<ThemeColour*, Colour> cColours;
    Spindizzy* cSpindizzy;

    public:
    Theme(IProject* project, Spindizzy* spindizzy, ThemeSet* themeSet, JSONObject object);

    void save(JSONObject object, IAssetIdentifier* identifier) const;

    ITexture* getTexture(ThemeTexture*);
    void removeTexture(ThemeTexture*);
    void removeColour(ThemeColour*);
    
    /*********************\
     * Implements ITheme *
    \*********************/
    IColour* getColour(ThemeColour*);
    void set();
    
    std::string getName();
    std::string getElementName(ThemeTexture*);
    std::string getElementName(ThemeColour*);
    void hintInUse(bool);

    void releaseAssets(IAssets* releaser);
    
    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged(ITexture* asset) override;

    virtual ~Theme() {
    }
  };
}

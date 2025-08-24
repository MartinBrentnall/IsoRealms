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

#include "ThemeColour.h"
#include "ThemeTexture.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  class ThemeSet;
  
  class Theme : public IStateListener<ITexture*> {
    public:
    Theme(ThemeSet& themeSet);
    Theme(ThemeSet& themeSet, JSONObject object);

    void save(JSONObject object) const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    void themeTextureAdded(ThemeTexture* texture);
    void themeColourAdded(ThemeColour* colour);


    ITexture* getTexture(ThemeTexture* texture);
    void removeTexture(ThemeTexture* texture);
    void removeColour(ThemeColour* colour);

    IColour* getColour(ThemeColour* colour);
    void set();
    
    std::string getName();
    std::string getElementName(ThemeTexture* texture);
    std::string getElementName(ThemeColour* colour);
    void hintInUse(bool);

    /****************************************\
     * Implements IStateListener<ITexture*> *
    \****************************************/
    void stateChanged(ITexture* asset) override;

    virtual ~Theme() {
    }

    private:
    static const std::string JSON_COLOUR;
    static const std::string JSON_COLOURS;
    static const std::string JSON_ELEMENT;
    static const std::string JSON_TEXTURE;
    static const std::string JSON_TEXTURES;

    // External interfaces.
    ThemeSet& cThemeSet;
    
    // Definition data.
    std::map<ThemeTexture*, Texture> cTextures;
    std::map<ThemeColour*, Colour> cColours;
  };
}

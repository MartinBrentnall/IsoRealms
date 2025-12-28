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

#include <string>

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a terrain state.  A terrain state may be used to
   * control the presence of terrain objects via conditions.
   */
  class TerrainState final : public IBoolean,
                             public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    TerrainState(Spindizzy& spindizzy, IResourceData& data);
    TerrainState(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /**
     * Get the condition element that represents this terrain state.
     */
    ConditionElement& getConditionElement();

    /**
     * Perform the action to give a hint that a change to this state is needed.
     */
    void giveHint();

    /***********************\
     * Implements IBoolean *
    \***********************/
    bool getValue() const override;

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(bool value);

    private:

    // JSON members.
    static const std::string JSON_HINT;
    static const std::string JSON_ICON;
    static const std::string JSON_ICON_SCALE;
    static const std::string JSON_ID;
    static const std::string JSON_STATE;

    // External interfaces.
    IResourceData& cResourceData;

    // Definition data.
    ConditionElement cDefConditionElement; /// Condition element representing this terrain state.
    bool cDefValue;                        /// Initial value of this terrain state.
    Action cDefHintAction;                 /// Action to perform when hinting of a required change to this state.
    Screen cDefIcon;                       /// Icon to represent this state when editing.
    float cDefIconScale;

    // Runtime data.
    bool cRuntimeValue; /// Current value of this terrain state.

    // Action interface.
    LuaBinding<TerrainState> cLuaBinding;

    /**
     * Construct the terrain state according to specified parameters.
     *
     * @param name ID of this terrain state.
     * @param value initial value of this terrain state.
     */
    TerrainState(IResourceData& owner, bool value, float iconScale);
  };
}

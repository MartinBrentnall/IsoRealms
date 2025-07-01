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
#include "TerrainState.h"

namespace IsoRealms::Spindizzy {
  const std::string TerrainState::JSON_HINT       = "hint";
  const std::string TerrainState::JSON_ICON       = "icon";
  const std::string TerrainState::JSON_ICON_SCALE = "iconScale";
  const std::string TerrainState::JSON_ID         = "id";
  const std::string TerrainState::JSON_STATE      = "state";

  TerrainState::TerrainState(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            TerrainState(project, data, "TODO", true, 1.0f) {
  }

  TerrainState::TerrainState(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            TerrainState(project, data, object.getString(JSON_ID), object.getBoolean(JSON_STATE), object.getFloat(JSON_ICON_SCALE, 1.0f)) {
    cDefIcon.init(object, JSON_ICON);
    cDefHintAction.init(object, JSON_HINT);
  }

  void TerrainState::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IBoolean*>(this), "", "Spindizzy Terrain States");
    assets.add(&cLuaBinding, "", "Spindizzy Terrain States");
  }

  void TerrainState::save(JSONObject object) const {
    object.addBoolean(JSON_STATE, cDefValue);
    cDefIcon.save(object, JSON_ICON);
    object.addFloat(JSON_ICON_SCALE, cDefIconScale);
    cDefHintAction.save(object, JSON_HINT);
  }

  void TerrainState::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool TerrainState::renderIcon() const {
    cDefIcon->renderScreen(1.0f, 1.0f);
    return true;
  }

  std::vector<std::unique_ptr<IProperty>> TerrainState::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Initial State", "TODO", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;}, owner.getProject()));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Hint Action",   "TODO", cDefHintAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Screen>>("Icon",          "TODO", cDefIcon));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>(  "Icon Scale",    "TODO", [this]() {return cDefIconScale;}, [this](float value) {if (value > 0.0f) {
      cDefIconScale = value;
      return true;
    }
    return false;
    }));
    return mProperties;
  }

  ConditionElement& TerrainState::getConditionElement() {
    return cDefConditionElement;
  }

  void TerrainState::giveHint() {
    cDefHintAction.execute();
  }

  bool TerrainState::getValue() const {
    return cRuntimeValue;
  }

  void TerrainState::renderScreen(float scale, float aspectRatio) const {
    glPushMatrix();
    glScalef(cDefIconScale, cDefIconScale, 0.0f);
    cDefIcon->renderScreen(scale, aspectRatio);
    glPopMatrix();
  }

  bool TerrainState::renderAssetIcon() const {
    return renderIcon();
  }

  void TerrainState::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> TerrainState::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool TerrainState::isDefaultConfiguration() const {
    return true;
  }

  void TerrainState::setValue(bool value) {
    cRuntimeValue = value;
  }

  TerrainState::TerrainState(IProject& project, IResourceData& owner, const std::string& name, bool state, float iconScale) :
            cProjectCallbackManager(project),
            cDefConditionElement(name, this, this),
            cDefValue(state),
            cDefHintAction(owner),
            cDefIcon(owner),
            cDefIconScale(iconScale),
            cRuntimeValue(state),
            cLuaBinding(project, this, [this]() {return renderAssetIcon();}) {
    cProjectCallbackManager.reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
}

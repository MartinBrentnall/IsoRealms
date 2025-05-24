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
  const std::string TerrainState::JSON_HINT  = "hint";
  const std::string TerrainState::JSON_ICON  = "icon";
  const std::string TerrainState::JSON_ID    = "id";
  const std::string TerrainState::JSON_STATE = "state";

  TerrainState::TerrainState(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            TerrainState(project, "TODO", true) {
  }

  TerrainState::TerrainState(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            TerrainState(project, object.getString(JSON_ID), object.getBoolean(JSON_STATE)) {
    cDefIcon.init(object, JSON_ICON);
    cDefHintAction.init(object, JSON_HINT);
  }

  void TerrainState::registerAssets(IAssetRegistry& assets) {
    assets.add(static_cast<IBoolean*>(this), "", "Spindizzy Terrain States");
    assets.add(&cLuaBinding, "", "Spindizzy Terrain States");
  }

  void TerrainState::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    assets.remove(static_cast<IBoolean*>(this), relinquish);
    assets.remove(&cLuaBinding,                 relinquish);
  }

  void TerrainState::save(JSONObject object, IAssetIdentifier& identifier) const {
    object.addBoolean(JSON_STATE, cDefValue);
    cDefIcon.save(object, JSON_ICON);
    cDefHintAction.save(object, JSON_HINT);
  }

  void TerrainState::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool TerrainState::renderIcon() const {
    glScalef(2.0f, 2.0f, 1.0f);
    cDefIcon->renderScreen(1.0f, 1.0f);
    return true;
  }

  std::vector<std::unique_ptr<IProperty>> TerrainState::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Initial State", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;}, browser.getProject()));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Hint Action",   cDefHintAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Screen>>("Icon",          cDefIcon));
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
    cDefIcon->renderScreen(scale, aspectRatio);
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

  TerrainState::TerrainState(IProject& project, const std::string& name, bool state) :
            cDefConditionElement(name, this, this),
            cDefValue(state),
            cDefHintAction(project),
            cDefIcon(project),
            cRuntimeValue(state),
            cLuaBinding(project, this, [this]() {return renderAssetIcon();}) {
    project.reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
}

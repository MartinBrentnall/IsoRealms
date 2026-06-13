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
#include "TerrainState.h"

namespace IsoRealms::Equilibria {
  TerrainState::TerrainState(Equilibria& equilibria, IComponentData& data) :
            TerrainState(data, true, 1.0f) {
  }

  void TerrainState::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IBoolean>(this, "", "Equilibria Terrain States");
    assets.add<IBinding>(&cLuaBinding, "", "Equilibria/Terrain States");
  }

  bool TerrainState::renderIcon() const {
    cDefIcon->renderScreen(1.0f, 1.0f);
    return true;
  }

  void TerrainState::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeBoolean("state",     [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;});
    owner.createPropertyTreeSelector( "hint",      cDefHintAction);
    owner.createPropertyTreeSelector( "icon",      cDefIcon);
    owner.createPropertyNativeFloat(  "iconScale", [this]() {return cDefIconScale;}, [this](float value) {cDefIconScale = value;}, 1.0f, [](float value) {return value > 0.0f;});
  }
  
  void TerrainState::reset() {
    cRuntimeValue = cDefValue;
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

  void TerrainState::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool TerrainState::isDefaultConfiguration() const {
    return true;
  }

  void TerrainState::setValue(bool value) {
    cRuntimeValue = value;
  }

  TerrainState::TerrainState(IComponentData& owner, bool state, float iconScale) :
            cComponentData(owner),
            cDefConditionElement([this]() {
              std::string mComponentID = cComponentData.getComponentID();
              return mComponentID.substr(mComponentID.find_last_of('/') + 1);
            }, *this, this),
            cDefValue(state),
            cDefHintAction(owner.getDummyActionContext()),
            cDefIcon(owner),
            cDefIconScale(iconScale),
            cRuntimeValue(state),
            cLuaBinding(owner.getProject().getLuaState(), this, [this]() {return renderAssetIcon();}) {
  }
}

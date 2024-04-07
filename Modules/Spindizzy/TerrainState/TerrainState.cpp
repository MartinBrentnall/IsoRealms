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
  const std::string TerrainState::TAG_HINT_ACTION = "HintAction";
  const std::string TerrainState::TAG_SCREEN      = "Screen";

  TerrainState::TerrainState(IProject* project, Spindizzy* spindizzy) :
            TerrainState(project, "TODO", true) {
  }

  TerrainState::TerrainState(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            TerrainState(project, node.getAttribute("name"), node.getBooleanAttribute("state")) {
    cDefIcon.init(node, TAG_SCREEN);
    cDefHintAction.init(node, TAG_HINT_ACTION);
  }

  void TerrainState::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IBoolean*>(this), "", "Spindizzy Terrain States");
    assets->add(&cLuaBinding, "", "Spindizzy Terrain States");
  }

  void TerrainState::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IBoolean*>(this));
    assets->remove(&cLuaBinding);
  }

  void TerrainState::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute("name", cDefConditionElement.getName());
    node->addAttribute("state", cDefValue);
    cDefIcon.save(node, TAG_SCREEN);
    cDefHintAction.save(node, TAG_HINT_ACTION);
  }

  void TerrainState::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool TerrainState::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> TerrainState::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  void TerrainState::setValue(bool value) {
    cRuntimeValue = value;
  }

  TerrainState::TerrainState(IProject* project, const std::string& name, bool state) :
            cDefConditionElement(name, this, this),
            cDefValue(state),
            cDefHintAction(project),
            cDefIcon(project),
            cRuntimeValue(state),
            cLuaBinding(project, this) {
    project->reset([this]() {
      cRuntimeValue = cDefValue;
    });
  }
}

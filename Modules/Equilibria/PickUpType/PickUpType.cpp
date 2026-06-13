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
//  * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PickUpType.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  PickUpType::PickUpType(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cAssets(equilibria),
            cDefModel(data) {
    cEquilibria.added(this);
  }
  
  void PickUpType::registerAssets(ComponentAssetRegistry& assets) {
    // Nothing to do.
  }

  void PickUpType::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool PickUpType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  void PickUpType::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("appearance", cDefModel);
  }

  void PickUpType::removed() {
    cEquilibria.removeAll(this);
    cEquilibria.remove(this);
    cEquilibria.removed(this);
  }

  bool PickUpType::hasReadOnlyReferences() const {
    return cEquilibria.isUsedInReadOnlyWorld(*this);
  }

  void PickUpType::overrideReadOnlyReferences() {
    cEquilibria.overrideReadOnlyWorlds(*this);
  }

  void PickUpType::registerAssets(const std::string& parentID) {
    cAssets.add<IWorldEditorTool>(this, parentID, "Pick Up Types");
    cAssets.add<IBoundaryType>(   this, parentID, "Pick Up Types");
  }  
  
  std::unique_ptr<ModelInstance> PickUpType::createModel() {
    return cDefModel.createInstance();
  }

  std::string PickUpType::getBoundaryTypeID() const {
    return "PickUp/" + cEquilibria.getComponentID(this);
  }
  
  IBinding* PickUpType::getBounderyTypeBinding(const std::string& id) const {
    return cEquilibria.getBindingPickUp(id);
  }

  std::string PickUpType::getBoundaryTypeBindingID(const IBinding* binding) const {
    return cEquilibria.getBindingIDPickUp(binding);
  }

  void PickUpType::forEachAvailableBoundaryTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cEquilibria.getTreeItemsPickUp(getTreeItemInfoFunction);
  }

  IWorldEditorToolInstance* PickUpType::createToolInstance(WorldEditor& editor, IComponentData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PickUpType::renderAssetIcon() const {
    return cDefModel.renderIcon();
  }

  void PickUpType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void PickUpType::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool PickUpType::isDefaultConfiguration() const {
    return true;
  }

  PickUpType::Pen::Pen(PickUpType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool PickUpType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool PickUpType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void PickUpType::Pen::renderEditingPreview() const {
    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    cParent.cDefModel.renderPreview();
  }

  void PickUpType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void PickUpType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool PickUpType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool PickUpType::Pen::inputTool(SignalInputID id, double yaw) {
    if (id == SignalInputID::USE_TOOL) {
      cEditor.getWorld().draw(cParent, cEditor.getCursorCell(), cEditor);
      return true;
    }
    return false;
  }

  bool PickUpType::Pen::isCursorLocked() const {
    return false;
  }

  void PickUpType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double PickUpType::Pen::getSnapInterval() const {
    return 1.0;
  }
}

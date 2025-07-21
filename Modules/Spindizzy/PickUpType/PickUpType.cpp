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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string PickUpType::JSON_APPEARANCE = "appearance";

  PickUpType::PickUpType(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cDefModel(data) {
    cSpindizzy.added(this);
  }
  
  PickUpType::PickUpType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            PickUpType(project, spindizzy, data) {
    cDefModel.init(object, JSON_APPEARANCE);
  }

  void PickUpType::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  void PickUpType::save(JSONObject object) const {
    cDefModel.save(object, JSON_APPEARANCE);
  }

  void PickUpType::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool PickUpType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  std::vector<std::unique_ptr<IProperty>> PickUpType::getProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Model>>(owner.getPropertyData("Appearance"), cDefModel));
    return mProperties;
  }

  PickUpType::~PickUpType() {
    cSpindizzy.remove(this);
    cSpindizzy.removed(this);
    cSpindizzy.removeAll(this);
  }

  void PickUpType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(this), "");
    registry->add(static_cast<IBoundaryType*>(   this), "");
  }  
  
  std::unique_ptr<ModelInstance> PickUpType::createModel() {
    return cDefModel.createInstance();
  }

  std::string PickUpType::getBoundaryTypeID() const {
    return "PickUp/" + cSpindizzy.getID(this);
  }
  
  IBinding* PickUpType::getBinding(const std::string& id) const {
    return nullptr;
  }

  std::string PickUpType::getBindingID(const IBinding* binding) const {
    return "";
  }

  IWorldEditorToolInstance* PickUpType::createToolInstance(WorldEditor& editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PickUpType::renderAssetIcon() const {
    return cDefModel.renderIcon();
  }

  void PickUpType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> PickUpType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
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

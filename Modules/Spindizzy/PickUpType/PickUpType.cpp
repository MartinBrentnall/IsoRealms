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
//  * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "PickUpType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string PickUpType::JSON_APPEARANCE = "appearance";

  PickUpType::PickUpType(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy),
            cDefModel(project) {
    cDefSpindizzy.added(this);
  }
  
  PickUpType::PickUpType(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data) :
            PickUpType(project, spindizzy) {
    cDefModel.init(object, JSON_APPEARANCE);
  }

  void PickUpType::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }

  void PickUpType::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }

  void PickUpType::save(JSONObject object, IAssetIdentifier* identifier) const {
    cDefModel.save(object, JSON_APPEARANCE);
  }

  void PickUpType::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool PickUpType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  std::vector<IProperty*> PickUpType::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  PickUpType::~PickUpType() {
    cDefSpindizzy.removed(this);
    cDefSpindizzy.removeAll(this);
  }

  void PickUpType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(this), "");
    registry->add(static_cast<IBoundaryType*>(   this), "");
  }  
  
  std::unique_ptr<ModelInstance> PickUpType::createModel() {
    return cDefModel.createInstance();
  }

  std::string PickUpType::getBoundaryTypeID() const {
    return "PickUp/" + cDefSpindizzy.getID(this);
  }
  
  IBinding* PickUpType::getBinding(const std::string& id) const {
    return nullptr;
  }

  std::string PickUpType::getBindingID(const IBinding* binding) const {
    return "";
  }

  IWorldEditorToolInstance* PickUpType::createToolInstance(WorldEditor* editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PickUpType::renderAssetIcon() const {
    return false;
  }

  void PickUpType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  PickUpType::Pen::Pen(PickUpType& parent, WorldEditor* editor) :
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
    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    cParent.cDefModel.renderPreview();
  }

  void PickUpType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void PickUpType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool PickUpType::Pen::inputEdit(sf::Event& event, double yaw) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: cEditor->getWorld()->draw(&cParent, cEditor->getCursorCell(), cEditor); return true;
          default: break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: cEditor->getWorld()->draw(&cParent, cEditor->getCursorCell(), cEditor); return true;
        }
        break;
      }

      default: break;
    }
    return false;
  }

  void PickUpType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double PickUpType::Pen::getSnapInterval() const {
    return 1.0;
  }
}

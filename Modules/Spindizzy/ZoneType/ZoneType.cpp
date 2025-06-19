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
#include "ZoneType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  ZoneType::ZoneType(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy) {
    cSpindizzy.added(this);
  }
  
  ZoneType::ZoneType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            ZoneType(project, spindizzy, data) {
  }

  void ZoneType::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void ZoneType::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    // Nothing to do.
  }
  
  void ZoneType::save(JSONObject object, IAssetIdentifier& identifier) const {
    // Nothing to do.
  }

  void ZoneType::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool ZoneType::renderIcon() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW, 0.0f, 0.0f, 1.0f);
    // TODO: Scale the icon
    glScalef(1.3f, 1.3f, 1.3f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    return true;
  }

  std::vector<std::unique_ptr<IProperty>> ZoneType::getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  ZoneType::~ZoneType() {
    cSpindizzy.removeAll(this);
    cSpindizzy.removed(this);
  }

  void ZoneType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(this), "");
    registry->add(static_cast<IBoundaryType*>(   this), "");
  }  
  
  void ZoneType::registerZoneProperty(const std::string& id, IBinding* property) {
    cZoneProperties[id] = property;
  }
  
  void ZoneType::unregisterZoneProperty(const std::string& id) {
    cZoneProperties.erase(id);    
  }
  
  std::string ZoneType::getBoundaryTypeID() const {
    return "Zone/" + cSpindizzy.getID(this);
  }  
  
  IBinding* ZoneType::getBinding(const std::string& id) const {
    return cSpindizzy.getZoneBinding(id);
  }
  
  std::string ZoneType::getBindingID(const IBinding* binding) const {
    return cSpindizzy.getZoneBindingID1(binding);
  }
  
  IWorldEditorToolInstance* ZoneType::createToolInstance(WorldEditor& editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool ZoneType::renderAssetIcon() const {
    return renderIcon();
  }

  void ZoneType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneType::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneType::isDefaultConfiguration() const {
    return true;
  }

  ZoneType::Pen::Pen(ZoneType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool ZoneType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool ZoneType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void ZoneType::Pen::renderEditingPreview() const {
    float mPinnedX = cDrawing ? cPinnedLocation.cDefX : cEditor.getCursorX();
    float mPinnedY = cDrawing ? cPinnedLocation.cDefY : cEditor.getCursorY();
    float mPinnedZ = cDrawing ? cPinnedLocation.cDefZ : cEditor.getCursorZ();
    if (cEditor.getWorld().intersectsZone(std::round(mPinnedX), std::round(mPinnedY), std::round(mPinnedZ), std::round(cEditor.getCursorX()), std::round(cEditor.getCursorY()), std::round(cEditor.getCursorZ()))) {
      glColor3f(1.0f, 0.0f, 0.0f);
    } else {
      glColor3f(0.0f, 1.0f, 0.0f);
    }
    glBegin(GL_LINES);
    Utils::renderVolumeLines(std::min(cEditor.getCursorX(), mPinnedX) - 0.5f,
                             std::min(cEditor.getCursorY(), mPinnedY) - 0.5f,
                            (std::min(cEditor.getCursorZ(), mPinnedZ) - 1) * 0.5f,
                             std::max(cEditor.getCursorX(), mPinnedX) + 0.5f,
                             std::max(cEditor.getCursorY(), mPinnedY) + 0.5f,
                             std::max(cEditor.getCursorZ(), mPinnedZ) * 0.5f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void ZoneType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void ZoneType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool ZoneType::Pen::inputTool(SignalInputID id, double yaw) {
    switch (id) {
      case SignalInputID::USE_TOOL: {
        if (!cDrawing) {
          if (cEditor.getWorld().getZone(cEditor.getCursorCell()) == nullptr) {
            cDrawing = true;
            cPinnedLocation = cEditor.getCursorCell();
          }
        } else {
          Zone* mZone = cEditor.getWorld().draw(cParent, cPinnedLocation, cEditor.getCursorCell(), cEditor, nullptr);
          if (mZone != nullptr) {
            mZone->registerView(cEditor);
            cDrawing = false;
          }
        }
        return true;
      }
      default: break;
    }
    return false;
  }

  bool ZoneType::Pen::isCursorLocked() const {
    return false;
  }

  void ZoneType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double ZoneType::Pen::getSnapInterval() const {
    return 1.0;
  }
}

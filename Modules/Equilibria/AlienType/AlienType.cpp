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
#include "AlienType.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  AlienType::AlienType(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cAssets(equilibria),
            cDefModel(data),
            cDefTarget(data),
            cDefAcceleration(DEFAULT_ACCELERATION),
            cDefFriction(DEFAULT_FRICTION),
            cDefSpinSpeed(DEFAULT_SPIN_SPEED),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefHugMomentum(DEFAULT_HUG_MOMENTUM),
            cLuaBinding(data.getProject().getLuaState(), this, [this]() {return renderAssetIcon();}) {
    cEquilibria.added(this);
  }
  
  void AlienType::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Equilibria/Aliens");
  }
    
  
  bool AlienType::renderIcon() const {
    glColor3f(1.0f, 1.0f, 1.0f);
    return cDefModel.renderIcon();
  }

  void AlienType::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("appearance",   cDefModel);
    owner.createPropertyTreeSelector("target",       cDefTarget);
    owner.createPropertyNativeFloat( "acceleration", [this]() {return cDefAcceleration;}, [this](float value) {cDefAcceleration = value;}, DEFAULT_ACCELERATION);
    owner.createPropertyNativeFloat( "friction",     [this]() {return cDefFriction;},     [this](float value) {cDefFriction     = value;}, DEFAULT_FRICTION);
    owner.createPropertyNativeFloat( "spinSpeed",    [this]() {return cDefSpinSpeed;},    [this](float value) {cDefSpinSpeed    = value;});
    owner.createPropertyNativeFloat( "height",       [this]() {return cDefHeight;},       [this](float value) {cDefHeight       = value;}, DEFAULT_HEIGHT);
    owner.createPropertyNativeFloat( "radius",       [this]() {return cDefRadius;},       [this](float value) {cDefRadius       = value;}, DEFAULT_RADIUS);
    owner.createPropertyNativeFloat( "hugMomentum",  [this]() {return cDefHugMomentum;},  [this](float value) {cDefHugMomentum  = value;}, DEFAULT_HUG_MOMENTUM);
  }

  void AlienType::removed() {
    cAssets.clear();
    cEquilibria.removeAll(this);
    cEquilibria.removed(this);
  }

  bool AlienType::hasReadOnlyReferences() const {
    return cEquilibria.isUsedInReadOnlyWorld(*this);
  }

  void AlienType::overrideReadOnlyReferences() {
    cEquilibria.overrideReadOnlyWorlds(*this);
  }

  void AlienType::reset() {
    cRuntimeSpinSpeed = cDefSpinSpeed;
  }

  void AlienType::registerAssets(const std::string& parentID) {
    cAssets.add(static_cast<IWorldEditorTool*>(   this), parentID, "Alien Types");
    cAssets.add(static_cast<IPhysicalObjectType*>(this), parentID, "Alien Types");
  }  
  
  std::unique_ptr<ModelInstance> AlienType::createModel() {
    return cDefModel.createInstance();
  }

  IVertex* AlienType::getTarget() {
    return *cDefTarget;
  }

  float AlienType::getAcceleration() {
    return cDefAcceleration;
  }

  float AlienType::getFriction() {
    return 1.0f - cDefFriction;
  }

  float AlienType::getSpinSpeed() {
    return cRuntimeSpinSpeed;
  }

  float AlienType::getStepReach() const {
    return 0.5f;
  }

  float AlienType::getHeight() const {
    return cDefHeight;
  }

  float AlienType::getRadius() const {
    return cDefRadius;
  }

  float AlienType::getHugMomentum() const {
    return cDefHugMomentum;
  }

  void AlienType::setSpinSpeed(float spinSpeed) {
    cRuntimeSpinSpeed = spinSpeed;
  }

  std::string AlienType::getPhysicalObjectTypeID() const {
    return cEquilibria.getComponentID(this);
  }

  IBinding* AlienType::getPhysicalObjectTypeBinding(const std::string& id) const {
    return nullptr;
  }
  
  std::string AlienType::getPhysicalObjectTypeBindingID(const IBinding* binding) const {
    return "";
  }

  void AlienType::forEachAvailablePhysicalObjectTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    // Nothing to do.
  }

  IWorldEditorToolInstance* AlienType::createToolInstance(WorldEditor& editor, IComponentData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool AlienType::renderAssetIcon() const {
    return cDefModel.renderIcon();
  }

  void AlienType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void AlienType::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool AlienType::isDefaultConfiguration() const {
    return true;
  }

  AlienType::Pen::Pen(AlienType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool AlienType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool AlienType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void AlienType::Pen::renderEditingPreview() const {
    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    glRotatef((cEditor.getCursorX() + cEditor.getCursorY() + 0.25) * cParent.cDefSpinSpeed, 0.0f, 0.0f, 1.0f);
    cParent.cDefModel.renderPreview();
  }

  void AlienType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void AlienType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool AlienType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool AlienType::Pen::inputTool(SignalInputID id, double yaw) {
    if (id == SignalInputID::USE_TOOL) {
      cEditor.getWorld().draw(cParent, cEditor.getCursorCell(), cEditor);
      return true;
    }
    return false;
  }

  bool AlienType::Pen::isCursorLocked() const {
    return false;
  }

  void AlienType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double AlienType::Pen::getSnapInterval() const {
    return 1.0;
  }
}

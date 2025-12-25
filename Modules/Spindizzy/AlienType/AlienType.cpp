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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string AlienType::JSON_ACCELERATION = "acceleration";
  const std::string AlienType::JSON_APPEARANCE   = "appearance";
  const std::string AlienType::JSON_FRICTION     = "friction";
  const std::string AlienType::JSON_HEIGHT       = "height";
  const std::string AlienType::JSON_HUG_MOMENTUM = "hugMomentum";
  const std::string AlienType::JSON_RADIUS       = "radius";
  const std::string AlienType::JSON_SPIN_SPEED   = "spinSpeed";
  const std::string AlienType::JSON_TARGET       = "target";

  const float AlienType::DEFAULT_ACCELERATION = 0.0000215f;
  const float AlienType::DEFAULT_FRICTION     = 0.001f;
  const float AlienType::DEFAULT_HEIGHT       = 1.7f;
  const float AlienType::DEFAULT_HUG_MOMENTUM = 0.0001f;
  const float AlienType::DEFAULT_RADIUS       = 0.4f;
  const float AlienType::DEFAULT_SPIN_SPEED   = 0.0f;

  AlienType::AlienType(Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cAssets(spindizzy),
            cDefModel(data),
            cDefTarget(data),
            cDefAcceleration(DEFAULT_ACCELERATION),
            cDefFriction(DEFAULT_FRICTION),
            cDefSpinSpeed(DEFAULT_SPIN_SPEED),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefHugMomentum(DEFAULT_HUG_MOMENTUM),
            cLuaBinding(data.getProject().getLuaState(), this, [this]() {return renderAssetIcon();}) {
    cSpindizzy.added(this);
  }
  
  AlienType::AlienType(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            AlienType(spindizzy, data) {
    cDefAcceleration = object.getFloat(JSON_ACCELERATION, DEFAULT_ACCELERATION);
    cDefFriction = object.getFloat(JSON_FRICTION, DEFAULT_FRICTION);
    cDefSpinSpeed = object.getFloat(JSON_SPIN_SPEED, DEFAULT_SPIN_SPEED);
    cDefHeight = object.getFloat(JSON_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = object.getFloat(JSON_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = object.getFloat(JSON_HUG_MOMENTUM, DEFAULT_HUG_MOMENTUM);
    cDefTarget.init(object, JSON_TARGET);
    cDefModel.init(object, JSON_APPEARANCE);
  }

  void AlienType::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Spindizzy Aliens");
  }
    
  void AlienType::save(JSONObject object) const {
    cDefModel.save(object, JSON_APPEARANCE);
    cDefTarget.save(object, JSON_TARGET);
    object.addFloat(JSON_ACCELERATION, cDefAcceleration, DEFAULT_ACCELERATION);
    object.addFloat(JSON_FRICTION, cDefFriction, DEFAULT_ACCELERATION);
    object.addFloat(JSON_SPIN_SPEED, cDefSpinSpeed, DEFAULT_SPIN_SPEED);
    object.addFloat(JSON_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    object.addFloat(JSON_RADIUS, cDefRadius, DEFAULT_RADIUS);
    object.addFloat(JSON_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_HUG_MOMENTUM);
  }

  void AlienType::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool AlienType::renderIcon() const {
    glColor3f(1.0f, 1.0f, 1.0f);
    return cDefModel.renderIcon();
  }

  void AlienType::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyAsset<Model>( metadata.getPropertyData("Appearance"),   cDefModel);
    owner.createPropertyAsset<Vertex>(metadata.getPropertyData("Target"),       cDefTarget);
    owner.createPropertyNativeFloat(  metadata.getPropertyData("Acceleration"), [this]() {return cDefAcceleration;}, [this](float value) {cDefAcceleration = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("Friction"),     [this]() {return cDefFriction;},     [this](float value) {cDefFriction     = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("SpinSpeed"),    [this]() {return cDefSpinSpeed;},    [this](float value) {cDefSpinSpeed    = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("Height"),       [this]() {return cDefHeight;},       [this](float value) {cDefHeight       = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("Radius"),       [this]() {return cDefRadius;},       [this](float value) {cDefRadius       = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("HugThreshold"), [this]() {return cDefHugMomentum;},  [this](float value) {cDefHugMomentum  = value;});
  }

  bool AlienType::hasReadOnlyReferences() const {
    return cSpindizzy.isUsedInReadOnlyWorld(*this);
  }

  void AlienType::overrideReadOnlyReferences() {
    cSpindizzy.overrideReadOnlyWorlds(*this);
  }

  void AlienType::reset() {
    cRuntimeSpinSpeed = cDefSpinSpeed;
  }

  AlienType::~AlienType() {
    cAssets.clear();
    cSpindizzy.removeAll(this);
    cSpindizzy.removed(this);
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
    return cSpindizzy.getResourceID(this);
  }

  IBinding* AlienType::getBinding(const std::string& id) const {
    return nullptr;
  }
  
  std::string AlienType::getBindingID(const IBinding* binding) const {
    return "";
  }

  IWorldEditorToolInstance* AlienType::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool AlienType::renderAssetIcon() const {
    return cDefModel.renderIcon();
  }

  void AlienType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void AlienType::getAssetProperties(PropertyMaker& owner) {
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

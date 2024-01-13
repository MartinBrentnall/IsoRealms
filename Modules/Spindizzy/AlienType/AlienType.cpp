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
#include "AlienType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string AlienType::TAG_MODEL  = "Model";
  const std::string AlienType::TAG_TARGET = "Target";

  const std::string AlienType::ATTRIBUTE_ACCELERATION = "acceleration";
  const std::string AlienType::ATTRIBUTE_FRICTION     = "friction";
  const std::string AlienType::ATTRIBUTE_HEIGHT       = "height";
  const std::string AlienType::ATTRIBUTE_HUG_MOMENTUM = "hugMomentum";
  const std::string AlienType::ATTRIBUTE_RADIUS       = "radius";
  const std::string AlienType::ATTRIBUTE_SPIN_SPEED   = "spinSpeed";

  const float AlienType::DEFAULT_ACCELERATION = 0.0000215f;
  const float AlienType::DEFAULT_FRICTION     = 0.001f;
  const float AlienType::DEFAULT_HEIGHT       = 1.7f;
  const float AlienType::DEFAULT_HUG_MOMENTUM = 0.0001f;
  const float AlienType::DEFAULT_RADIUS       = 0.4f;
  const float AlienType::DEFAULT_SPIN_SPEED   = 0.0f;

  AlienType::AlienType(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy),
            cDefModel(project),
            cDefTarget(project),
            cDefAcceleration(DEFAULT_ACCELERATION),
            cDefFriction(DEFAULT_FRICTION),
            cDefSpinSpeed(DEFAULT_SPIN_SPEED),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefHugMomentum(DEFAULT_HUG_MOMENTUM),
            cLuaBinding(project, this) {
    cDefSpindizzy.added(this);

    project->reset([this]() {
      cRuntimeSpinSpeed = cDefSpinSpeed;
    });
  }
  
  AlienType::AlienType(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            AlienType(project, spindizzy) {
    cDefAcceleration = node.getFloatAttribute(ATTRIBUTE_ACCELERATION);
    cDefFriction = node.getFloatAttribute(ATTRIBUTE_FRICTION);
    cDefSpinSpeed = node.getFloatAttribute(ATTRIBUTE_SPIN_SPEED);
    cDefHeight = node.getFloatAttribute(ATTRIBUTE_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = node.getFloatAttribute(ATTRIBUTE_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = node.getFloatAttribute(ATTRIBUTE_ACCELERATION, DEFAULT_HUG_MOMENTUM);
    cDefTarget.init(node.getNode(TAG_TARGET));
    cDefModel.init(node.getNode(TAG_MODEL));
  }

  void AlienType::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, "", "Spindizzy Aliens");
  }
    
  void AlienType::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
  }
  
  void AlienType::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefModel.save(node, TAG_MODEL);
    cDefTarget.save(node, TAG_TARGET);
    node->addAttribute(ATTRIBUTE_ACCELERATION, cDefAcceleration, DEFAULT_ACCELERATION);
    node->addAttribute(ATTRIBUTE_FRICTION, cDefFriction, DEFAULT_FRICTION);
    node->addAttribute(ATTRIBUTE_SPIN_SPEED, cDefSpinSpeed, DEFAULT_SPIN_SPEED);
    node->addAttribute(ATTRIBUTE_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    node->addAttribute(ATTRIBUTE_RADIUS, cDefRadius, DEFAULT_RADIUS);
    node->addAttribute(ATTRIBUTE_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_HUG_MOMENTUM);
  }

  void AlienType::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool AlienType::renderIcon() const {
    glColor3f(1.0f, 1.0f, 1.0f);
    return cDefModel.renderIcon();
  }

  std::vector<IProperty*> AlienType::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  AlienType::~AlienType() {
    cDefSpindizzy.removed(this);
    cDefSpindizzy.removeAll(this);
  }
  
  void AlienType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(   this), "");
    registry->add(static_cast<IPhysicalObjectType*>(this), "");
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
    return cDefSpindizzy.getID(this);
  }

  IBinding* AlienType::getBinding(const std::string& id) const {
    return nullptr;
  }

  IWorldEditorToolInstance* AlienType::createToolInstance(WorldEditor* editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  AlienType::Pen::Pen(AlienType& parent, WorldEditor* editor) :
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
    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    glRotatef((cEditor->getCursorX() + cEditor->getCursorY() + 0.25) * cParent.cDefSpinSpeed, 0.0f, 0.0f, 1.0f);
    cParent.cDefModel.renderIcon();
  }

  void AlienType::Pen::renderUI() const {
    // Nothing to do.
  }

  void AlienType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool AlienType::Pen::inputEdit(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: cEditor->getWorld()->draw(&cParent, cEditor->getCursorCell(), cEditor); return true;
          default:         break;
        }
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

  void AlienType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double AlienType::Pen::getSnapInterval() const {
    return 1.0;
  }
}

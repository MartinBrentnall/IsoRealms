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
#include "PlayerType.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Terrain/Wall.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  PlayerType::PlayerType(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cResources(equilibria),
            cWallBounceActionContext(data, cWallBounceBindings),
            cPlayerActionContext(data, cPlayerBindings),
            cWallBounceBindings(*this),
            cPlayerBindings(*this),
            cDefModel(data),
            cDefInputThrust(data, false),
            cDefInputX(data, 0.0f),
            cDefInputY(data, 0.0f),
            cDefOrientation(data, 0.0f),
            cDefRespawnAction(cPlayerActionContext),
            cDefFallImpactAction(cPlayerActionContext),
            cDefFallBounceAction(cPlayerActionContext),
            cDefWallBounceAction(cWallBounceActionContext),
            cDefLeaveSurfaceAction(cPlayerActionContext),
            cDefApexAction(cPlayerActionContext),
            cLuaBinding(data.getProject().getLuaState(), this, [this]() {return renderResourceIcon();}) {
    cEquilibria.added(this);
  }

  void PlayerType::define(IComponentDefiner& definer) {

    // Dimensions
    definer.propertyFloat(   "radius",         [this]() {return cDefRadius;},       [this](float value) {cDefRadius       = value;}, DEFAULT_RADIUS);
    definer.propertyFloat(   "height",         [this]() {return cDefHeight;},       [this](float value) {cDefHeight       = value;}, DEFAULT_HEIGHT);

    // Physics
    definer.propertyFloat(   "acceleration",   [this]() {return cDefAcceleration;}, [this](float value) {cDefAcceleration = value;}, DEFAULT_ACCELERATION);
    definer.propertyFloat(   "bounceFactor",   [this]() {return cDefBounceFactor;}, [this](float value) {cDefBounceFactor = value;}, DEFAULT_BOUNCE_FACTOR);
    definer.propertyFloat(   "stepReach",      [this]() {return cDefStepReach;},    [this](float value) {cDefStepReach    = value;}, DEFAULT_STEP_REACH);
    definer.propertyFloat(   "hugMomentum",    [this]() {return cDefHugMomentum;},  [this](float value) {cDefHugMomentum  = value;}, DEFAULT_HUG_MOMENTUM);

    // appearance
    definer.propertyResource("appearance",     cDefModel);
    definer.propertyFloat(   "spinSpeed",      [this]() {return cDefSpinSpeed;},    [this](float value) {cDefSpinSpeed    = value;});

    // Input
    definer.propertyResource("xInput",         cDefInputX);
    definer.propertyResource("yInput",         cDefInputY);
    definer.propertyResource("thrustInput",    cDefInputThrust);

    // Actions
    definer.propertyResource("onRespawn",      cDefRespawnAction);
    definer.propertyResource("onFallImpact",   cDefFallImpactAction);
    definer.propertyResource("onFallBounce",   cDefFallBounceAction);
    definer.propertyResource("onWallBounce",   cDefWallBounceAction);
    definer.propertyResource("onLeaveSurface", cDefLeaveSurfaceAction);
    definer.propertyResource("onApex",         cDefApexAction);

    // Misc
    definer.propertyInteger( "respawnDelay",   [this]() {return cDefRespawnDelay;}, [this](int   value) {cDefRespawnDelay = value;}, DEFAULT_RESPAWN_DELAY);
    definer.propertyResource("orientation",    cDefOrientation);
  }

  void PlayerType::publish(ResourcePublisher& publisher) {
    publisher.publish<IBinding>(&cLuaBinding, "", "Equilibria/Players");
  }

  bool PlayerType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  void PlayerType::removed() {
    cEquilibria.removed(this);
    cEquilibria.removeAll(this);
  }

  void PlayerType::reset() {
    cRuntimeSpinSpeed = cDefSpinSpeed;
  }
  
  void PlayerType::publish(const std::string& parentID) {
    cResources.publish<IWorldEditorTool>(   this, parentID, "Player Types");
    cResources.publish<IPhysicalObjectType>(this, parentID, "Player Types");
  }  
  
  std::unique_ptr<ModelInstance> PlayerType::createModel() {
    return cDefModel.createInstance();
  }

  void PlayerType::impactSurface(Player* player) {
    cEquilibria.bind(player);
    cDefFallImpactAction.execute();
  }

  void PlayerType::bounceWall(Player* player, Zone* zone) {
    cEquilibria.bind(player);
    cEquilibria.bind1(zone);
    cDefWallBounceAction.execute();
  }

  void PlayerType::bounceSurface() {
    cDefFallBounceAction.execute();
  }

  void PlayerType::leaveSurface(Player* player, Zone* zone) {
    cEquilibria.bind(player);
    cEquilibria.bind1(zone);
    cDefLeaveSurfaceAction.execute();
  }

  void PlayerType::objectApex(Player* player) {
    cEquilibria.bind(player);
    cDefApexAction.execute();
  }

  void PlayerType::respawn() {
    cDefRespawnAction.execute();
  }

  float PlayerType::getXThrust() const {
    return getMovingX() * cDefAcceleration * (cDefInputThrust->getValue() ? 2.0f : 1.0f);
  }

  float PlayerType::getYThrust() const {
    return getMovingY() * cDefAcceleration * (cDefInputThrust->getValue() ? 2.0f : 1.0f);
  }

  float PlayerType::getSpinSpeed() const {
    return cRuntimeSpinSpeed;
  }

  float PlayerType::getStepReach() const {
    return cDefStepReach;
  }

  float PlayerType::getHeight() const {
    return cDefHeight;
  }

  float PlayerType::getRadius() const {
    return cDefRadius;
  }

  float PlayerType::getHugMomentum() const {
    return cDefHugMomentum;
  }

  float PlayerType::getBounceFactor() const {
    return cDefBounceFactor;
  }

  int PlayerType::getRespawnDelay() const {
    return cDefRespawnDelay;
  }

  float PlayerType::getMovingX() const {
    float mCameraAngle = cDefOrientation->getValue();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? -cDefInputY->getValue()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ?  cDefInputX->getValue()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ?  cDefInputY->getValue()
         :                                                     -cDefInputX->getValue();
  }

  float PlayerType::getMovingY() const {
    float mCameraAngle = cDefOrientation->getValue();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? -cDefInputX->getValue()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? -cDefInputY->getValue()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ?  cDefInputX->getValue()
         :                                                      cDefInputY->getValue();
  }

  void PlayerType::setSpinSpeed(float spinSpeed) {
    cRuntimeSpinSpeed = spinSpeed;
  }

  std::string PlayerType::getPhysicalObjectTypeID() const {
    return cEquilibria.getComponentID(this);
  }
  
  std::string PlayerType::getPhysicalObjectTypeBindingID(const IBinding* binding) const {
    return cEquilibria.getBindingIDPlayer(binding);
  }

  void PlayerType::forEachAvailablePhysicalObjectTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cEquilibria.getTreeItemsPlayer(getTreeItemInfoFunction);
  }

  IWorldEditorToolInstance* PlayerType::createToolInstance(WorldEditor& editor, IComponentData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PlayerType::renderResourceIcon() const {
    return renderIcon();
  }

  IBinding* PlayerType::getPhysicalObjectTypeBinding(const std::string& id) const {
    return cEquilibria.getBindingPlayer(id);
  }

  // void PlayerType::releaseBinding(const IBinding* asset) {
  //   // Nothing to do.
  // }

  PlayerType::Pen::Pen(PlayerType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool PlayerType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool PlayerType::Pen::renderIcon(float yaw) const {
    return cParent.cDefModel.renderIcon();
  }

  void PlayerType::Pen::renderEditingPreview() const {
    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    glRotatef((cEditor.getCursorX() + cEditor.getCursorY() + 0.25) * cParent.cDefSpinSpeed, 0.0f, 0.0f, 1.0f);
    cParent.cDefModel.renderPreview();
  }

  void PlayerType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void PlayerType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool PlayerType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool PlayerType::Pen::inputTool(SignalInputID id, double yaw) {
    if (id == SignalInputID::USE_TOOL) {
      cEditor.getWorld().draw(cParent, cEditor.getCursorLocation());
      return true;
    }
    return false;
  }

  bool PlayerType::Pen::isCursorLocked() const {
    return false;
  }

  void PlayerType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double PlayerType::Pen::getSnapInterval() const {
    return 0.5;
  }

  PlayerType::WallBounceBindings::WallBounceBindings(PlayerType& parent) :
            cParent(parent) {
  }

  IBinding* PlayerType::WallBounceBindings::getBinding(const std::string& id) {
    return cParent.cEquilibria.getBindingWallBounce(id);
  }

  std::string PlayerType::WallBounceBindings::getBindingID(const IBinding* binding) const {
    return cParent.cEquilibria.getBindingIDWallBounce(binding);
  }

  void PlayerType::WallBounceBindings::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cParent.cEquilibria.getTreeItemsWallBounce(getTreeItemInfoFunction);
  }

  void PlayerType::WallBounceBindings::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  PlayerType::PlayerBindings::PlayerBindings(PlayerType& parent) :
            cParent(parent) {
  }

  IBinding* PlayerType::PlayerBindings::getBinding(const std::string& id) {
    return cParent.cEquilibria.getBindingPlayer(id);
  }
  
  std::string PlayerType::PlayerBindings::getBindingID(const IBinding* binding) const {
    return cParent.cEquilibria.getBindingIDPlayer(binding);
  }

  void PlayerType::PlayerBindings::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    cParent.cEquilibria.getTreeItemsPlayer(getTreeItemInfoFunction);
  }

  void PlayerType::PlayerBindings::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }
}

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
#include "PlayerType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string PlayerType::TAG_FALL_BOUNCE_ACTION = "FallBounceAction";
  const std::string PlayerType::TAG_FALL_IMPACT_ACTION = "FallImpactAction";
  const std::string PlayerType::TAG_INPUT_THRUST       = "InputThrust";
  const std::string PlayerType::TAG_INPUT_X            = "InputX";
  const std::string PlayerType::TAG_INPUT_Y            = "InputY";
  const std::string PlayerType::TAG_MODEL              = "Model";
  const std::string PlayerType::TAG_ORIENTATION        = "Orientation";
  const std::string PlayerType::TAG_RESPAWN_ACTION     = "RespawnAction";
  const std::string PlayerType::TAG_WALL_BOUNCE_ACTION = "WallBounceAction";

  const std::string PlayerType::ATTRIBUTE_ACCELERATION  = "acceleration";
  const std::string PlayerType::ATTRIBUTE_BOUNCE_FACTOR = "bounceFactor";
  const std::string PlayerType::ATTRIBUTE_HEIGHT        = "height";
  const std::string PlayerType::ATTRIBUTE_HUG_MOMENTUM  = "hugMomentum";
  const std::string PlayerType::ATTRIBUTE_RADIUS        = "radius";
  const std::string PlayerType::ATTRIBUTE_RESPAWN_DELAY = "respawnDelay";
  const std::string PlayerType::ATTRIBUTE_SPIN_SPEED    = "spinSpeed";
  const std::string PlayerType::ATTRIBUTE_STEP_REACH    = "stepReach";

  const float PlayerType::DEFAULT_ACCELERATION  = 0.0000265f;
  const float PlayerType::DEFAULT_BOUNCE_FACTOR = 1.0f;
  const float PlayerType::DEFAULT_HEIGHT        = 1.7f;
  const float PlayerType::DEFAULT_HUG_MOMENTUM  = 0.0001f;
  const float PlayerType::DEFAULT_RADIUS        = 0.4f;
  const int   PlayerType::DEFAULT_RESPAWN_DELAY = 500;
  const float PlayerType::DEFAULT_SPIN_SPEED    = 0.0f;
  const float PlayerType::DEFAULT_STEP_REACH    = 0.5f;

  const std::string PlayerType::BIND_TO_PLAYER  = "Player";
  const std::string PlayerType::BIND_TO_TERRAIN = "Terrain";

  PlayerType::PlayerType(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy),
            cDefAcceleration(DEFAULT_ACCELERATION),
            cDefSpinSpeed(DEFAULT_SPIN_SPEED),
            cDefBounceFactor(DEFAULT_BOUNCE_FACTOR),
            cDefStepReach(DEFAULT_STEP_REACH),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefHugMomentum(DEFAULT_HUG_MOMENTUM),
            cDefRespawnDelay(DEFAULT_RESPAWN_DELAY),
            cDefModel(project),
            cDefInputThrust(project, false),
            cDefInputX(project, 0.0f),
            cDefInputY(project, 0.0f),
            cDefOrientation(project, 0.0f),
            cDefRespawnAction(project),
            cDefFallImpactAction(project),
            cDefFallBounceAction(project),
            cDefWallBounceAction(project),
            cLuaBinding(project, this) {
    cDefSpindizzy.added(this);
    project->reset([this]() {
      cRuntimeSpinSpeed = cDefSpinSpeed;
    });
  }

  PlayerType::PlayerType(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            PlayerType(project, spindizzy) {
    cDefSpinSpeed = node.getFloatAttribute(ATTRIBUTE_SPIN_SPEED, DEFAULT_SPIN_SPEED);
    cDefBounceFactor = node.getFloatAttribute(ATTRIBUTE_BOUNCE_FACTOR, DEFAULT_ACCELERATION);
    cDefAcceleration = node.getFloatAttribute(ATTRIBUTE_ACCELERATION, DEFAULT_ACCELERATION);
    cDefStepReach = node.getFloatAttribute(ATTRIBUTE_STEP_REACH, DEFAULT_STEP_REACH);
    cDefHeight = node.getFloatAttribute(ATTRIBUTE_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = node.getFloatAttribute(ATTRIBUTE_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = node.getFloatAttribute(ATTRIBUTE_HUG_MOMENTUM, DEFAULT_HUG_MOMENTUM);
    cDefRespawnDelay = node.getIntegerAttribute(ATTRIBUTE_RESPAWN_DELAY, DEFAULT_RESPAWN_DELAY);
    cDefModel.init(node.getNode(TAG_MODEL));
    cDefInputThrust.init(node.getNode(TAG_INPUT_THRUST));
    cDefInputX.init(node.getNode(TAG_INPUT_X));
    cDefInputY.init(node.getNode(TAG_INPUT_Y));
    cDefOrientation.init(node.getNode(TAG_ORIENTATION));
    cDefFallImpactAction.init(node, TAG_FALL_IMPACT_ACTION, &cDefSpindizzy);
    cDefFallBounceAction.init(node, TAG_FALL_BOUNCE_ACTION, &cDefSpindizzy);
    cDefWallBounceAction.init(node, TAG_WALL_BOUNCE_ACTION, this);
    cDefRespawnAction.init(node, TAG_RESPAWN_ACTION, &cDefSpindizzy);
  }

  void PlayerType::registerAssets(IAssetRegistry* assets) {
    assets->add(&cLuaBinding, "", "Spindizzy Players");
  }

  void PlayerType::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(&cLuaBinding);
  }

  void PlayerType::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_SPIN_SPEED, cDefSpinSpeed, DEFAULT_SPIN_SPEED);
    node->addAttribute(ATTRIBUTE_ACCELERATION, cDefAcceleration, DEFAULT_ACCELERATION);
    node->addAttribute(ATTRIBUTE_BOUNCE_FACTOR, cDefBounceFactor, DEFAULT_BOUNCE_FACTOR);
    node->addAttribute(ATTRIBUTE_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    node->addAttribute(ATTRIBUTE_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    node->addAttribute(ATTRIBUTE_RADIUS, cDefRadius, DEFAULT_RADIUS);
    node->addAttribute(ATTRIBUTE_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_ACCELERATION);
    node->addAttribute(ATTRIBUTE_RESPAWN_DELAY, cDefRespawnDelay, DEFAULT_RESPAWN_DELAY);
    cDefModel.save(node, TAG_MODEL);
    cDefRespawnAction.save(node, TAG_RESPAWN_ACTION);
    cDefInputThrust.save(node, TAG_INPUT_THRUST);
    cDefInputX.save(node, TAG_INPUT_X);
    cDefInputY.save(node, TAG_INPUT_Y);
    cDefFallImpactAction.save(node, TAG_FALL_IMPACT_ACTION);
    cDefFallBounceAction.save(node, TAG_FALL_BOUNCE_ACTION);
    cDefWallBounceAction.save(node, TAG_WALL_BOUNCE_ACTION);
    cDefOrientation.save(node, TAG_ORIENTATION);
  }

  void PlayerType::hintInUse(bool inUse) {
  }

  bool PlayerType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  std::vector<IProperty*> PlayerType::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  PlayerType::~PlayerType() {
    cDefSpindizzy.removed(this);
    cDefSpindizzy.removeAll(this);
  }
  
  void PlayerType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(   this), "");
    registry->add(static_cast<IPhysicalObjectType*>(this), "");
  }  
  
  std::unique_ptr<ModelInstance> PlayerType::createModel() {
    return cDefModel.createInstance();
  }

  void PlayerType::impactSurface(Player* player) {
    cDefSpindizzy.bind(player);
    cDefFallImpactAction.execute();
  }

  void PlayerType::bounceWall(Player* player, Zone* zone) {
    cDefSpindizzy.bind(player);
    cDefSpindizzy.bind(zone);
    cDefWallBounceAction.execute();
  }

  void PlayerType::bounceSurface() {
    cDefFallBounceAction.execute();
  }

  void PlayerType::respawn(LiteralVertex& launchMomentum) {
    cDefSpindizzy.bindLaunchMomentum(&launchMomentum);
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
    return cDefSpindizzy.getID(this);
  }
  
  IBinding* PlayerType::getBinding(const std::string& id) const {
    return nullptr; // TODO: Implement this.
  }
  
  void PlayerType::save(DOMNodeWriter* node, const IBinding* binding) const {
    // TODO: Implement this.
  }

  std::string PlayerType::getBindingID(const IBinding* binding) const {
    return ""; // TODO: Implement this.
  }

  IWorldEditorToolInstance* PlayerType::createToolInstance(WorldEditor* editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PlayerType::renderAssetIcon() const {
    return false;
  }

  IBinding* PlayerType::getBinding(const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    return mBindTo == BIND_TO_TERRAIN ? cDefSpindizzy.getZoneBinding2(mBindPath)
         : mBindTo == BIND_TO_PLAYER  ? cDefSpindizzy.getZoneBinding(mBindPath)
         :                              nullptr;
  }

  void PlayerType::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

  PlayerType::Pen::Pen(PlayerType& parent, WorldEditor* editor) :
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
    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    glRotatef((cEditor->getCursorX() + cEditor->getCursorY() + 0.25) * cParent.cDefSpinSpeed, 0.0f, 0.0f, 1.0f);
    cParent.cDefModel.renderPreview();
  }

  void PlayerType::Pen::renderUI() const {
    // Nothing to do.
  }

  void PlayerType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool PlayerType::Pen::inputEdit(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: cEditor->getWorld()->draw(&cParent, cEditor->getCursorLocation()); return true;
          default:         break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: cEditor->getWorld()->draw(&cParent, cEditor->getCursorLocation()); return true;
        }
        break;
      }

      default: break;
    }
    return false;
  }

  void PlayerType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    // Nothing to do.
  }

  double PlayerType::Pen::getSnapInterval() const {
    return 0.5;
  }
}

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

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Terrain/Wall.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string PlayerType::JSON_ACCELERATION   = "acceleration";
  const std::string PlayerType::JSON_APPEARANCE     = "appearance";
  const std::string PlayerType::JSON_BOUNCE_FACTOR  = "bounceFactor";
  const std::string PlayerType::JSON_HEIGHT         = "height";
  const std::string PlayerType::JSON_HUG_MOMENTUM   = "hugMomentum";
  const std::string PlayerType::JSON_ON_FALL_BOUNCE = "onFallBounce";
  const std::string PlayerType::JSON_ON_FALL_IMPACT = "onFallImpact";
  const std::string PlayerType::JSON_ON_WALL_BOUNCE = "onWallBounce";
  const std::string PlayerType::JSON_ON_RESPAWN     = "onRespawn";
  const std::string PlayerType::JSON_ORIENTATION    = "orientation";
  const std::string PlayerType::JSON_RADIUS         = "radius";
  const std::string PlayerType::JSON_RESPAWN_DELAY  = "respawnDelay";
  const std::string PlayerType::JSON_SPIN_SPEED     = "spinSpeed";
  const std::string PlayerType::JSON_STEP_REACH     = "stepReach";
  const std::string PlayerType::JSON_THRUST_INPUT   = "thrustInput";
  const std::string PlayerType::JSON_X_INPUT        = "xInput";
  const std::string PlayerType::JSON_Y_INPUT        = "yInput";


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

  PlayerType::PlayerType(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cActionClient(data, *this),
            cDefAcceleration(DEFAULT_ACCELERATION),
            cDefSpinSpeed(DEFAULT_SPIN_SPEED),
            cDefBounceFactor(DEFAULT_BOUNCE_FACTOR),
            cDefStepReach(DEFAULT_STEP_REACH),
            cDefHeight(DEFAULT_HEIGHT),
            cDefRadius(DEFAULT_RADIUS),
            cDefHugMomentum(DEFAULT_HUG_MOMENTUM),
            cDefRespawnDelay(DEFAULT_RESPAWN_DELAY),
            cDefModel(data),
            cDefInputThrust(data, false),
            cDefInputX(data, 0.0f),
            cDefInputY(data, 0.0f),
            cDefOrientation(data, 0.0f),
            cDefRespawnAction(cActionClient),
            cDefFallImpactAction(cActionClient),
            cDefFallBounceAction(cActionClient),
            cDefWallBounceAction(cActionClient),
            cLuaBinding(project, this, [this]() {return renderAssetIcon();}) {
    cSpindizzy.added(this);
  }

  PlayerType::PlayerType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            PlayerType(project, spindizzy, data) {
    cDefSpinSpeed = object.getFloat(JSON_SPIN_SPEED, DEFAULT_SPIN_SPEED);
    cDefBounceFactor = object.getFloat(JSON_BOUNCE_FACTOR, DEFAULT_ACCELERATION);
    cDefAcceleration = object.getFloat(JSON_ACCELERATION, DEFAULT_ACCELERATION);
    cDefStepReach = object.getFloat(JSON_STEP_REACH, DEFAULT_STEP_REACH);
    cDefHeight = object.getFloat(JSON_HEIGHT, DEFAULT_HEIGHT);
    cDefRadius = object.getFloat(JSON_RADIUS, DEFAULT_RADIUS);
    cDefHugMomentum = object.getFloat(JSON_HUG_MOMENTUM, DEFAULT_HUG_MOMENTUM);
    cDefRespawnDelay = object.getInteger(JSON_RESPAWN_DELAY, DEFAULT_RESPAWN_DELAY);
    cDefModel.init(object, JSON_APPEARANCE);
    cDefInputThrust.init(object, JSON_THRUST_INPUT);
    cDefInputX.init(object, JSON_X_INPUT);
    cDefInputY.init(object, JSON_Y_INPUT);
    cDefOrientation.init(object, JSON_ORIENTATION);
    cDefFallImpactAction.init(object, JSON_ON_FALL_IMPACT);
    cDefFallBounceAction.init(object, JSON_ON_FALL_BOUNCE);
    cDefWallBounceAction.init(object, JSON_ON_WALL_BOUNCE);
    cDefRespawnAction.init(object, JSON_ON_RESPAWN);
  }

  void PlayerType::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IBinding>(&cLuaBinding, "", "Spindizzy Players");
  }

  void PlayerType::save(JSONObject object) const {
    object.addFloat(JSON_SPIN_SPEED, cDefSpinSpeed, DEFAULT_SPIN_SPEED);
    object.addFloat(JSON_ACCELERATION, cDefAcceleration, DEFAULT_ACCELERATION);
    object.addFloat(JSON_BOUNCE_FACTOR, cDefBounceFactor, DEFAULT_BOUNCE_FACTOR);
    object.addFloat(JSON_STEP_REACH, cDefStepReach, DEFAULT_STEP_REACH);
    object.addFloat(JSON_HEIGHT, cDefHeight, DEFAULT_HEIGHT);
    object.addFloat(JSON_RADIUS, cDefRadius, DEFAULT_RADIUS);
    object.addFloat(JSON_HUG_MOMENTUM, cDefHugMomentum, DEFAULT_HUG_MOMENTUM);
    object.addInteger(JSON_RESPAWN_DELAY, cDefRespawnDelay, DEFAULT_RESPAWN_DELAY);
    cDefModel.save(object, JSON_APPEARANCE);
    cDefRespawnAction.save(object, JSON_ON_RESPAWN);
    cDefInputThrust.save(object, JSON_THRUST_INPUT);
    cDefInputX.save(object, JSON_X_INPUT);
    cDefInputY.save(object, JSON_Y_INPUT);
    cDefFallImpactAction.save(object, JSON_ON_FALL_IMPACT);
    cDefFallBounceAction.save(object, JSON_ON_FALL_BOUNCE);
    cSpindizzy.setBindingIdentifier(this);
    cDefWallBounceAction.save(object, JSON_ON_WALL_BOUNCE);
    cSpindizzy.setBindingIdentifier(nullptr);
    cDefOrientation.save(object, JSON_ORIENTATION);
  }

  void PlayerType::hintInUse(bool inUse) {
  }

  bool PlayerType::renderIcon() const {
    return cDefModel.renderIcon();
  }

  void PlayerType::getProperties(PropertyMaker& owner, const Metadata& metadata) {

    // Dimensions
    owner.createPropertyNativeFloat(   metadata.getPropertyData("Radius"),               [this]() {return cDefRadius;},       [this](float value) {cDefRadius       = value;});
    owner.createPropertyNativeFloat(   metadata.getPropertyData("Height"),               [this]() {return cDefHeight;},       [this](float value) {cDefHeight       = value;});

    // Physics
    owner.createPropertyNativeFloat(   metadata.getPropertyData("Acceleration"),         [this]() {return cDefAcceleration;}, [this](float value) {cDefAcceleration = value;});
    owner.createPropertyNativeFloat(   metadata.getPropertyData("WallBounce"),           [this]() {return cDefBounceFactor;}, [this](float value) {cDefBounceFactor = value;});
    owner.createPropertyNativeFloat(   metadata.getPropertyData("StepReach"),            [this]() {return cDefStepReach;},    [this](float value) {cDefStepReach    = value;});
    owner.createPropertyNativeFloat(   metadata.getPropertyData("HugMomentumThreshold"), [this]() {return cDefHugMomentum;},  [this](float value) {cDefHugMomentum  = value;});

    // appearance
    owner.createPropertyAsset<Model>(  metadata.getPropertyData("Appearance"),           cDefModel);
    owner.createPropertyNativeFloat(   metadata.getPropertyData("SpinSpeed"),            [this]() {return cDefSpinSpeed;},    [this](float value) {cDefSpinSpeed    = value;});

    // Input
    owner.createPropertyAsset<Float>(  metadata.getPropertyData("InputX"),               cDefInputX);
    owner.createPropertyAsset<Float>(  metadata.getPropertyData("InputY"),               cDefInputY);
    owner.createPropertyAsset<Boolean>(metadata.getPropertyData("InputThrust"),          cDefInputThrust);

    // Actions
    owner.createPropertyAsset<Action>( metadata.getPropertyData("OnRespawn"),            cDefRespawnAction);
    owner.createPropertyAsset<Action>( metadata.getPropertyData("OnImpact"),             cDefFallImpactAction);
    owner.createPropertyAsset<Action>( metadata.getPropertyData("OnBounce"),             cDefFallBounceAction);
    owner.createPropertyAsset<Action>( metadata.getPropertyData("OnWallBounce"),         cDefWallBounceAction);

    // Misc
    owner.createPropertyNativeInteger( metadata.getPropertyData("RespawnDelay"),         [this]() {return cDefRespawnDelay;}, [this](int   value) {cDefRespawnDelay = value;});
    owner.createPropertyAsset<Float>(  metadata.getPropertyData("ViewOrientation"),      cDefOrientation);
  }

  void PlayerType::reset() {
    cRuntimeSpinSpeed = cDefSpinSpeed;
  }
  
  PlayerType::~PlayerType() {
    cSpindizzy.removed(this);
    cSpindizzy.removeAll(this);
  }
  
  void PlayerType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(static_cast<IWorldEditorTool*>(   this), "");
    registry->add(static_cast<IPhysicalObjectType*>(this), "");
  }  
  
  std::unique_ptr<ModelInstance> PlayerType::createModel() {
    return cDefModel.createInstance();
  }

  void PlayerType::impactSurface(Player* player) {
    cSpindizzy.bind(player);
    cDefFallImpactAction.execute();
  }

  void PlayerType::bounceWall(Player* player, Zone* zone) {
    cSpindizzy.bind(player);
    cSpindizzy.bind(zone);
    cDefWallBounceAction.execute();
  }

  void PlayerType::bounceSurface() {
    cDefFallBounceAction.execute();
  }

  void PlayerType::respawn(LiteralVertex& launchMomentum) {
    cSpindizzy.bindLaunchMomentum(&launchMomentum);
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
    return cSpindizzy.getID(this);
  }
  
  IBinding* PlayerType::getBinding(const std::string& id) const {
    return nullptr; // TODO: Implement this.
  }
  
  void PlayerType::saveBinding(JSONObject object, const IBinding* binding) const {
    // TODO: Implement this.
  }

  std::string PlayerType::getBindingID(const IBinding* binding) const {
    std::string mBindingID = cSpindizzy.getZoneBindingID2(binding);
    if (mBindingID != "") {
      return BIND_TO_TERRAIN + "/" + mBindingID;
    }
    return BIND_TO_PLAYER + "/" + cSpindizzy.getZoneBindingID1(binding);
  }

  IWorldEditorToolInstance* PlayerType::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool PlayerType::renderAssetIcon() const {
    return renderIcon();
  }

  void PlayerType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void PlayerType::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool PlayerType::isDefaultConfiguration() const {
    return true;
  }

  IBinding* PlayerType::getBinding(const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    return mBindTo == BIND_TO_TERRAIN ? cSpindizzy.getZoneBinding2(mBindPath)
         : mBindTo == BIND_TO_PLAYER  ? cSpindizzy.getZoneBinding(mBindPath)
         :                              nullptr;
  }

  void PlayerType::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }

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
}

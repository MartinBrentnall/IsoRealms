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
#include "TerrainType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/ZoneType/ZoneType.h"

namespace IsoRealms::Spindizzy {
  const std::string TerrainType::JSON_ALLOW_RESPAWN = "allowRespawn";
  const std::string TerrainType::JSON_EAST_WALL     = "eastWall";
  const std::string TerrainType::JSON_FLOOR_BOUNCE  = "floorBounce";
  const std::string TerrainType::JSON_FRICTION      = "friction";
  const std::string TerrainType::JSON_GRIP          = "grip";
  const std::string TerrainType::JSON_NORTH_WALL    = "northWall";
  const std::string TerrainType::JSON_ON_IMPACT     = "onImpact";
  const std::string TerrainType::JSON_ON_TOUCH      = "onTouch";
  const std::string TerrainType::JSON_SOLID         = "solid";
  const std::string TerrainType::JSON_SOUTH_WALL    = "southWall";
  const std::string TerrainType::JSON_SURFACE       = "surface";
  const std::string TerrainType::JSON_WALL_BOUNCE   = "wallBounce";
  const std::string TerrainType::JSON_WEST_WALL     = "westWall";

  const float TerrainType::DEFAULT_WALL_BOUNCE = 0.6f;

  TerrainType::TerrainType(Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cResourceData(data),
            cAssets(spindizzy),
            cDefSurfaceFriction(0.0f),
            cDefSurfaceGrip(0.0f),
            cDefSurfaceBounce(0.0f),
            cDefWallBounce(DEFAULT_WALL_BOUNCE),
            cDefRespawnAllowed(true),
            cDefSolid(false),
            cDefSurfacePattern(  spindizzy, *this, [&spindizzy]() {spindizzy.stateChanged(nullptr);}),
            cDefWestWallPattern( spindizzy, *this, [&spindizzy]() {spindizzy.stateChanged(nullptr);}),
            cDefEastWallPattern( spindizzy, *this, [&spindizzy]() {spindizzy.stateChanged(nullptr);}),
            cDefSouthWallPattern(spindizzy, *this, [&spindizzy]() {spindizzy.stateChanged(nullptr);}),
            cDefNorthWallPattern(spindizzy, *this, [&spindizzy]() {spindizzy.stateChanged(nullptr);}),
            cDefContactAction(data.getDummyActionClient()),
            cDefImpactAction(data.getDummyActionClient()) {
  }
  
  TerrainType::TerrainType(Spindizzy& spindizzy, IResourceData& data, JSONObject object) :
            TerrainType(spindizzy, data) {
    cDefSolid = object.getBoolean(JSON_SOLID);
    cDefSurfaceBounce = object.getFloat(JSON_FLOOR_BOUNCE);
    cDefWallBounce = object.getFloat(JSON_WALL_BOUNCE, DEFAULT_WALL_BOUNCE);
    cDefRespawnAllowed = object.getBoolean(JSON_ALLOW_RESPAWN);
    cDefSurfaceGrip = object.getFloat(JSON_GRIP);
    cDefSurfaceFriction = object.getFloat(JSON_FRICTION);
    cDefContactAction.init(object, JSON_ON_TOUCH);
    cDefImpactAction.init(object, JSON_ON_IMPACT);
    cDefWestWallPattern.init(object, JSON_WEST_WALL);
    cDefEastWallPattern.init(object, JSON_EAST_WALL);
    cDefSouthWallPattern.init(object, JSON_SOUTH_WALL);
    cDefNorthWallPattern.init(object, JSON_NORTH_WALL);
    cDefSurfacePattern.init(object, JSON_SURFACE);
  }

  void TerrainType::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void TerrainType::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeFloat(  metadata.getPropertyData("SurfaceFriction"),     [this]() {return cDefSurfaceFriction;}, [this](float value) {cDefSurfaceFriction = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("SurfaceGrip"),         [this]() {return cDefSurfaceGrip;},     [this](float value) {cDefSurfaceGrip     = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("SurfaceBounce"),       [this]() {return cDefSurfaceBounce;},   [this](float value) {cDefSurfaceBounce   = value;});
    owner.createPropertyNativeFloat(  metadata.getPropertyData("WallBounce"),          [this]() {return cDefWallBounce;},      [this](float value) {cDefWallBounce      = value;});
    owner.createPropertyNativeBoolean(metadata.getPropertyData("AllowRespawn"),        [this]() {return cDefRespawnAllowed;},  [this](bool  value) {cDefRespawnAllowed  = value;});
    owner.createPropertyNativeBoolean(metadata.getPropertyData("Solid"),               [this]() {return cDefSolid;},           [this](bool  value) {cDefSolid           = value;});
    owner.createPropertyTreeSelector( metadata.getPropertyData("OnTouch"),             cDefContactAction);
    owner.createPropertyTreeSelector( metadata.getPropertyData("OnImpact"),            cDefImpactAction);
    owner.createPropertyTreeSelector( metadata.getPropertyData("AppearanceSurface"),   cDefSurfacePattern);
    owner.createPropertyTreeSelector( metadata.getPropertyData("AppearanceWallNorth"), cDefNorthWallPattern);
    owner.createPropertyTreeSelector( metadata.getPropertyData("AppearanceWallSouth"), cDefSouthWallPattern);
    owner.createPropertyTreeSelector( metadata.getPropertyData("AppearanceWallWest"),  cDefWestWallPattern);
    owner.createPropertyTreeSelector( metadata.getPropertyData("AppearanceWallEast"),  cDefEastWallPattern);
  }

  void TerrainType::removed() {
    cSpindizzy.removeAll(this);
  }

  bool TerrainType::hasReadOnlyReferences() const {
    return cSpindizzy.isUsedInReadOnlyWorld(*this);
  }

  void TerrainType::overrideReadOnlyReferences() {
    cSpindizzy.overrideReadOnlyWorlds(*this);
  }

  ISurfacePattern* TerrainType::getSurfacePattern() const {
    return *cDefSurfacePattern;
  }

  IWallPattern* TerrainType::getWestWallPattern() const {
    return *cDefWestWallPattern;
  }

  IWallPattern* TerrainType::getEastWallPattern() const {
    return *cDefEastWallPattern;
  }

  IWallPattern* TerrainType::getSouthWallPattern() const {
    return *cDefSouthWallPattern;
  }

  IWallPattern* TerrainType::getNorthWallPattern() const {
    return *cDefNorthWallPattern;
  }

  Project& TerrainType::getProject() {
    return cSpindizzy.getProject();
  }

  Spindizzy& TerrainType::getSpindizzy() const {
    return cSpindizzy;
  }

  Spindizzy& TerrainType::getAssetManager() {
    return cSpindizzy;
  }

  IResourceData& TerrainType::getResourceData() {
    return cResourceData;
  }

  void TerrainType::executeContactScript() {
    cDefContactAction.execute();
  }

  void TerrainType::executeImpactScript() {
    cDefImpactAction.execute();
  }

  float TerrainType::getSurfaceFriction() const {
    return cDefSurfaceFriction;
  }

  float TerrainType::getSurfaceGrip() const {
    return cDefSurfaceGrip;
  }

  float TerrainType::getSurfaceBounce() const {
    return cDefSurfaceBounce;
  }

  float TerrainType::getWallBounce() const {
    return cDefWallBounce;
  }

  bool TerrainType::isRespawnAllowed() const {
    return cDefRespawnAllowed;
  }

  bool TerrainType::isSolid() const {
    return cDefSolid;
  }

  void TerrainType::save(JSONObject object) const {
    object.addBoolean(JSON_SOLID, cDefSolid);
    object.addFloat(JSON_FRICTION, cDefSurfaceFriction);
    object.addFloat(JSON_GRIP, cDefSurfaceGrip);
    object.addFloat(JSON_FLOOR_BOUNCE, cDefSurfaceBounce);
    object.addFloat(JSON_WALL_BOUNCE, cDefWallBounce, DEFAULT_WALL_BOUNCE);
    object.addBoolean(JSON_ALLOW_RESPAWN, cDefRespawnAllowed);
    cDefImpactAction.save(object, JSON_ON_IMPACT);
    cDefContactAction.save(object, JSON_ON_TOUCH);
    cDefSurfacePattern.save(object, JSON_SURFACE);
    cDefWestWallPattern.save(object, JSON_EAST_WALL);
    cDefEastWallPattern.save(object, JSON_WEST_WALL);
    cDefSouthWallPattern.save(object, JSON_SOUTH_WALL);
    cDefNorthWallPattern.save(object, JSON_NORTH_WALL);
  }

  std::vector<ConditionElement*> TerrainType::getTerrainStateConditionElements() {
    return cSpindizzy.getTerrainStateConditionElements();
  }








  IWorldEditorToolInstance* TerrainType::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool TerrainType::renderAssetIcon() const {
    return renderIcon();
  }

  void TerrainType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void TerrainType::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool TerrainType::isDefaultConfiguration() const {
    return true;
  }

  TerrainType::Pen::Pen(TerrainType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor),
            cPinnedZone(nullptr),
            cDrawingNegation(false) {
  }

  void TerrainType::Pen::draw() {
    if (!cEditor.getTerrainBrush().isEditing()) {
      if (cPinnedZone == nullptr) {
        cPinnedZone = cEditor.getWorld().getOrDrawZone(cEditor.getCursorCell(), cEditor, nullptr);
        cPinnedLocation = cEditor.getCursorCell();
      } else {
        TerrainBrush& mTerrainBrush = cEditor.getTerrainBrush();
        if (cEditor.getWorld().draw(cParent, cPinnedLocation, cEditor.getCursorCell(), mTerrainBrush.getSouthWestHeight(), mTerrainBrush.getSouthEastHeight(), mTerrainBrush.getNorthWestHeight(), mTerrainBrush.getNorthEastHeight(), mTerrainBrush.isAlternativeSplit(), cDrawingSteppedBase, cDrawingNegation, cEditor) != nullptr) {
          cPinnedZone = nullptr;
          cDrawingSteppedBase = false;
        }
      }
    } else {
      cEditor.getTerrainBrush().toggleEditing();
    }
  }

  bool TerrainType::Pen::cancel() {
    if (cEditor.getTerrainBrush().isEditing()) {
      cEditor.getTerrainBrush().toggleEditing();
      return true;
    } else if (cPinnedZone != nullptr) {
      if (cPinnedZone->empty()) {
        cEditor.getWorld().remove(cPinnedZone);
      }
      cPinnedZone = nullptr;
      return true;
    }
    return false;
  }

  void TerrainType::Pen::toggleNegation() {
    if (cPinnedZone == nullptr) {
      cDrawingNegation = !cDrawingNegation;
      if (cDrawingNegation) {
        cEditor.getTerrainBrush().reset();
      }
    }
  }

  void TerrainType::Pen::toggleShapeEditor() {
    if (!cDrawingNegation) {
      if (cPinnedZone != nullptr) {
        cDrawingSteppedBase = !cDrawingSteppedBase;
      } else {
        cEditor.getTerrainBrush().toggleEditing();
      }
    }
  }

  bool TerrainType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool TerrainType::Pen::inputTool(SignalInputID id, double yaw) {
    if (cEditor.getTerrainBrush().input(id, yaw)) {
      return true;
    }

    switch (id) {
      case SignalInputID::USE_TOOL:       draw();              return true;
      case SignalInputID::CANCEL:         return cancel();
      case SignalInputID::TOOL_MODE:      toggleNegation();    return true;
      case SignalInputID::CONFIGURE_TOOL: toggleShapeEditor(); return true;
      default:                                                 break;
    }
    return false;
  }

  bool TerrainType::Pen::isCursorLocked() const {
    return cEditor.getTerrainBrush().isCursorLocked();
  }

  void TerrainType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end == nullptr) {
      cancel();
    } else if (cPinnedZone != nullptr) {
      cPinnedZone->processCursorMovement(*end);
      if (cEditor.getTerrainBrush().isSplit()) {
        end->x = cPinnedLocation.cDefX;
        end->y = cPinnedLocation.cDefY;
      }
    }
  }

  bool TerrainType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  double TerrainType::Pen::getSnapInterval() const {
    return 1.0;
  }

  void TerrainType::Pen::renderEditingPreview() const {
    const TerrainBrush& mTerrainBrush = cEditor.getTerrainBrush();
    if (cPinnedZone == nullptr || cPinnedZone->isValidTerrainPlacement(cPinnedLocation, WorldEditorCursorCell(std::round(cEditor.getCursorX()), std::round(cEditor.getCursorY()), std::round(cEditor.getCursorZ())), mTerrainBrush.getSouthWestHeight(), mTerrainBrush.getSouthEastHeight(), mTerrainBrush.getNorthWestHeight(), mTerrainBrush.getNorthEastHeight())) {
      if (cDrawingNegation) {
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
      } else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      }
    } else {
      glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    }
    float mPinnedX = cPinnedZone != nullptr ? cPinnedLocation.cDefX : cEditor.getCursorX();
    float mPinnedY = cPinnedZone != nullptr ? cPinnedLocation.cDefY : cEditor.getCursorY();
    float mPinnedZ = cPinnedZone != nullptr ? cPinnedLocation.cDefZ : cEditor.getCursorZ();
    mTerrainBrush.renderEditing(&cParent, mPinnedX, mPinnedY, mPinnedZ, cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ(), cDrawingSteppedBase);
  }

  void TerrainType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void TerrainType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool TerrainType::renderIcon() const {
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW, 0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.4f);
    glColor3f(1.0f, 1.0f, 1.0f);
    cDefSurfacePattern->render(  -0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, ISurface::Direction::UP);
    if (cDefSolid) {
      cDefWestWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::WEST);
      cDefEastWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::EAST);
      cDefNorthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::NORTH);
      cDefSouthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::SOUTH);
    }
    return true;
  }

  bool TerrainType::Pen::renderIcon(float yaw) const {
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.4f);
    glColor3f(1.0f, 1.0f, 1.0f);
    cParent.cDefSurfacePattern->render(  -0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, ISurface::Direction::UP);
    if (cParent.cDefSolid) {
      cParent.cDefWestWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::WEST);
      cParent.cDefEastWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::EAST);
      cParent.cDefNorthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::NORTH);
      cParent.cDefSouthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::SOUTH);
    }
    return true;
  }

  void TerrainType::hintInUse(bool inUse) {
    cDefSurfacePattern->hintInUse(inUse);
    cDefWestWallPattern->hintInUse(inUse);
    cDefEastWallPattern->hintInUse(inUse);
    cDefNorthWallPattern->hintInUse(inUse);
    cDefSouthWallPattern->hintInUse(inUse);
  }
  
  bool TerrainType::isReadOnly() const {
    return false;
  } // TODO: Probably shouldn't be here.

  void TerrainType::setOwner(ProjectFile* owner) {
  } // TODO: Probably shouldn't be here.

  void TerrainType::registerAssets(const std::string& parentID) {
    cAssets.add(static_cast<IWorldEditorTool*>(this), parentID, "Terrain Types");
  }  
}

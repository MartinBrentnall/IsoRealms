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

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"
#include "Modules/Equilibria/ZoneType/ZoneType.h"

namespace IsoRealms::Equilibria {
  TerrainType::TerrainType(Equilibria& equilibria, IComponentData& data) :
            cEquilibria(equilibria),
            cComponentData(data),
            cAssets(equilibria),
            cDefSurfacePattern( equilibria, *this, [&equilibria]() {equilibria.stateChanged(nullptr);}),
            cDefWestWallPattern( equilibria, *this, [&equilibria]() {equilibria.stateChanged(nullptr);}),
            cDefEastWallPattern( equilibria, *this, [&equilibria]() {equilibria.stateChanged(nullptr);}),
            cDefSouthWallPattern(equilibria, *this, [&equilibria]() {equilibria.stateChanged(nullptr);}),
            cDefNorthWallPattern(equilibria, *this, [&equilibria]() {equilibria.stateChanged(nullptr);}),
            cDefContactAction(data.getDummyActionContext()),
            cDefImpactAction(data.getDummyActionContext()) {
  }
  
  void TerrainType::registerAssets(ComponentAssetRegistry& assets) {
    // Nothing to do.
  }


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void TerrainType::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyNativeFloat(  "friction",     [this]() {return cDefSurfaceFriction;}, [this](float value) {cDefSurfaceFriction = value;});
    owner.createPropertyNativeFloat(  "grip",         [this]() {return cDefSurfaceGrip;},     [this](float value) {cDefSurfaceGrip     = value;});
    owner.createPropertyNativeFloat(  "floorBounce",  [this]() {return cDefSurfaceBounce;},   [this](float value) {cDefSurfaceBounce   = value;});
    owner.createPropertyNativeFloat(  "wallBounce",   [this]() {return cDefWallBounce;},      [this](float value) {cDefWallBounce      = value;}, DEFAULT_WALL_BOUNCE);
    owner.createPropertyNativeBoolean("allowRespawn", [this]() {return cDefRespawnAllowed;},  [this](bool  value) {cDefRespawnAllowed  = value;});
    owner.createPropertyNativeBoolean("solid",        [this]() {return cDefSolid;},           [this](bool  value) {cDefSolid           = value;});
    owner.createPropertyTreeSelector( "onTouch",      cDefContactAction);
    owner.createPropertyTreeSelector( "onImpact",     cDefImpactAction);
    owner.createPropertyTreeSelector( "surface",      cDefSurfacePattern);
    owner.createPropertyTreeSelector( "northWall",    cDefNorthWallPattern);
    owner.createPropertyTreeSelector( "southWall",    cDefSouthWallPattern);
    owner.createPropertyTreeSelector( "westWall",     cDefWestWallPattern);
    owner.createPropertyTreeSelector( "eastWall",     cDefEastWallPattern);
  }

  void TerrainType::removed() {
    cEquilibria.removeAll(this);
  }

  bool TerrainType::hasReadOnlyReferences() const {
    return cEquilibria.isUsedInReadOnlyWorld(*this);
  }

  void TerrainType::overrideReadOnlyReferences() {
    cEquilibria.overrideReadOnlyWorlds(*this);
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
    return cEquilibria.getProject();
  }

  Equilibria& TerrainType::getEquilibria() const {
    return cEquilibria;
  }

  Equilibria& TerrainType::getAssetManager() {
    return cEquilibria;
  }

  IComponentData& TerrainType::getComponentData() {
    return cComponentData;
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

  std::vector<ConditionElement*> TerrainType::getTerrainStateConditionElements() {
    return cEquilibria.getTerrainStateConditionElements();
  }


  IWorldEditorToolInstance* TerrainType::createToolInstance(WorldEditor& editor, IComponentData& owner) {
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
            cEditor(editor) {
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
    glRotatef(Equilibria::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Equilibria::DEFAULT_VIEW_ANGLE_YAW, 0.0f, 0.0f, 1.0f);
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
    glRotatef(Equilibria::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
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

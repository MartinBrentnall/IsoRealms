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
#include "TerrainType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/ZoneType/ZoneType.h"

namespace IsoRealms::Spindizzy {
  const std::string TerrainType::TAG_IMPACT_ACTION   = "ImpactAction";
  const std::string TerrainType::TAG_CONTACT_ACTION  = "ContactAction";
  const std::string TerrainType::TAG_SURFACE         = "Surface";
  const std::string TerrainType::TAG_SPLIT_SURFACE_A = "SplitSurfaceA";
  const std::string TerrainType::TAG_SPLIT_SURFACE_B = "SplitSurfaceB";
  const std::string TerrainType::TAG_WEST_WALL       = "WestWall";
  const std::string TerrainType::TAG_EAST_WALL       = "EastWall";
  const std::string TerrainType::TAG_SOUTH_WALL      = "SouthWall";
  const std::string TerrainType::TAG_NORTH_WALL      = "NorthWall";
  const std::string TerrainType::TAG_TEXTURE         = "Texture";

  const std::string TerrainType::ATTRIBUTE_FRICTION          = "friction";
  const std::string TerrainType::ATTRIBUTE_GRIP              = "grip";
  const std::string TerrainType::ATTRIBUTE_BOUNCE            = "bounce";
  const std::string TerrainType::ATTRIBUTE_PATTERN           = "pattern";
  const std::string TerrainType::ATTRIBUTE_RESPAWN_ALLOWED   = "respawnAllowed";
  const std::string TerrainType::ATTRIBUTE_SOLID             = "solid";
  const std::string TerrainType::ATTRIBUTE_DEFAULT_ZONE_TYPE = "defaultZoneType";
  const std::string TerrainType::ATTRIBUTE_DEFAULT_THEME_SET = "defaultThemeSet";
  const std::string TerrainType::ATTRIBUTE_WALL_BOUNCE       = "wallBounce";

  const float TerrainType::DEFAULT_WALL_BOUNCE = 0.6f;

  TerrainType::TerrainType(IProject* project, Spindizzy* spindizzy) :
            cSpindizzy(spindizzy),
            cDefSurfaceFriction(0.0f),
            cDefSurfaceGrip(0.0f),
            cDefSurfaceBounce(0.0f),
            cDefWallBounce(DEFAULT_WALL_BOUNCE),
            cDefRespawnAllowed(true),
            cDefSolid(false),
            cDefSurfacePattern(spindizzy),
            cDefWestWallPattern(spindizzy),
            cDefEastWallPattern(spindizzy),
            cDefSouthWallPattern(spindizzy),
            cDefNorthWallPattern(spindizzy),
            cDefContactAction(project),
            cDefImpactAction(project) {
  }
  
  TerrainType::TerrainType(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            TerrainType(project, spindizzy) {
    cDefSolid = node.getBooleanAttribute(ATTRIBUTE_SOLID);
    cDefSurfaceBounce = node.getFloatAttribute(ATTRIBUTE_BOUNCE);
    cDefWallBounce = node.getFloatAttribute(ATTRIBUTE_WALL_BOUNCE, DEFAULT_WALL_BOUNCE);
    cDefRespawnAllowed = node.getBooleanAttribute(ATTRIBUTE_RESPAWN_ALLOWED);
    cDefSurfaceGrip = node.getFloatAttribute(ATTRIBUTE_GRIP);
    cDefSurfaceFriction = node.getFloatAttribute(ATTRIBUTE_FRICTION);
    cDefContactAction.init(node, TAG_CONTACT_ACTION);
    cDefImpactAction.init(node, TAG_IMPACT_ACTION);
    cDefWestWallPattern.init(node.getNode(TAG_WEST_WALL));
    cDefEastWallPattern.init(node.getNode(TAG_EAST_WALL));
    cDefSouthWallPattern.init(node.getNode(TAG_SOUTH_WALL));
    cDefNorthWallPattern.init(node.getNode(TAG_NORTH_WALL));
    cDefSurfacePattern.init(node.getNode(TAG_SURFACE));
  }
  
  void TerrainType::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }

  void TerrainType::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<IProperty*> TerrainType::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  Spindizzy* TerrainType::getSpindizzy() const {
    return cSpindizzy;
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

  void TerrainType::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_SOLID, cDefSolid);
    node->addAttribute(ATTRIBUTE_FRICTION, cDefSurfaceFriction);
    node->addAttribute(ATTRIBUTE_GRIP, cDefSurfaceGrip);
    node->addAttribute(ATTRIBUTE_BOUNCE, cDefSurfaceBounce);
    node->addAttribute(ATTRIBUTE_WALL_BOUNCE, cDefWallBounce, DEFAULT_WALL_BOUNCE);
    node->addAttribute(ATTRIBUTE_RESPAWN_ALLOWED, cDefRespawnAllowed);
    cDefImpactAction.save(node, TAG_IMPACT_ACTION);
    cDefContactAction.save(node, TAG_CONTACT_ACTION);
    cDefSurfacePattern.save(node, TAG_SURFACE);
    cDefWestWallPattern.save(node, TAG_WEST_WALL);
    cDefEastWallPattern.save(node, TAG_EAST_WALL);
    cDefSouthWallPattern.save(node, TAG_SOUTH_WALL);
    cDefNorthWallPattern.save(node, TAG_NORTH_WALL);
  }

  std::vector<ConditionElement*> TerrainType::getTerrainStateConditionElements() {
    return cSpindizzy->getTerrainStateConditionElements();
  }








  IWorldEditorToolInstance* TerrainType::createToolInstance(WorldEditor* editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool TerrainType::renderAssetIcon() const {
    return false;
  }

  TerrainType::Pen::Pen(TerrainType& parent, WorldEditor* editor) :
            cParent(parent),
            cEditor(editor),
            cPinnedZone(nullptr),
            cDrawingNegation(false) {
  }

  void TerrainType::Pen::draw() {
    if (!cEditor->getTerrainBrush().isEditing()) {
      if (cPinnedZone == nullptr) {
        cPinnedZone = cEditor->getWorld()->getOrDrawZone(cEditor->getCursorCell(), cEditor);
        cPinnedLocation = cEditor->getCursorCell();
      } else {
        TerrainBrush& mTerrainBrush = cEditor->getTerrainBrush();
        if (cEditor->getWorld()->draw(&cParent, cPinnedLocation, cEditor->getCursorCell(), mTerrainBrush.getSouthWestHeight(), mTerrainBrush.getSouthEastHeight(), mTerrainBrush.getNorthWestHeight(), mTerrainBrush.getNorthEastHeight(), mTerrainBrush.isAlternativeSplit(), cDrawingSteppedBase, cDrawingNegation, cEditor) != nullptr) {
          cPinnedZone = nullptr;
          cDrawingSteppedBase = false;
        }
      }
    } else {
      cEditor->getTerrainBrush().toggleEditing();
    }
  }

  bool TerrainType::Pen::cancel() {
    if (cEditor->getTerrainBrush().isEditing()) {
      cEditor->getTerrainBrush().toggleEditing();
      return true;
    } else if (cPinnedZone != nullptr) {
      if (cPinnedZone->empty()) {
        cEditor->getWorld()->remove(cPinnedZone);
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
        cEditor->getTerrainBrush().reset();
      }
    }
  }

  void TerrainType::Pen::toggleShapeEditor() {
    if (!cDrawingNegation) {
      if (cPinnedZone != nullptr) {
        cDrawingSteppedBase = !cDrawingSteppedBase;
      } else {
        cEditor->getTerrainBrush().toggleEditing();
      }
    }
  }

  bool TerrainType::Pen::inputEdit(sf::Event& event) {
    if (cEditor->getTerrainBrush().input(event)) {
      return true;
    }

    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Escape:                      return cancel();
          case sf::Keyboard::Tab:    toggleNegation();    return true;
          case sf::Keyboard::Space:  draw();              return true;
          case sf::Keyboard::F3:     toggleShapeEditor(); return true;
          default:                                        break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: draw();                            return true;
          case 1: if (!cancel()) {toggleNegation();} return true;
          case 2: toggleShapeEditor();               return true;
        }
        break;
      }

      default: {
        break;
      }
    }
    return false;
  }

  void TerrainType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (cPinnedZone != nullptr) {
      cPinnedZone->processCursorMovement(*end);
      if (cEditor->getTerrainBrush().isSplit()) {
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
    const TerrainBrush& mTerrainBrush = cEditor->getTerrainBrush();
    if (cPinnedZone == nullptr || cPinnedZone->isValidTerrainPlacement(cPinnedLocation, WorldEditorCursorCell(std::round(cEditor->getCursorX()), std::round(cEditor->getCursorY()), std::round(cEditor->getCursorZ())), mTerrainBrush.getSouthWestHeight(), mTerrainBrush.getSouthEastHeight(), mTerrainBrush.getNorthWestHeight(), mTerrainBrush.getNorthEastHeight())) {
      if (cDrawingNegation) {
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
      } else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      }
    } else {
      glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    }
    float mPinnedX = cPinnedZone != nullptr ? cPinnedLocation.cDefX : cEditor->getCursorX();
    float mPinnedY = cPinnedZone != nullptr ? cPinnedLocation.cDefY : cEditor->getCursorY();
    float mPinnedZ = cPinnedZone != nullptr ? cPinnedLocation.cDefZ : cEditor->getCursorZ();
    mTerrainBrush.renderEditing(&cParent, mPinnedX, mPinnedY, mPinnedZ, cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ(), cDrawingSteppedBase);
  }

  void TerrainType::Pen::renderUI() const {
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
    cDefWestWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::WEST);
    cDefEastWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::EAST);
    cDefNorthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::NORTH);
    cDefSouthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::SOUTH);
    return true;
  }

  bool TerrainType::Pen::renderIcon(float yaw) const {
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.4f);
    glColor3f(1.0f, 1.0f, 1.0f);
    cParent.cDefSurfacePattern->render(  -0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, ISurface::Direction::UP);
    cParent.cDefWestWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::WEST);
    cParent.cDefEastWallPattern->render(  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::EAST);
    cParent.cDefNorthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::NORTH);
    cParent.cDefSouthWallPattern->render( 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Wall::Direction::SOUTH);
    return true;
  }

  void TerrainType::hintInUse(bool inUse) {
    cDefSurfacePattern->hintInUse(inUse);
    cDefWestWallPattern->hintInUse(inUse);
    cDefEastWallPattern->hintInUse(inUse);
    cDefNorthWallPattern->hintInUse(inUse);
    cDefSouthWallPattern->hintInUse(inUse);
  }
  
  TerrainType::~TerrainType() {
    cSpindizzy->removeAll(this);
  }
  
  void TerrainType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(this, "");
  }  
}

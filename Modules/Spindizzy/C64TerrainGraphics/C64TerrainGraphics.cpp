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
#include "C64TerrainGraphics.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  const float C64TerrainGraphics::TILE_SIZE                    = 0.88f;
  const float C64TerrainGraphics::SWITCH_SQUARE_OUTER          = 0.63f;
  const float C64TerrainGraphics::SWITCH_SQUARE_INNER          = 0.44f;
  const float C64TerrainGraphics::SWITCH_SQUARE_TRIANGLE_INNER = 0.0f;

  const float C64TerrainGraphics::SWITCH_DIAMOND_OUTER         = 0.66f;
  const float C64TerrainGraphics::SWITCH_DIAMOND_INNER         = 0.41f;

  const float C64TerrainGraphics::SWITCH_CIRCLE_OUTER          = 0.7f;
  const float C64TerrainGraphics::SWITCH_CIRCLE_INNER          = 0.52f;
  const float C64TerrainGraphics::ICE_EDGE_WIDTH               = 0.1f;
  const float C64TerrainGraphics::ARROW_SIZE                   = 0.66f;
  const float C64TerrainGraphics::ARROW_LINE_WIDTH             = 0.25f;
  const float C64TerrainGraphics::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);
  
  const float C64TerrainGraphics::MAX_LUMINANCE_ADJUSTMENT     = 0.3f;

  const std::string C64TerrainGraphics::PLAIN                = "Plain";
  const std::string C64TerrainGraphics::PLAIN_SPLIT          = "PlainSplit";
  const std::string C64TerrainGraphics::ICE_WATER            = "IceWater";
  const std::string C64TerrainGraphics::ARROW                = "Arrow";
  const std::string C64TerrainGraphics::SWITCH_CIRCLE_BOTH   = "SwitchCircleFilled";
  const std::string C64TerrainGraphics::SWITCH_CIRCLE_ONE    = "SwitchCircleOne";
  const std::string C64TerrainGraphics::SWITCH_CIRCLE_NONE   = "SwitchCircleEmpty";
  const std::string C64TerrainGraphics::SWITCH_SQUARE_BOTH   = "SwitchSquareFilled";
  const std::string C64TerrainGraphics::SWITCH_SQUARE_ONE    = "SwitchSquareOne";
  const std::string C64TerrainGraphics::SWITCH_SQUARE_NONE   = "SwitchSquareEmpty";
  const std::string C64TerrainGraphics::SWITCH_DIAMOND_BOTH  = "SwitchDiamondFilled";
  const std::string C64TerrainGraphics::SWITCH_DIAMOND_ONE   = "SwitchDiamondOne";
  const std::string C64TerrainGraphics::SWITCH_DIAMOND_NONE  = "SwitchDiamondEmpty";
  const std::string C64TerrainGraphics::TRAMPOLINE           = "Trampoline";
  const std::string C64TerrainGraphics::WALL_ICE             = "WallIce";
  const std::string C64TerrainGraphics::WALL_MIXED_CAP       = "WallMixedCap";
  const std::string C64TerrainGraphics::WALL_MIXED_MIDDLE    = "WallMixedMiddle";
  const std::string C64TerrainGraphics::WALL_PLAIN_CAP       = "WallPlainCap";
  const std::string C64TerrainGraphics::WALL_PLAIN_MIDDLE    = "WallPlainMiddle";

  const std::string C64TerrainGraphics::JSON_FLOOR     = "floor";
  const std::string C64TerrainGraphics::JSON_GRID      = "grid";
  const std::string C64TerrainGraphics::JSON_HIGHLIGHT = "highlight";
  const std::string C64TerrainGraphics::JSON_WALL      = "wall";

  C64TerrainGraphics::C64TerrainGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cProject(project),
            cDefaultYaw(data, Spindizzy::DEFAULT_VIEW_ANGLE_YAW),
            cDefFloor(data, 1.0f, 1.0f, 1.0f, 0.0f, [this]() {setNeedsFullRedraw();}),
            cDefWall(data, 0.7f, 0.7f, 0.7f, 0.0f, [this]() {setNeedsFullRedraw();}),
            cDefGrid(data, 0.3f, 0.3f, 0.3f, 0.0f, [this]() {setNeedsFullRedraw();}),
            cDefHighlight(data, 0.0f, 0.0f, 0.0f, 0.0f, [this]() {setNeedsFullRedraw();}) {


    cTexturesInUseCount = 0;
    
    cTextures[SWITCH_CIRCLE_BOTH]         = createTexture();
    cOrientedTextures[SWITCH_CIRCLE_ONE]  = createOrientedTexture();
    cTextures[SWITCH_CIRCLE_NONE]         = createTexture();
    cOrientedTextures[SWITCH_SQUARE_BOTH] = createOrientedTexture();
    cOrientedTextures[SWITCH_SQUARE_ONE]  = createOrientedTexture();
    cTextures[SWITCH_SQUARE_NONE]         = createTexture();
    cTextures[SWITCH_DIAMOND_NONE]        = createTexture();
    cOrientedTextures[SWITCH_DIAMOND_ONE] = createOrientedTexture();
    cTextures[SWITCH_DIAMOND_BOTH]        = createTexture();
    cTextures[ARROW]                      = createTexture();
    cTextures[TRAMPOLINE]                 = createTexture();
    cTextures[ICE_WATER]                  = createTexture();
    cTextures[PLAIN]                      = createTexture();
    cTextures[PLAIN_SPLIT]                = createTexture();
    cOrientedTextures[WALL_MIXED_CAP]     = createOrientedTexture();
    cOrientedTextures[WALL_MIXED_MIDDLE]  = createOrientedTexture();
    cOrientedTextures[WALL_PLAIN_CAP]     = createOrientedTexture();
    cOrientedTextures[WALL_PLAIN_MIDDLE]  = createOrientedTexture();
    cTextures[WALL_ICE]                   = createTexture();

    for (std::pair<const std::string, std::unique_ptr<OrientedTexture>>& mOrientedTexture : cOrientedTextures) {
      bool mClamp = mOrientedTexture.first == WALL_MIXED_CAP || mOrientedTexture.first == WALL_PLAIN_CAP;
      mOrientedTexture.second->addOrientation(*cDefaultYaw, cProject, mClamp);
    }
    cUniqueViews.insert(*cDefaultYaw);
    
    // This sets the default texture orientation
    screenPostRender(nullptr);

    project.addScreenListener(this);
  }
  
  C64TerrainGraphics::C64TerrainGraphics(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            C64TerrainGraphics(project, spindizzy, data) {
    cDefFloor.init(object, JSON_FLOOR);
    cDefWall.init(object, JSON_WALL);
    cDefGrid.init(object, JSON_GRID);
    cDefHighlight.init(object, JSON_HIGHLIGHT);
    setNeedsFullRedraw();
  }

  void C64TerrainGraphics::setNeedsFullRedraw() {
    cNeedsFullRedraw = true;
    cChangedAngles.clear();
    if (cTexturesInUseCount > 0) {
      updateLater();
    }
  }
  
  std::vector<std::unique_ptr<IProperty>> C64TerrainGraphics::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>(owner.getPropertyData("Floor"),     cDefFloor));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>(owner.getPropertyData("Wall"),      cDefWall));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>(owner.getPropertyData("Grid"),      cDefGrid));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>(owner.getPropertyData("highlight"), cDefHighlight));
    return mProperties;
  }
  
  bool C64TerrainGraphics::renderIcon() {
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW,   0.0f, 0.0f, 1.0f);
    glScalef(1.6f, 1.6f, 1.6f);
    cTextures[ARROW]->set();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  0.5f, 0.25f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.25f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, 0.25f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f,  0.5f, 0.25f);
    glEnd();
    
    cOrientedTextures[WALL_PLAIN_CAP]->set();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.25f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.25f);
    
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.25f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.25f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
    glEnd();
    
    cOrientedTextures[WALL_MIXED_CAP]->set();
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.25f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.25f);
    
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.25f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.25f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.0f);
    glEnd();
    return true;
  }

  void C64TerrainGraphics::hintInUse(bool inUse) {
    for (std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mTexture : cTextures) {
      mTexture.second->hintTextureInUse(inUse);
    }
  }
  
  void C64TerrainGraphics::registerAssets(ResourceAssetRegistry& assets) {
    for (std::pair<const std::string, std::unique_ptr<LiteralTexture>>& mTexture : cTextures) {
      assets.add<ITexture>(mTexture.second.get(), mTexture.first, "Spindizzy Terrain Textures");
    }
    for (std::pair<const std::string, std::unique_ptr<OrientedTexture>>& mOrientedTexture : cOrientedTextures) {
      assets.add<ITexture>(mOrientedTexture.second.get(), mOrientedTexture.first, "Spindizzy Terrain Textures");
    }
  }
    
  std::unique_ptr<LiteralTexture> C64TerrainGraphics::createTexture(bool clamp) {
    std::unique_ptr<LiteralTexture> mTexture = std::make_unique<LiteralTexture>(cProject, 128, 128, false, clamp);
    mTexture->addUseListener(this);
    return mTexture;
  }

  std::unique_ptr<C64TerrainGraphics::OrientedTexture> C64TerrainGraphics::createOrientedTexture() {
    std::unique_ptr<OrientedTexture> mOrientedTexture = std::make_unique<OrientedTexture>();
    mOrientedTexture->addUseListener(this);
    return mOrientedTexture;
  }
  
  float C64TerrainGraphics::getGridWallLuminanceAdjustment() {
    return std::clamp(Utils::luminance(**cDefWall) / Utils::luminance(**cDefFloor), 1.0f - MAX_LUMINANCE_ADJUSTMENT, 1.0f + MAX_LUMINANCE_ADJUSTMENT) - 1.0f;
  }

  void C64TerrainGraphics::clear(const IColour& colour) {
    glClearColor(colour.getRed(), colour.getGreen(), colour.getBlue(), colour.getAlpha());
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void C64TerrainGraphics::renderSquare(float size, Colour& colour) {
    glBegin(GL_QUADS);
    colour->set();
    glVertex2f(-size, -size);
    glVertex2f( size, -size);
    glVertex2f( size,  size);
    glVertex2f(-size,  size);
    glEnd();
  }

  void C64TerrainGraphics::renderDiamond(float size, Colour& colour) {
    glBegin(GL_QUADS);
    colour->set();
    glVertex2f(-size,  0.0f);
    glVertex2f( 0.0f, -size);
    glVertex2f( size,  0.0f);
    glVertex2f( 0.0f,  size);
    glEnd();
  }

  void C64TerrainGraphics::renderCircle(float radius, Colour& colour) {
    glBegin(GL_TRIANGLE_FAN);
    colour->set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = 0.0f * (M_PI / 180.0f);
    float mEndAngle = 360.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * radius, std::cos(angle) * radius);
    }
    glEnd();
  }

  void C64TerrainGraphics::renderRectangle(float xs, float ys, float xe, float ye, const IColour& colour) {
    colour.set();
    glBegin(GL_QUADS);
    glVertex2f(xe, ys);
    glVertex2f(xe, ye);
    glVertex2f(xs, ye);
    glVertex2f(xs, ys);
    glEnd();
  }

  void C64TerrainGraphics::renderTile(Colour& colour) {
    clear(**cDefGrid);
    renderSquare(TILE_SIZE, colour);
  }

  void C64TerrainGraphics::renderIce() {
    return clear(**cDefGrid);
  }

  void C64TerrainGraphics::renderPlain() {
    renderTile(cDefFloor);
  }

  void C64TerrainGraphics::renderSplitPlain() {
    clear(**cDefGrid);
    glBegin(GL_TRIANGLES);
    float mInnerTileSize = TILE_SIZE - (1.0f - TILE_SIZE);
    glVertex2f(-TILE_SIZE,      -TILE_SIZE);
    glVertex2f( mInnerTileSize, -TILE_SIZE);
    glVertex2f(-TILE_SIZE,       mInnerTileSize);
    glVertex2f( TILE_SIZE,       TILE_SIZE);
    glVertex2f(-mInnerTileSize,  TILE_SIZE);
    glVertex2f( TILE_SIZE,      -mInnerTileSize);
    glEnd();
  }

  void C64TerrainGraphics::renderTrampoline() {
    renderTile(cDefHighlight);
    renderSquare(TILE_SIZE - (1.0f - TILE_SIZE), cDefWall);
  }

  void C64TerrainGraphics::renderSwitchSquare(float angle) {
    renderTile(cDefFloor);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    renderSquare(SWITCH_SQUARE_OUTER, cDefWall);
    renderSquare(SWITCH_SQUARE_INNER, cDefFloor);
  }

  void C64TerrainGraphics::renderSwitchSquareHalf(float angle) {
    renderSwitchSquare(angle);
    glBegin(GL_TRIANGLES);
    cDefGrid->set();
    glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_INNER);
    glVertex2f(SWITCH_SQUARE_INNER,          -SWITCH_SQUARE_TRIANGLE_INNER);
    glVertex2f(SWITCH_SQUARE_TRIANGLE_INNER, -SWITCH_SQUARE_INNER);
    glEnd();
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
  }

  void C64TerrainGraphics::renderSwitchSquareBoth(float angle) {
    renderSwitchSquareHalf(angle);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    cDefGrid->set();
    glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_INNER);
    glVertex2f(-SWITCH_SQUARE_INNER,          SWITCH_SQUARE_TRIANGLE_INNER);
    glVertex2f(-SWITCH_SQUARE_TRIANGLE_INNER, SWITCH_SQUARE_INNER);
    glEnd();
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
  }

  void C64TerrainGraphics::renderSwitchDiamond(float angle) {
    renderTile(cDefFloor);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    renderSquare(SWITCH_DIAMOND_OUTER, cDefWall);
    renderSquare(SWITCH_DIAMOND_INNER, cDefFloor);
    renderDiamond(SWITCH_DIAMOND_OUTER, cDefGrid);
    renderDiamond(SWITCH_DIAMOND_INNER, cDefFloor);
  }

  void C64TerrainGraphics::renderSwitchDiamondHalf(float angle) {
    renderSwitchDiamond(angle);
    renderRectangle( SWITCH_DIAMOND_INNER, -SWITCH_DIAMOND_OUTER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, **cDefWall);
    renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER, **cDefWall);
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
  }

  void C64TerrainGraphics::renderSwitchDiamondBoth() {
    renderSwitchDiamondHalf(0.0f);
    renderRectangle(-SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_OUTER, -SWITCH_DIAMOND_INNER,  SWITCH_DIAMOND_OUTER, **cDefWall);
    renderRectangle(-SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_INNER, SWITCH_DIAMOND_OUTER,  SWITCH_DIAMOND_OUTER, **cDefWall);
  }

  void C64TerrainGraphics::renderSwitchCircle(float angle) {
    renderTile(cDefFloor);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    renderCircle(SWITCH_CIRCLE_OUTER, cDefGrid);
    renderCircle(SWITCH_CIRCLE_INNER, cDefFloor);
  }

  void C64TerrainGraphics::renderSwitchCircleBoth() {
    renderTile(cDefFloor);
    renderCircle(SWITCH_CIRCLE_OUTER, cDefGrid);
    renderCircle(SWITCH_CIRCLE_INNER, cDefWall);
  }

  void C64TerrainGraphics::renderSwitchCircleHalf(float angle) {
    renderSwitchCircle(angle);
    glBegin(GL_TRIANGLE_FAN);
    cDefWall->set();
    glVertex2f(0.0f, 0.0f);
    float mStartAngle = 45.0f * (M_PI / 180.0f);
    float mEndAngle = 225.0f * (M_PI / 180.0f);
    for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
      glVertex2f(std::sin(angle) * SWITCH_CIRCLE_INNER, std::cos(angle) * SWITCH_CIRCLE_INNER);
    }
    glEnd();
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
  }

  void C64TerrainGraphics::renderArrow() {
    renderTile(cDefFloor);
    renderRectangle(-ARROW_LINE_WIDTH, 0.0f, ARROW_LINE_WIDTH, ARROW_SIZE, **cDefWall);
    glBegin(GL_TRIANGLES);
    cDefWall->set();
    glVertex2f( ARROW_SIZE,  0.0f);
    glVertex2f(-ARROW_SIZE,  0.0f);
    glVertex2f( 0.0f,       -ARROW_SIZE);
    glEnd();
  }

  void C64TerrainGraphics::renderWallMiddle(float interpolation) {
    LiteralColour mGridColour(**cDefGrid, 1.0f + getGridWallLuminanceAdjustment() / (interpolation + 1.0f));
    LiteralColour mWallFloorMix(**cDefWall, **cDefFloor, interpolation * 0.5f);
    clear(mGridColour);
    renderRectangle(-TILE_SIZE, -1.0f, TILE_SIZE, 1.0f, mWallFloorMix);
  }

  void C64TerrainGraphics::renderWallCap(float interpolation) {
    LiteralColour mGridColour(**cDefGrid, 1.0f + getGridWallLuminanceAdjustment() / (interpolation + 1.0f));
    LiteralColour mWallFloorMix(**cDefWall, **cDefFloor, interpolation * 0.5f);
    clear(mGridColour);
    renderRectangle(-TILE_SIZE, -TILE_SIZE / 2.0f, TILE_SIZE, 1.0f, mWallFloorMix);
  }

  void C64TerrainGraphics::renderIceWall() {
    clear(**cDefGrid);
    glBegin(GL_QUADS);
    cDefHighlight->set();
    glVertex2f( 1.0f,                  -1.0f);
    glVertex2f( 1.0f - ICE_EDGE_WIDTH, -1.0f + ICE_EDGE_WIDTH * 2.0f);
    glVertex2f(-1.0f + ICE_EDGE_WIDTH, -1.0f + ICE_EDGE_WIDTH * 2.0f);
    glVertex2f(-1.0f,                  -1.0f);
    glEnd();
  }

  void C64TerrainGraphics::generateAngledTextures(const IFloat* angle) {
    float mAngle = angle != nullptr ? angle->getValue() + 45.0f : 45.0f;
    if (mAngle > 180.0f) {
      mAngle -= 360.0f;
    }
    cOrientedTextures[SWITCH_CIRCLE_ONE]->setRenderTarget(angle);   renderSwitchCircleHalf(mAngle);
    cOrientedTextures[SWITCH_SQUARE_ONE]->setRenderTarget(angle);   renderSwitchSquareHalf(mAngle);
    cOrientedTextures[SWITCH_SQUARE_BOTH]->setRenderTarget(angle);  renderSwitchSquareBoth(mAngle);
    cOrientedTextures[SWITCH_DIAMOND_ONE]->setRenderTarget(angle);  renderSwitchDiamondHalf(mAngle);
    float mInterpolation = std::abs(mAngle / 90.0f);
    if (mInterpolation > 1.0f) {
      mInterpolation -= ((mInterpolation - 1.0f) * 2.0f);
    }
    cOrientedTextures[WALL_MIXED_CAP]->setRenderTarget(angle);      renderWallCap(std::abs(mInterpolation - 1.0f));
    cOrientedTextures[WALL_MIXED_MIDDLE]->setRenderTarget(angle);   renderWallMiddle(std::abs(mInterpolation - 1.0f));
    cOrientedTextures[WALL_PLAIN_CAP]->setRenderTarget(angle);      renderWallCap(mInterpolation);
    cOrientedTextures[WALL_PLAIN_MIDDLE]->setRenderTarget(angle);   renderWallMiddle(mInterpolation);
  }

  void C64TerrainGraphics::generateTextures() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    
    for (const IFloat* mView : cUniqueViews) {
      generateAngledTextures(mView);
    }

    cTextures[SWITCH_CIRCLE_BOTH]->setRenderTarget();  renderSwitchCircleBoth();
    cTextures[SWITCH_CIRCLE_NONE]->setRenderTarget();  renderSwitchCircle(0.0f);
    cTextures[SWITCH_SQUARE_NONE]->setRenderTarget();  renderSwitchSquare(0.0f);
    cTextures[SWITCH_DIAMOND_BOTH]->setRenderTarget(); renderSwitchDiamondBoth();
    cTextures[SWITCH_DIAMOND_NONE]->setRenderTarget(); renderSwitchDiamond(0.0f);
    cTextures[ARROW]->setRenderTarget();               renderArrow();
    cTextures[TRAMPOLINE]->setRenderTarget();          renderTrampoline();
    cTextures[ICE_WATER]->setRenderTarget();           renderIce();
    cTextures[PLAIN]->setRenderTarget();               renderPlain();
    cTextures[PLAIN_SPLIT]->setRenderTarget();         renderSplitPlain();
    cTextures[WALL_ICE]->setRenderTarget();            renderIceWall();

    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    IApplication& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }

  // TODO: Redraw on Float relinquish

  void C64TerrainGraphics::save(JSONObject object) const {
    cDefFloor.save(object, JSON_FLOOR);
    cDefWall.save(object, JSON_WALL);
    cDefGrid.save(object, JSON_GRID);
    cDefHighlight.save(object, JSON_HIGHLIGHT);
  }

  void C64TerrainGraphics::performAngleRedraw(IFloat* angle) {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    generateAngledTextures(angle);
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();  

    IApplication& mApplication = cProject.getApplication();
    mApplication.setViewPort();
  }

  void C64TerrainGraphics::hintTextureUsed(ITexture* texture, bool inUse) {
    if (cTexturesInUseCount == 0 && inUse && (!cChangedAngles.empty() || cNeedsFullRedraw)) {
      updateLater();
    }
    cTexturesInUseCount += inUse ? 1 : -1;
  }

  void C64TerrainGraphics::stateChanged(IFloat* value) {
    if (!cNeedsFullRedraw) {
      cChangedAngles.insert(value);
      if (cTexturesInUseCount > 0) {
        updateLater();
      }
    }
  }

  void C64TerrainGraphics::updateLater() {
    cProject.updateLater([this]() {
      if (cNeedsFullRedraw) {
        generateTextures();
      } else for (IFloat* mAngle : cChangedAngles) {
        performAngleRedraw(mAngle);
      }
      cNeedsFullRedraw = false;
      cChangedAngles.clear();
    });
  }
  
  void C64TerrainGraphics::screenAdded(IProject& project, const IScreen* screen) {
    const IFloat* mAngle = screen->getYaw(); // TODO: What happens if the screen gets assigned a different IFloat asset!?
    if (mAngle != nullptr) {
      project.addStateChangeListener(mAngle, this);
      for (std::pair<const std::string, std::unique_ptr<OrientedTexture>>& mOrientedTexture : cOrientedTextures) {
        bool mClamp = mOrientedTexture.first == WALL_MIXED_CAP || mOrientedTexture.first == WALL_PLAIN_CAP;
        mOrientedTexture.second->addOrientation(mAngle, cProject, mClamp);
      }
      cUniqueViews.insert(mAngle);
    }
  }

  void C64TerrainGraphics::screenRemoved(const IScreen* screen) {
    // TODO: Implement this
  }

  void C64TerrainGraphics::screenPreRender(const IScreen* screen) {
    const IFloat* mYaw = screen->getYaw();
    for (std::pair<const std::string, std::unique_ptr<OrientedTexture>>& mOrientedTexture : cOrientedTextures) {
      mOrientedTexture.second->setScreen(mYaw);
    }
  }
  
  void C64TerrainGraphics::screenPostRender(const IScreen* screen) {
    for (std::pair<const std::string, std::unique_ptr<OrientedTexture>>& mOrientedTexture : cOrientedTextures) {
      mOrientedTexture.second->setScreen(*cDefaultYaw);
    }
  }
    
  C64TerrainGraphics::OrientedTexture::OrientedTexture() {
    cCurrentTexture = nullptr;
  }
    
  void C64TerrainGraphics::OrientedTexture::addOrientation(const IFloat* angle, IProject& project, bool clamp) {
    if (cTextures.find(angle) == cTextures.end()) {
      cTextures[angle] = std::make_unique<LiteralTexture>(project, 128, 128, false, clamp);
      if (angle == nullptr) {
        cCurrentTexture = cTextures[angle].get();
      }
    }
  }
  
  void C64TerrainGraphics::OrientedTexture::setRenderTarget(const IFloat* angle) {
    std::map<const IFloat*, std::unique_ptr<LiteralTexture>>::iterator mTexture = cTextures.find(angle);
    if (mTexture != cTextures.end()) {
      mTexture->second->setRenderTarget();
    }
  }
  
  void C64TerrainGraphics::OrientedTexture::setScreen(const IFloat* angle) {
    std::map<const IFloat*, std::unique_ptr<LiteralTexture>>::iterator mTexture = cTextures.find(angle);
    if (mTexture != cTextures.end()) {
      cCurrentTexture = mTexture->second.get();
    }
  }
  
  void C64TerrainGraphics::OrientedTexture::addUseListener(ITextureUseListener* listener) {
    cListeners.insert(listener);
  }
  
  ITexture* C64TerrainGraphics::OrientedTexture::getTexture() {
    return cCurrentTexture;
  }
  
  void C64TerrainGraphics::OrientedTexture::set() const {
    cCurrentTexture->set();
  }
  
  void C64TerrainGraphics::OrientedTexture::hintTextureInUse(bool inUse) {
    cCurrentTexture->hintTextureInUse(inUse); // TODO: Should do all texture and not just the current one?
  }
  
  void C64TerrainGraphics::OrientedTexture::coord(float x, float y) const {
    glTexCoord2f(x, y);
  }

  void C64TerrainGraphics::OrientedTexture::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> C64TerrainGraphics::OrientedTexture::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool C64TerrainGraphics::OrientedTexture::isDefaultConfiguration() const {
    return true;
  }
}

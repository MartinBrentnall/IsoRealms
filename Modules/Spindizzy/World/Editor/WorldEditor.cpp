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
#include "WorldEditor.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"

namespace IsoRealms::Spindizzy {
  const float WorldEditor::SPEED_FAST     = 0.3f;
  const float WorldEditor::SPEED_NORMAL   = 0.1f;
  const float WorldEditor::SPEED_SLOW     = 0.05f;
  const float WorldEditor::STOP_THRESHOLD = 0.01f;
  const float WorldEditor::SELECTION_BOUNDARY_RENDERING_OFFSET = 0.001f;

  const float WorldEditor::BORDER_SPACING = 0.02f;
  const float WorldEditor::ICON_WIDTH = 0.12f;
  const float WorldEditor::ICON_HEIGHT = 0.12f;
  const float WorldEditor::BOTTOM_BORDER = -1.0f + BORDER_SPACING;
  const float WorldEditor::ICON_SPACING = 0.02f;
  
  WorldEditor::WorldEditor(IAssetRegistry& assets, World& world) :
            cAnalogueInputsByName({
              {"MoveViewIn",      &cDistanceInSpeed},
              {"MoveViewOut",     &cDistanceOutSpeed},
              {"MoveCursorX",     &cXSpeed},
              {"MoveCursorY",     &cYSpeed},
              {"MoveCursorZ",     &cZSpeed},
              {"RotateViewPitch", &cPitchSpeed},
              {"RotateViewYaw",   &cYawSpeed}
            }),
            cDigitalInputsByName({
              {"MoveCursorBackward", &cActiveDown},
              {"MoveCursorDown",     &cActiveLower},
              {"MoveCursorFaster",   &cActiveFast},
              {"MoveCursorForward",  &cActiveUp},
              {"MoveCursorLeft",     &cActiveLeft},
              {"MoveCursorRight",    &cActiveRight},
              {"MoveCursorSlower",   &cActiveSlow},
              {"MoveCursorUp",       &cActiveHigher},
              {"MoveView",           &cZoomingView},
              {"RotateView",         &cRotatingView},
              {"Cancel",             &cCancel},
              {"ConfigureTool",      &cConfigureTool},
              {"NextTheme",          &cNextTheme},
              {"NextTool",           &cNextTool},
              {"PreviousTheme",      &cPreviousTheme},
              {"PreviousTool",       &cPreviousTool},
              {"ToolMode",           &cToolMode},
              {"UseTool",            &cUseTool},
              {"Exit",               &cExit},
            }),
            cActiveLeft(*this, SignalInputID::MOVE_CURSOR_LEFT),
            cActiveRight(*this, SignalInputID::MOVE_CURSOR_RIGHT),
            cActiveUp(*this, SignalInputID::MOVE_CURSOR_FORWARD),
            cActiveDown(*this, SignalInputID::MOVE_CURSOR_BACKWARD),
            cActiveHigher(*this, SignalInputID::MOVE_CURSOR_UP),
            cActiveLower(*this, SignalInputID::MOVE_CURSOR_DOWN),
            cActiveSlow(*this, SignalInputID::MOVE_CURSOR_SLOWER),
            cActiveFast(*this, SignalInputID::MOVE_CURSOR_FASTER),
            cRotatingView(*this, SignalInputID::ROTATE_VIEW),
            cZoomingView(*this, SignalInputID::MOVE_VIEW),
            cCancel(*this, SignalInputID::CANCEL),
            cConfigureTool(*this, SignalInputID::CONFIGURE_TOOL),
            cNextTheme(*this, SignalInputID::NEXT_THEME),
            cNextTool(*this, SignalInputID::NEXT_TOOL),
            cPreviousTheme(*this, SignalInputID::PREVIOUS_THEME),
            cPreviousTool(*this, SignalInputID::PREVIOUS_TOOL),
            cToolMode(*this, SignalInputID::TOOL_MODE),
            cUseTool(*this, SignalInputID::USE_TOOL),
            cExit(*this, SignalInputID::EXIT),
            cSignalConsumed(false),
            cHasFocus(true),
            cExitAction(nullptr),
            cScreenYaw(&cRotation),
            cScreenPitch(&cTilt),
            cHatHandler(world.getSpindizzy().getProject().getApplication().getHatHandler()),
            cSelectedTool(nullptr),
            cPreviousX(0),
            cPreviousY(0),
            cWorld(world),
            cToolbar(world.getSpindizzy().createToolSet(*this), [this](IWorldEditorToolInstance* tool) {
              if (cSelectedTool != nullptr) {
                cSelectedTool->processCursorMovement(&cLocation, nullptr);
              }
              cSelectedTool = tool;
              cSelectedTool->processCursorMovement(nullptr, &cLocation);
            }, [this](IWorldEditorToolInstance* tool) {
              tool->renderIcon(cScreenYaw.getValue());
            }),
            cProxyScreen(nullptr) {
    cDefAnalogueSensitivity = 10;
    cPaletteSelectionX.init(0.0f);
    resetEditingView();
    cLocation.x = -1.0f;
    IWorldEditorTool* mDefaultTool = world.getSpindizzy().getDefaultWorldEditorTool();
    cToolbar.selectInstance(mDefaultTool);

    // TODO: I think 'assets' is wrong... should be local assets, not the whole project.
    cScreenYawNotifier   = assets.add(&cScreenYaw,   "EditorYaw",   "System");
    cScreenPitchNotifier = assets.add(&cScreenPitch, "EditorPitch", "System");
    cProxyScreen         = assets.add(static_cast<IScreen*>(this), "Editor", "External"); // TODO: Should have a unique name in case multple instances
  }

  void WorldEditor::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cScreenYaw,                 relinquish);
    assets.remove(&cScreenPitch,               relinquish);
    assets.remove(static_cast<IScreen*>(this), relinquish);
  }

  bool WorldEditor::isMovingNorth() {
    if (isCursorLocked()) {
      return false;
    }
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveLeft.get()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveUp.get()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveRight.get()
         :                                                     cActiveDown.get();
  }

  bool WorldEditor::isMovingEast() {
    if (isCursorLocked()) {
      return false;
    }
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveUp.get()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveRight.get()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveDown.get()
         :                                                     cActiveLeft.get();
  }

  bool WorldEditor::isMovingSouth() {
    if (isCursorLocked()) {
      return false;
    }
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveRight.get()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveDown.get()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveLeft.get()
         :                                                     cActiveUp.get();
  }

  bool WorldEditor::isMovingWest() {
    if (isCursorLocked()) {
      return false;
    }
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveDown.get()
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveLeft.get()
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveUp.get()
         :                                                     cActiveRight.get();
  }

  float WorldEditor::getMovementSpeed() {
    return isCursorLocked()                       ? 0.0f
         : cActiveSlow.get() && cActiveFast.get() ? SPEED_NORMAL
         : cActiveFast.get()                      ? SPEED_FAST
         : cActiveSlow.get()                      ? SPEED_SLOW
         :                                          SPEED_NORMAL;
  }

  void WorldEditor::resetEditingView() {
    cXDirection = 0;
    cYDirection = 0;
    cZDirection = 0;
    cLocation.x = 0.0;
    cLocation.y = 0.0;
    cLocation.z = 0.0;
    cMomentum.x = 0.0;
    cMomentum.y = 0.0;
    cMomentum.z = 0.0;
    cDistance   = -20.0;
    cRotation   = 315.0;
    cTilt       = -50.0;
    cRoll       = 0.0;
  }

  float WorldEditor::getAngle() {
    float mAngle = cRotation;
    if (mAngle > 180.0f) {
      mAngle -= 360.0f;
    }
    return mAngle;
  }

  void WorldEditor::setNextTheme() {
    Zone* mZone = cWorld.getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setNextTheme();
    } else {
      cWorld.getSpindizzy().setNextTheme();
    }
  }

  void WorldEditor::setPreviousTheme() {
    Zone* mZone = cWorld.getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setPreviousTheme();
    } else {
      cWorld.getSpindizzy().setPreviousTheme();
    }
  }

  TerrainBrush& WorldEditor::getTerrainBrush() {
    return cTerrainBrush;
  }

  const TerrainBrush& WorldEditor::getTerrainBrush() const {
    return cTerrainBrush;
  }

  WorldEditorCursorCell WorldEditor::getCursorCell() {
    return WorldEditorCursorCell(std::round(cLocation.x), std::round(cLocation.y), std::round(cLocation.z));
  }

  LiteralVertex& WorldEditor::getCursorLocation() {
    return cLocation;
  }

  float WorldEditor::getCursorX() const {
    return cLocation.x;
  }

  float WorldEditor::getCursorY() const {
    return cLocation.y;
  }

  float WorldEditor::getCursorZ() const {
    return cLocation.z;
  }
  
  void WorldEditor::removeTool(IWorldEditorTool* tool) {
    cToolbar.removeTool(tool);
  }
  
  IFont* WorldEditor::getFont() const {
    return cFont;
  }

  float WorldEditor::getFontSize() const {
    return cFontSize;
  }

  bool WorldEditor::input(sf::Event& event) {
    if (cHasFocus) {
      if (cToolbar.input(event)) {
        return true;
      } else switch (event.type) {
        case sf::Event::MouseMoved: {
          if (cRotatingView.get()) {
            rotate(event.mouseMove.x - cPreviousX, event.mouseMove.y - cPreviousY);
          } else if (cZoomingView.get()) {
            move(cPreviousY - event.mouseMove.y);
          }
          cPreviousX = event.mouseMove.x;
          cPreviousY = event.mouseMove.y;
          return true;
        }

        default: {
          break;
        }
      }
    }
    return false;
  }

  void WorldEditor::resetInput() {
    // Nothing to do.
  }

  void WorldEditor::updateScreen(unsigned int milliseconds) {
    for (std::pair<std::string, EditorDigitalInput<WorldEditor, SignalInputID>*> mPair : cDigitalInputsByName) {
      if (mPair.second->triggerOnChange()) {
        cSignalConsumed = true;
      }
    }
    cSignalConsumed = false;
    rotate(cYawSpeed.get() * 5.0f, -cPitchSpeed.get() * 5.0f);
    if (isMovingWest())      {cMomentum.x -= getMovementSpeed(); cXDirection = -1;}
    if (isMovingEast())      {cMomentum.x += getMovementSpeed(); cXDirection =  1;}
    if (isMovingSouth())     {cMomentum.y -= getMovementSpeed(); cYDirection = -1;}
    if (isMovingNorth())     {cMomentum.y += getMovementSpeed(); cYDirection =  1;}
    if (cActiveLower.get())  {cMomentum.z -= getMovementSpeed(); cZDirection = -1;}
    if (cActiveHigher.get()) {cMomentum.z += getMovementSpeed(); cZDirection =  1;}
    cMomentum.x *= 0.5f;
    cMomentum.y *= 0.5f;
    cMomentum.z *= 0.5f;
    double mMovementDirection = atan2(cYSpeed.get(), cXSpeed.get()) + 90.0f * (M_PI / 180.f);
    double mMovementSpeed = isCursorLocked() ? 0.0f : Utils::distance(0.0, 0.0, cXSpeed.get() * 0.15, cYSpeed.get() * 0.15);
    double mXSpeed = std::sin(cRotation * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    double mYSpeed = std::cos(cRotation * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    LiteralVertex mPreviousLocation = cLocation;
    move(cMomentum.x + mXSpeed, cMomentum.y + mYSpeed, cMomentum.z + cZSpeed.get());
    move(cDistanceInSpeed.get() - cDistanceOutSpeed.get());

    if (isCursorLocked() || (std::abs(cMomentum.x) < STOP_THRESHOLD && !cActiveLeft.get() && !cActiveRight.get())) {
      cMomentum.x = 0.0;
      cXDirection = 0;
    }
    if (isCursorLocked() || (std::abs(cMomentum.y) < STOP_THRESHOLD && !cActiveUp.get() && !cActiveDown.get())) {
      cMomentum.y = 0.0;
      cYDirection = 0;
    }
    if (isCursorLocked() || (std::abs(cMomentum.z) < STOP_THRESHOLD && !cActiveHigher.get() && !cActiveLower.get())) {
      cMomentum.z = 0.0;
      cZDirection = 0;
    }

    if (cSelectedTool != nullptr) {
      cSelectedTool->processCursorMovement(&mPreviousLocation, &cLocation);
      cSelectedTool->updateUI(milliseconds);
    }

    Zone* mZone = cWorld.getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setDefaultTheme();
    }

    cPaletteSelectionX.update(milliseconds);
    cWorld.updateEditing(milliseconds);
    cTerrainBrush.update(milliseconds, cScreenYaw.getValue(), cXSpeed.get(), cYSpeed.get());
  }

  void WorldEditor::renderScreen(float scale, float aspectRatio) const {
    cWorld.getSpindizzy().applyDefaultThemes();
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(45.0, aspectRatio, 0.3, 800.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, cDistance);
    glRotatef(cTilt,     1.0, 0.0, 0.0);
    glRotatef(cRotation, 0.0, 0.0, 1.0);
    glTranslatef(-cLocation.x, -cLocation.y, -cLocation.z * 0.5f);

    glBegin(GL_QUADS);
    for (int x = cLocation.getX() - 64; x <= cLocation.getX() + 64; x++) {
      for (int y = cLocation.getY() - 64; y <= cLocation.getY() + 64; y++) {
        double mDistance = Utils::distance(cLocation.getX(), cLocation.getY(), x, y);
        float mShade = -(mDistance / 64.0f) + 1.0f;
        int mWest = std::floor((cLocation.x + 0.5f) / 8.0f) * 8;
        int mEast = mWest + 7;
        int mSouth = std::floor((cLocation.y + 0.5f) / 8.0f) * 8;
        int mNorth = mSouth + 7;
        if (x >= mWest && x <= mEast && y >= mSouth && y <= mNorth) {
          if ((x + y) % 2 == 0) {
            glColor3f(0.2f * mShade, 0.4f * mShade, 0.6f * mShade);
          } else {
            glColor3f(0.175f * mShade, 0.35f * mShade, 0.5f * mShade);
          }
        } else {
          if ((x + y) % 2 == 0) {
            glColor3f(0.4f * mShade, 0.4f * mShade, 0.4f * mShade);
          } else {
            glColor3f(0.35f * mShade, 0.35f * mShade, 0.35f * mShade);
          }
        }
        glVertex3f(x - 0.5f, y - 0.5f, std::round(cLocation.getZ() * 0.5f / 8.0f) * 8.0f - 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, std::round(cLocation.getZ() * 0.5f / 8.0f) * 8.0f - 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, std::round(cLocation.getZ() * 0.5f / 8.0f) * 8.0f - 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, std::round(cLocation.getZ() * 0.5f / 8.0f) * 8.0f - 0.5f);
      }
    }
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    cWorld.renderEditing(this);

    glLineWidth(1.0);
    glPushMatrix();
    if (cSelectedTool != nullptr) {
      cSelectedTool->renderEditingPreview();
    } else {
      glTranslatef(cLocation.x, cLocation.y, cLocation.z * 0.5f);
      glBegin(GL_LINES);
      glColor3f(0.5f, 1.0f, 0.5f);
      Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
      glEnd();
    }
    glPopMatrix();
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.5f, 1.0f, 0.5f);
    glVertex3f(cLocation.x, cLocation.y, (cLocation.z) * 0.5f);
    glVertex3f(cLocation.x, cLocation.y, -1.0f         * 0.5f);
    glEnd();
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(cLocation.x, cLocation.y, (cLocation.z) * 0.5f);
    glVertex3f(cLocation.x, cLocation.y, -1.0f         * 0.5f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    for (IVisualElement* mEditingVisual : cEditingVisuals) {
      mEditingVisual->render();
    }
    
    cToolbar.render(aspectRatio);

    if (cSelectedTool != nullptr) {
      cSelectedTool->renderUI(aspectRatio);
    }

    glLoadIdentity();
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
  }

  bool WorldEditor::renderAssetIcon() const {
    return false;
  }

  void WorldEditor::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> WorldEditor::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool WorldEditor::isDefaultConfiguration() const {
    return true;
  }

  void WorldEditor::notifyVisible() {
    cWorld.getSpindizzy().setAllThemesInUse(true);
  }
  
  void WorldEditor::notifyHidden() {
    cWorld.getSpindizzy().setAllThemesInUse(false);
  }

  void WorldEditor::notifyLostFocus() {
    cHasFocus = false;
  }

  void WorldEditor::notifyGainedFocus() {
    cHasFocus = true;
  }

  std::vector<std::string> WorldEditor::getDigitalInputs() const {
    std::vector<std::string> mDigitalInputNames;
    for (std::pair<std::string, EditorDigitalInput<WorldEditor, SignalInputID>*> mPair : cDigitalInputsByName) {
      mDigitalInputNames.emplace_back(mPair.first);
    }
    return mDigitalInputNames;
  }

  std::vector<std::string> WorldEditor::getAnalogueInputs() const {
    std::vector<std::string> mAnalogueInputNames;
    for (std::pair<std::string, EditorAnalogueInput*> mPair : cAnalogueInputsByName) {
      mAnalogueInputNames.emplace_back(mPair.first);
    }
    return mAnalogueInputNames;
  }

  void WorldEditor::setDigitalInput(const std::string& name, IBoolean* input) {
    cDigitalInputsByName.find(name)->second->set(input);
  }

  void WorldEditor::setAnalogueInput(const std::string& name, IFloat* input) {
    cAnalogueInputsByName.find(name)->second->set(input);
  }

  void WorldEditor::setExitAction(ActionExecutor* action) {
    cExitAction = action;
  }

  bool WorldEditor::signal(SignalInputID id) {
    if (cHasFocus) {
      if (!cSignalConsumed) {
        if (cSelectedTool != nullptr) {
          if (cSelectedTool->inputTool(id, getAngle())) {
            return true;
          }
        }

        switch (id) {
          case SignalInputID::NEXT_THEME:     setNextTheme();                                       return true;
          case SignalInputID::PREVIOUS_THEME: setPreviousTheme();                                   return true;
          case SignalInputID::EXIT:           if (cExitAction != nullptr) {cExitAction->execute();} return true;
          default:                                                                                  break;
        }
      }
    }
    return false;
  }

  void WorldEditor::setAppearance(IFont* font, float scale) {
    cFont = font;
    cFontSize = scale;
  }

  const IFloat* WorldEditor::getYaw() const {
    return &cScreenYaw;
  }

  const IFloat* WorldEditor::getPitch() const {
    return &cScreenPitch;
  }

  void WorldEditor::move(float amount) {
    cDistance += amount * 0.3f;
    if (cDistance > -5.0f) {
      cDistance = -5.0f;
    } else if (cDistance < -500.0f) {
      cDistance = -500.0f;
    }
  }

  void WorldEditor::rotate(float rotate, float tilt) {
    cTilt += tilt * 0.3f;
    cRotation += rotate * 0.3f;
    if (cRotation >= 360.0) {
      cRotation -= 360.0;
    } else if (cRotation < 0.0) {
      cRotation += 360.0;
    }
    if (cTilt < -90.0f) {
      cTilt = -90.0f;
    } else if (cTilt > 0.0f) {
      cTilt = 0.0f;
    }
    if (rotate != 0.0f) {
      cScreenYawNotifier->stateChanged(&cScreenYaw);
    }
    if (tilt != 0.0f) {
      cScreenPitchNotifier->stateChanged(&cScreenPitch);
    }
  }

  bool WorldEditor::isCursorLocked() const {
    return !cHasFocus || (cSelectedTool != nullptr ? cSelectedTool->isCursorLocked() : false);
  }

  void WorldEditor::move(float x, float y, float z) {
    LiteralVertex mNewLocation;
    mNewLocation.x = std::clamp(cLocation.x + x, static_cast<double>(cWorld.getSpindizzy().getEditorMinX()), static_cast<double>(cWorld.getSpindizzy().getEditorMaxX()));
    mNewLocation.y = std::clamp(cLocation.y + y, static_cast<double>(cWorld.getSpindizzy().getEditorMinY()), static_cast<double>(cWorld.getSpindizzy().getEditorMaxY()));
    mNewLocation.z = std::clamp(cLocation.z + z, static_cast<double>(cWorld.getSpindizzy().getEditorMinZ()), static_cast<double>(cWorld.getSpindizzy().getEditorMaxZ()));
    double mSnapInterval = cSelectedTool != nullptr ? cSelectedTool->getSnapInterval() : 1.0;
    if (isCursorLocked() || (std::abs(x) < STOP_THRESHOLD && !cActiveLeft.get() && !cActiveRight.get())) {
      mNewLocation.x = Utils::round(mNewLocation.x, mSnapInterval, cXDirection);
    }
    if (isCursorLocked() || (std::abs(y) < STOP_THRESHOLD && !cActiveUp.get() && !cActiveDown.get())) {
      mNewLocation.y = Utils::round(mNewLocation.y, mSnapInterval, cYDirection);
    }
    if (isCursorLocked() || (std::abs(z) < STOP_THRESHOLD && !cActiveHigher.get() && !cActiveLower.get())) {
      mNewLocation.z = Utils::round(mNewLocation.z, mSnapInterval, cZDirection);
    }
    cLocation = mNewLocation;
  }

  World& WorldEditor::getWorld() const {
    return cWorld;
  }

  WorldEditor::ScreenFloat::ScreenFloat(double* value) :
            cValue(value) {
  }

  float WorldEditor::ScreenFloat::getValue() const {
    return *cValue;
  }

  bool WorldEditor::ScreenFloat::renderAssetIcon() const {
    return false;
  }

  void WorldEditor::ScreenFloat::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> WorldEditor::ScreenFloat::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool WorldEditor::ScreenFloat::isDefaultConfiguration() const {
    return true;
  }

  IScreen* WorldEditor::screen() {
    return cProxyScreen;
  }
}

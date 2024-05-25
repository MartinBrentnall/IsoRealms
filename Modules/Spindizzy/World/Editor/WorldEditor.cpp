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
  
  WorldEditor::WorldEditor(IAssetRegistry* assets, World* world) :
            cScreenYaw(&cRotation),
            cScreenPitch(&cTilt),
            cHatHandler(world->getSpindizzy()->getProject()->getApplication()->getHatHandler()),
            cRotatingView(false),
            cZoomingView(false),
            cPreviousX(0),
            cPreviousY(0),
            cProxyScreen(nullptr) {
    cActiveFast           = false;
    cActiveSlow           = false;
    cActiveLeft           = false;
    cActiveRight          = false;
    cActiveUp             = false;
    cActiveDown           = false;
    cActiveHigher         = false;
    cActiveLower          = false;
    cWorld                = world;
    cYawSpeed             = 0.0f;
    cPitchSpeed           = 0.0f;
    cXSpeed               = 0.0f;
    cYSpeed               = 0.0f;
    cZSpeed               = 0.0f;
    cDistanceInSpeed      = 0.0f;
    cDistanceOutSpeed     = 0.0f;
    cDefAnalogueSensitivity = 10;
    cPaletteSelectionX.init(0.0f);
    resetEditingView();
    cLocation.x = -1.0f;
    cTools = world->getSpindizzy()->createToolSet(this);
    IWorldEditorTool* mDefaultTool = world->getSpindizzy()->getDefaultWorldEditorTool();
    for (IWorldEditorToolInstance* mTool : cTools) {
      if (mTool->isTool(mDefaultTool)) {
        cSelectedTool = mTool;
        break;
      }
    }

    // TODO: I think 'assets' is wrong... should be local assets, not the whole project.
    cScreenYawNotifier   = assets->add(&cScreenYaw,   "EditorYaw",   "System");
    cScreenPitchNotifier = assets->add(&cScreenPitch, "EditorPitch", "System");
    cProxyScreen         = assets->add(static_cast<IScreen*>(this), "Editor", "External"); // TODO: Should have a unique name in case multple instances
    selectToolRelative(0);
  }

  void WorldEditor::unregisterAssets(IAssetRemover* assets) {
    assets->remove(&cScreenYaw);
    assets->remove(&cScreenPitch);
    assets->remove(static_cast<IScreen*>(this));
  }

  bool WorldEditor::isMovingNorth() {
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveLeft
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveUp
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveRight
         :                                                     cActiveDown;
  }

  bool WorldEditor::isMovingEast() {
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveUp
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveRight
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveDown
         :                                                     cActiveLeft;
  }

  bool WorldEditor::isMovingSouth() {
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveRight
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveDown
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveLeft
         :                                                     cActiveUp;
  }

  bool WorldEditor::isMovingWest() {
    float mCameraAngle = getAngle();
    return mCameraAngle >=   40.0f && mCameraAngle <= 130.0f ? cActiveDown
         : mCameraAngle >=  -50.0f && mCameraAngle <=  40.0f ? cActiveLeft
         : mCameraAngle >= -140.0f && mCameraAngle <= -50.0f ? cActiveUp
         :                                                     cActiveRight;
  }

  float WorldEditor::getMovementSpeed() {
    return cActiveSlow && cActiveFast ? SPEED_NORMAL
         : cActiveFast                ? SPEED_FAST
         : cActiveSlow                ? SPEED_SLOW
         :                              SPEED_NORMAL;
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

  void WorldEditor::selectToolRelative(int amount) {
    int mSelectedToolIndex = -1;
    if (cSelectedTool == nullptr) {
      mSelectedToolIndex = amount > 0 ? 0 : cTools.size() - 1;
      cPaletteSelectionX.init(mSelectedToolIndex);
    } else {
      for (unsigned int i = 0; i < cTools.size(); i++) {
        if (cTools[i] == cSelectedTool) {
          mSelectedToolIndex = i + amount;
          while (mSelectedToolIndex < 0) {
            mSelectedToolIndex += cTools.size();
          }
          while (mSelectedToolIndex >= static_cast<int>(cTools.size())) {
            mSelectedToolIndex -= cTools.size();
          }
          break;
        }
      }
    }
    cPaletteSelectionX = mSelectedToolIndex;

    if (cSelectedTool != nullptr) {
      cSelectedTool->processCursorMovement(&cLocation, nullptr);
    }
    cSelectedTool = cTools[mSelectedToolIndex];
    cSelectedTool->processCursorMovement(nullptr, &cLocation);

//    float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
//    float mPaletteScrollRange = std::max(0.0f, mPaletteWidth - PALETTE_SPACE);
//    cPaletteScroll = std::clamp(LEFT_BORDER + mSelectedToolIndex * (ICON_WIDTH + ICON_SPACING) + ICON_WIDTH * 0.5f, 0.0f, mPaletteScrollRange);
  }

  void WorldEditor::setNextTheme() {
    Zone* mZone = cWorld->getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setNextTheme();
    } else {
      cWorld->getSpindizzy()->setNextTheme();
    }
  }

  void WorldEditor::setPreviousTheme() {
    Zone* mZone = cWorld->getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setPreviousTheme();
    } else {
      cWorld->getSpindizzy()->setPreviousTheme();
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
  
  IPropertyAppearance* WorldEditor::getPropertyAppearance() {
    return &cPropertyAppearance;
  }

  bool WorldEditor::input(sf::Event& event) {
//     if (cSelectedTool != nullptr) {
//       if (cSelectedTool->inputEdit(event, getAngle())) {
//         return true;
//       }
//     }
//
//     switch (event.type) {
//       case sf::Event::JoystickButtonPressed: {
//         switch (event.joystickButton.button) {
//           case 4: cZSpeed = -32767 / 200000.0f; return true;
//           case 5: cZSpeed =  32767 / 200000.0f; return true;
//         }
//         break;
//       }
//
//       case sf::Event::JoystickButtonReleased: {
//         switch (event.joystickButton.button) {
//           case 4: cZSpeed = 0.0f; return true;
//           case 5: cZSpeed = 0.0f; return true;
//         }
//         break;
//       }
//
//       case sf::Event::JoystickMoved: {
//         bool mDirectionPressed = false;
//         if (cHatHandler.leftPressed())  {selectToolRelative(-1); mDirectionPressed = true;}
//         if (cHatHandler.rightPressed()) {selectToolRelative(1);  mDirectionPressed = true;}
//         if (cHatHandler.upPressed())    {setPreviousTheme();     mDirectionPressed = true;}
//         if (cHatHandler.downPressed())  {setNextTheme();         mDirectionPressed = true;}
//         if (mDirectionPressed)          {return true;}
//         int mValue = std::abs(event.joystickMove.position) < cDefAnalogueSensitivity ? 0 : (event.joystickMove.position - (event.joystickMove.position < 0 ? -cDefAnalogueSensitivity : cDefAnalogueSensitivity)) * (32767 / static_cast<float>(32767 - cDefAnalogueSensitivity));
//         switch (event.joystickMove.axis) {
//           case sf::Joystick::Axis::X: cXSpeed           =  mValue / 500.0f; return true;
//           case sf::Joystick::Axis::Y: cYSpeed           = -mValue / 500.0f; return true;
//           case sf::Joystick::Axis::Z: cDistanceOutSpeed =  mValue / 50.0f;  return true;
//           case sf::Joystick::Axis::U: cYawSpeed         =  mValue / 9.0f;   return true;
//           case sf::Joystick::Axis::V: cPitchSpeed       = -mValue / 9.0f;   return true;
//           case sf::Joystick::Axis::R: cDistanceInSpeed  =  mValue / 50.0f;  return true;
//           default:                                                          break;
//         }
//         break;
//       }
//
//       case sf::Event::KeyPressed: {
//         switch (event.key.code) {
//           case sf::Keyboard::LShift:   cActiveSlow   = true;   return true;
//           case sf::Keyboard::LControl: cActiveFast   = true;   return true;
//           case sf::Keyboard::A:        // Fallthrough
//           case sf::Keyboard::Left:     cActiveLeft   = true;   return true;
//           case sf::Keyboard::W:        // Fallthrough
//           case sf::Keyboard::Up:       cActiveUp     = true;   return true;
//           case sf::Keyboard::S:        // Fallthrough
//           case sf::Keyboard::Down:     cActiveDown   = true;   return true;
//           case sf::Keyboard::D:        // Fallthrough
//           case sf::Keyboard::Right:    cActiveRight  = true;   return true;
//           case sf::Keyboard::PageUp:   cActiveHigher = true;   return true;
//           case sf::Keyboard::PageDown: cActiveLower  = true;   return true;
//           case sf::Keyboard::Home:     setNextTheme();         return true;
//           case sf::Keyboard::End:      setPreviousTheme();     return true;
//           case sf::Keyboard::Q:        // Fallthrough
//           case sf::Keyboard::F1:       selectToolRelative(-1); return true;
//           case sf::Keyboard::E:        // Fallthrough
//           case sf::Keyboard::F2:       selectToolRelative(1);  return true;
//           default:                                             break;
//         }
//         break;
//       }
//
//       case sf::Event::KeyReleased: {
//         switch (event.key.code) {
//           case sf::Keyboard::LShift:   cActiveSlow   = false; return true;
//           case sf::Keyboard::LControl: cActiveFast   = false; return true;
//           case sf::Keyboard::A:        // Fallthrough
//           case sf::Keyboard::Left:     cActiveLeft   = false; return true;
//           case sf::Keyboard::D:        // Fallthrough
//           case sf::Keyboard::Right:    cActiveRight  = false; return true;
//           case sf::Keyboard::W:        // Fallthrough
//           case sf::Keyboard::Up:       cActiveUp     = false; return true;
//           case sf::Keyboard::S:        // Fallthrough
//           case sf::Keyboard::Down:     cActiveDown   = false; return true;
//           case sf::Keyboard::PageUp:   cActiveHigher = false; return true;
//           case sf::Keyboard::PageDown: cActiveLower  = false; return true;
//           default:                                            break;
//         }
//         break;
//       }
//
//       case sf::Event::MouseButtonReleased: {
//         switch (event.mouseButton.button) {
//           case sf::Mouse::Left: {
//             cRotatingView = false;
//             return true;
//           }
//
//           case sf::Mouse::Right: {
//             cZoomingView = false;
//             return true;
//           }
//
//           default: {
//             break;
//           }
//         }
//       }
//
//       case sf::Event::MouseButtonPressed: {
//         switch (event.mouseButton.button) {
//           case sf::Mouse::Left: {
// //             IApplication* mApplication = cWorld->getSpindizzy()->getProject()->getApplication();
// //             ScreenLocation mLocation = mApplication->normalise(event.mouseButton.x, event.mouseButton.y);
// //             float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
// //             if (mLocation.getY() >= BOTTOM_BORDER && mLocation.getY() <= BOTTOM_BORDER + ICON_HEIGHT) {
// //               for (unsigned int i = 0; i < cTools.size(); i++) {
// //                 float mIconLeft = mPaletteWidth < PALETTE_SPACE ? (-mPaletteWidth * 0.5f) + i * (ICON_WIDTH + ICON_SPACING)
// //                                                                 : (LEFT_BORDER + i * (ICON_WIDTH + ICON_SPACING));
// //                 float mIconRight = mIconLeft + ICON_WIDTH;
// //                 if (mLocation.getX() >= mIconLeft && mLocation.getX() <= mIconRight) {
// //                   if (cSelectedTool == nullptr) {
// //                     cPaletteSelectionX.init(i);
// //                   } else {
// //                     cPaletteSelectionX = i;
// //                   }
// //                   if (cSelectedTool != nullptr) {
// //                     cSelectedTool->processCursorMovement(&cLocation, nullptr);
// //                   }
// //                   cSelectedTool = cTools[i];
// //                   cSelectedTool->processCursorMovement(nullptr, &cLocation);
// //                   break;
// //                 }
// //               }
// //             } else {
//               cRotatingView = true;
//               cPreviousX = event.mouseButton.x;
//               cPreviousY = event.mouseButton.y;
// //             }
//             return true;
//           }
//
//           case sf::Mouse::Right: {
//             cZoomingView = true;
//             cPreviousX = event.mouseButton.x;
//             cPreviousY = event.mouseButton.y;
//             return true;
//           }
//
//           default: {
//             break;
//           }
//         }
//         break;
//       }
//
//       case sf::Event::MouseWheelScrolled: {
//         switch (event.mouseWheelScroll.wheel) {
//           case sf::Mouse::VerticalWheel: {
//             if (event.mouseWheelScroll.delta > 0) {
//               selectToolRelative(-1);
//             } else {
//               selectToolRelative(1);
//             }
//             return true;
//           }
//
//           default: {
//             break;
//           }
//         }
//         break;
//       }
//
//       case sf::Event::MouseMoved: {
//         if (cRotatingView) {
//           rotate(event.mouseMove.x - cPreviousX, event.mouseMove.y - cPreviousY);
//         } else if (cZoomingView) {
//           move(cPreviousY - event.mouseMove.y);
//         }
//         cPreviousX = event.mouseMove.x;
//         cPreviousY = event.mouseMove.y;
//         return true;
//       }
//
//       default: {
//         break;
//       }
//     }
    return false;
  }

  void WorldEditor::updateScreen(unsigned int milliseconds) {
    rotate(cYawSpeed, cPitchSpeed);
    if (isMovingWest())  {cMomentum.x -= getMovementSpeed(); cXDirection = -1;}
    if (isMovingEast())  {cMomentum.x += getMovementSpeed(); cXDirection =  1;}
    if (isMovingSouth()) {cMomentum.y -= getMovementSpeed(); cYDirection = -1;}
    if (isMovingNorth()) {cMomentum.y += getMovementSpeed(); cYDirection =  1;}
    if (cActiveLower)    {cMomentum.z -= getMovementSpeed(); cZDirection = -1;}
    if (cActiveHigher)   {cMomentum.z += getMovementSpeed(); cZDirection =  1;}
    cMomentum.x *= 0.5f;
    cMomentum.y *= 0.5f;
    cMomentum.z *= 0.5f;
    double mMovementDirection = atan2(-cYSpeed, cXSpeed) + 90.0f * (M_PI / 180.f);
    double mMovementSpeed = Utils::distance(0.0, 0.0, cXSpeed, cYSpeed);
    double mXSpeed = std::sin(cRotation * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    double mYSpeed = std::cos(cRotation * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    LiteralVertex mPreviousLocation = cLocation;
    move(cMomentum.x + mXSpeed, cMomentum.y + mYSpeed, cMomentum.z + cZSpeed);
    move(cDistanceInSpeed - cDistanceOutSpeed);

    if (std::abs(cMomentum.x) < STOP_THRESHOLD && !cActiveLeft && !cActiveRight) {
      cMomentum.x = 0.0;
      cXDirection = 0;
    }
    if (std::abs(cMomentum.y) < STOP_THRESHOLD && !cActiveUp && !cActiveDown) {
      cMomentum.y = 0.0;
      cYDirection = 0;
    }
    if (std::abs(cMomentum.z) < STOP_THRESHOLD && !cActiveHigher && !cActiveLower) {
      cMomentum.z = 0.0;
      cZDirection = 0;
    }

    if (cSelectedTool != nullptr) {
      cSelectedTool->processCursorMovement(&mPreviousLocation, &cLocation);
      cSelectedTool->updateUI(milliseconds);
    }

    Zone* mZone = cWorld->getZone(getCursorCell());
    if (mZone != nullptr) {
      mZone->setDefaultTheme();
    }

    cPaletteSelectionX.update(milliseconds);
    cWorld->updateEditing(milliseconds);
    cTerrainBrush.update(milliseconds, cScreenYaw.getValue());
  }

  void WorldEditor::renderScreen(float scale, float aspectRatio) const {
    cWorld->getSpindizzy()->applyDefaultThemes();
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
    cWorld->renderEditing(this);

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

    // Render UI
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);
    glLoadIdentity();

    // Render world selection highlight.
    float mCursorXPosition = cPaletteSelectionX.animation() * (ICON_WIDTH + ICON_SPACING);
    float mPaletteWidth = cTools.size() * (ICON_WIDTH + ICON_SPACING) - ICON_SPACING;
    float mDifference = aspectRatio;
    float mScrollAmount = std::min((mPaletteWidth - mDifference) - (ICON_WIDTH / 2.0f - BORDER_SPACING),  std::max(mDifference - (ICON_WIDTH / 2.0f + BORDER_SPACING), mCursorXPosition));
    glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
    glBegin(GL_QUADS);
    Utils::renderRectangle(-1.0f, -1.0f, 1.0f, -1.0f + ICON_HEIGHT + BORDER_SPACING * 2.0f);
    glEnd();
    glScalef(1.0f / aspectRatio, 1.0f, 1.0f);
    glTranslatef(-mScrollAmount, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderCurve(mCursorXPosition - ICON_WIDTH * 0.5f, BOTTOM_BORDER + ICON_HEIGHT, 0.01f, 0.0f,  0.25f);
    Utils::renderCurve(mCursorXPosition - ICON_WIDTH * 0.5f, BOTTOM_BORDER,               0.01f, 0.25f, 0.5f);
    Utils::renderCurve(mCursorXPosition + ICON_WIDTH * 0.5f, BOTTOM_BORDER + ICON_HEIGHT, 0.01f, 0.75f, 1.0f);
    Utils::renderCurve(mCursorXPosition + ICON_WIDTH * 0.5f, BOTTOM_BORDER,               0.01f, 0.5f,  0.75f);
    glBegin(GL_QUADS);
    Utils::renderRectangle(mCursorXPosition -  ICON_WIDTH * 0.5f,          BOTTOM_BORDER - 0.01f,       mCursorXPosition +  ICON_WIDTH * 0.5f,                         BOTTOM_BORDER);
    Utils::renderRectangle(mCursorXPosition -  ICON_WIDTH * 0.5f,          BOTTOM_BORDER + ICON_HEIGHT, mCursorXPosition +  ICON_WIDTH * 0.5f,                         BOTTOM_BORDER + ICON_HEIGHT + 0.01f);
    Utils::renderRectangle(mCursorXPosition - (ICON_WIDTH * 0.5f + 0.01f), BOTTOM_BORDER,               mCursorXPosition + (ICON_WIDTH * 0.5f + 0.01f), BOTTOM_BORDER + ICON_HEIGHT);
    glEnd();
    glPopMatrix();

    for (unsigned int i = 0; i < cTools.size(); i++) {
      glPushMatrix();
//       if (mPaletteWidth < PALETTE_SPACE) {
//         glTranslatef(((-mPaletteWidth * 0.5f + ICON_WIDTH * 0.5f) + i * ICON_WIDTH) + i * ICON_SPACING, BOTTOM_BORDER + ICON_HEIGHT * 0.5f, 0.0f);
//       } else {
        glTranslatef(i * (ICON_WIDTH + ICON_SPACING) - mScrollAmount, BOTTOM_BORDER + ICON_HEIGHT * 0.5f, 0.0f);
//       }
      glScalef(ICON_WIDTH * 0.5f, ICON_HEIGHT * 0.5f, 0.0f);
      cTools[i]->renderIcon(cScreenYaw.getValue());
      glPopMatrix();
    }
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

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

  void WorldEditor::notifyVisible() {
    cWorld->getSpindizzy()->setAllThemesInUse(true);
  }
  
  void WorldEditor::notifyHidden() {
    cWorld->getSpindizzy()->setAllThemesInUse(false);
  }

  void WorldEditor::notifyLostFocus() {
    cActiveLeft = false;
    cActiveRight = false;
    cActiveDown = false;
    cActiveUp = false;
    cActiveLower = false;
    cActiveHigher = false;
    cActiveSlow = false;
    cActiveFast = false;
    cXSpeed = 0.0f;
    cYSpeed = 0.0f;
    cZSpeed = 0.0f;
    cYawSpeed = 0.0f;
    cPitchSpeed = 0.0f;
    cDistanceInSpeed = 0.0f;
    cDistanceOutSpeed = 0.0f;
  }
  
  std::vector<std::string> WorldEditor::getDigitalInputs() const {
    std::vector<std::string> mDigitalInputNames;
    for (std::pair<std::string, DigitalInputID> mPair : cDigitalInputsByName) {
      mDigitalInputNames.emplace_back(mPair.first);
    }
    return mDigitalInputNames;
  }

  int WorldEditor::getDigitalInputID(const std::string& name) const {
    return static_cast<std::underlying_type_t<DigitalInputID>>(cDigitalInputsByName.find(name)->second);
  }

  void WorldEditor::inputEditable(int id, bool value) {
    if (cSelectedTool != nullptr) {
      if (cSelectedTool->inputTool(id, value, getAngle())) {
        return;
      }
    }

    if (value) {
      switch (static_cast<DigitalInputID>(id)) {
        case DigitalInputID::MOVE_CURSOR_BACKWARD: cActiveDown   = true;   break;
        case DigitalInputID::MOVE_CURSOR_DOWN:     cActiveLower  = true;   break;
        case DigitalInputID::MOVE_CURSOR_FASTER:   cActiveFast   = true;   break;
        case DigitalInputID::MOVE_CURSOR_FORWARD:  cActiveUp     = true;   break;
        case DigitalInputID::MOVE_CURSOR_LEFT:     cActiveLeft   = true;   break;
        case DigitalInputID::MOVE_CURSOR_RIGHT:    cActiveRight  = true;   break;
        case DigitalInputID::MOVE_CURSOR_SLOWER:   cActiveSlow   = true;   break;
        case DigitalInputID::MOVE_CURSOR_UP:       cActiveHigher = true;   break;
        case DigitalInputID::NEXT_THEME:           setNextTheme();         break;
        case DigitalInputID::NEXT_TOOL:            selectToolRelative(1);  break;
        case DigitalInputID::PREVIOUS_THEME:       setPreviousTheme();     break;
        case DigitalInputID::PREVIOUS_TOOL:        selectToolRelative(-1); break;
        default:                                                           break;
      }
    } else {
      switch (static_cast<DigitalInputID>(id)) {
        case DigitalInputID::MOVE_CURSOR_BACKWARD: cActiveDown   = false; break;
        case DigitalInputID::MOVE_CURSOR_DOWN:     cActiveLower  = false; break;
        case DigitalInputID::MOVE_CURSOR_FASTER:   cActiveFast   = false; break;
        case DigitalInputID::MOVE_CURSOR_FORWARD:  cActiveUp     = false; break;
        case DigitalInputID::MOVE_CURSOR_LEFT:     cActiveLeft   = false; break;
        case DigitalInputID::MOVE_CURSOR_RIGHT:    cActiveRight  = false; break;
        case DigitalInputID::MOVE_CURSOR_SLOWER:   cActiveSlow   = false; break;
        case DigitalInputID::MOVE_CURSOR_UP:       cActiveHigher = false; break;
        default:                                   /* Nothing to do. */   break;
      }
    }
  }

  void WorldEditor::setAppearance(IFont* font, float scale) {
    cPropertyAppearance.set(font, scale);
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

  void WorldEditor::move(float x, float y, float z) {
    LiteralVertex mNewLocation;
    mNewLocation.x = std::clamp(cLocation.x + x, static_cast<double>(cWorld->getSpindizzy()->getEditorMinX()), static_cast<double>(cWorld->getSpindizzy()->getEditorMaxX()));
    mNewLocation.y = std::clamp(cLocation.y + y, static_cast<double>(cWorld->getSpindizzy()->getEditorMinY()), static_cast<double>(cWorld->getSpindizzy()->getEditorMaxY()));
    mNewLocation.z = std::clamp(cLocation.z + z, static_cast<double>(cWorld->getSpindizzy()->getEditorMinZ()), static_cast<double>(cWorld->getSpindizzy()->getEditorMaxZ()));
    double mSnapInterval = cSelectedTool != nullptr ? cSelectedTool->getSnapInterval() : 1.0;
    if (std::abs(x) < STOP_THRESHOLD && !cActiveLeft && !cActiveRight) {
      mNewLocation.x = Utils::round(mNewLocation.x, mSnapInterval, cXDirection);
    }
    if (std::abs(y) < STOP_THRESHOLD && !cActiveUp && !cActiveDown) {
      mNewLocation.y = Utils::round(mNewLocation.y, mSnapInterval, cYDirection);
    }
    if (std::abs(z) < STOP_THRESHOLD && !cActiveHigher && !cActiveLower) {
      mNewLocation.z = Utils::round(mNewLocation.z, mSnapInterval, cZDirection);
    }
    cLocation = mNewLocation;
  }

  World* WorldEditor::getWorld() const {
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

  IScreen* WorldEditor::screen() {
    return cProxyScreen;
  }

  const std::map<std::string, WorldEditor::DigitalInputID> WorldEditor::cDigitalInputsByName = {
    {"Cancel",             WorldEditor::DigitalInputID::CANCEL},
    {"ConfigureTool",      WorldEditor::DigitalInputID::CONFIGURE_TOOL},
    {"MoveCursorBackward", WorldEditor::DigitalInputID::MOVE_CURSOR_BACKWARD},
    {"MoveCursorDown",     WorldEditor::DigitalInputID::MOVE_CURSOR_DOWN},
    {"MoveCursorFaster",   WorldEditor::DigitalInputID::MOVE_CURSOR_FASTER},
    {"MoveCursorForward",  WorldEditor::DigitalInputID::MOVE_CURSOR_FORWARD},
    {"MoveCursorLeft",     WorldEditor::DigitalInputID::MOVE_CURSOR_LEFT},
    {"MoveCursorRight",    WorldEditor::DigitalInputID::MOVE_CURSOR_RIGHT},
    {"MoveCursorSlower",   WorldEditor::DigitalInputID::MOVE_CURSOR_SLOWER},
    {"MoveCursorUp",       WorldEditor::DigitalInputID::MOVE_CURSOR_UP},
    {"NextTheme",          WorldEditor::DigitalInputID::NEXT_THEME},
    {"NextTool",           WorldEditor::DigitalInputID::NEXT_TOOL},
    {"PreviousTheme",      WorldEditor::DigitalInputID::PREVIOUS_THEME},
    {"PreviousTool",       WorldEditor::DigitalInputID::PREVIOUS_TOOL},
    {"ToolMode",           WorldEditor::DigitalInputID::TOOL_MODE},
    {"UseTool",            WorldEditor::DigitalInputID::USE_TOOL},
  };
}

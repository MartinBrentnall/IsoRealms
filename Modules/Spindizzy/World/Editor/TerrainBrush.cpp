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
#include "TerrainBrush.h"

#include "Modules/Spindizzy/World/Object/Terrain/Surface.h"
#include "Modules/Spindizzy/World/Object/Terrain/SplitSurface.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"

namespace IsoRealms::Spindizzy {
  TerrainBrush::TerrainBrush() :
            cRuntimeEditing(false),
            cRuntimeCursorX(0.0),
            cRuntimeCursorY(0.0),
            cRuntimeCursorXSpeed(0.0),
            cRuntimeCursorYSpeed(0.0),
            cDefAnalogueSensitivity(5000) {
    reset();
  }

  bool TerrainBrush::isEditing() const {
    return cRuntimeEditing;
  }

  void TerrainBrush::raiseCorner(unsigned int x, unsigned int y) {
    if (cCornerHeight[x][y].value() < 2) {
      cCornerHeight[x][y] += 1.0f;
    }

    while (cCornerHeight[0][0].value() > 0 && cCornerHeight[0][1].value() > 0 && cCornerHeight[1][0].value() > 0 && cCornerHeight[1][1].value() > 0) {
      cCornerHeight[0][0] -= 1.0f;
      cCornerHeight[0][1] -= 1.0f;
      cCornerHeight[1][0] -= 1.0f;
      cCornerHeight[1][1] -= 1.0f;
    }
  }

  void TerrainBrush::lowerCorner(unsigned int x, unsigned int y) {
    if (cCornerHeight[x][y].value() > 0) {
      cCornerHeight[x][y] -= 1.0f;
    }
  }

  bool TerrainBrush::lowerSelected() {
    if (cRuntimeEditing) {
      int mX = std::round(cRuntimeCursorX);
      int mY = std::round(cRuntimeCursorY);
      if (mX == 0 && mY == 0) {
        toggleSplit();
      } else if (mX == 0) {
        if (mY == -1) {mY = 0;}
        lowerCorner(0, mY);
        lowerCorner(1, mY);
      } else if (mY == 0) {
        if (mX == -1) {mX = 0;}
        lowerCorner(mX, 0);
        lowerCorner(mX, 1);
      } else {
        if (mX == -1) {mX = 0;}
        if (mY == -1) {mY = 0;}
        lowerCorner(mX, mY);
      }
      return true;
    }
    return false;
  }

  bool TerrainBrush::raiseSelected() {
    if (cRuntimeEditing) {
      int mX = std::round(cRuntimeCursorX);
      int mY = std::round(cRuntimeCursorY);
      if (mX == 0 && mY == 0) {
        toggleSplit();
      } else if (mX == 0) {
        if (mY == -1) {mY = 0;}
        raiseCorner(0, mY);
        raiseCorner(1, mY);
      } else if (mY == 0) {
        if (mX == -1) {mX = 0;}
        raiseCorner(mX, 0);
        raiseCorner(mX, 1);
      } else {
        if (mX == -1) {mX = 0;}
        if (mY == -1) {mY = 0;}
        raiseCorner(mX, mY);
      }
      return true;
    }
    return false;
  }

  void TerrainBrush::toggleEditing() {
    cRuntimeEditing = !cRuntimeEditing;
  }

  void TerrainBrush::toggleSplit() {
    cAlternativeSplit = !cAlternativeSplit;
  }

  void TerrainBrush::reset() {
    cCornerHeight[0][0].init(0);
    cCornerHeight[0][1].init(0);
    cCornerHeight[1][0].init(0);
    cCornerHeight[1][1].init(0);
    cAlternativeSplit = false;
  }

  int TerrainBrush::getNorthWestHeight() const {
    return cCornerHeight[0][1].value();
  }

  int TerrainBrush::getNorthEastHeight() const {
    return cCornerHeight[1][1].value();
  }

  int TerrainBrush::getSouthWestHeight() const {
    return cCornerHeight[0][0].value();
  }

  int TerrainBrush::getSouthEastHeight() const {
    return cCornerHeight[1][0].value();
  }

  bool TerrainBrush::isAlternativeSplit() const {
    return cAlternativeSplit;
  }

  int TerrainBrush::getXSlope() const {
    return Spindizzy::getTerrainSlope(cCornerHeight[0][1].value(), cCornerHeight[0][0].value(), cCornerHeight[1][1].value(), cCornerHeight[1][0].value());
  }

  int TerrainBrush::getYSlope() const {
    return Spindizzy::getTerrainSlope(cCornerHeight[0][0].value(), cCornerHeight[1][0].value(), cCornerHeight[0][1].value(), cCornerHeight[1][1].value());
  }

  bool TerrainBrush::isSplit() const {
    return getXSlope() == 0 && getYSlope() == 0 && (cCornerHeight[0][0].value() != 0 || cCornerHeight[1][0].value() != 0 || cCornerHeight[0][1].value() != 0 || cCornerHeight[1][1].value() != 0);
  }

  float TerrainBrush::getXSlopeAnimation() const {
    return Spindizzy::getTerrainSlope(cCornerHeight[0][1].animation(), cCornerHeight[0][0].animation(), cCornerHeight[1][1].animation(), cCornerHeight[1][0].animation());
  }

  float TerrainBrush::getYSlopeAnimation() const {
    return Spindizzy::getTerrainSlope(cCornerHeight[0][0].animation(), cCornerHeight[1][0].animation(), cCornerHeight[0][1].animation(), cCornerHeight[1][1].animation());
  }

  bool TerrainBrush::isSplitAnimation() const {
    return getXSlopeAnimation() == 0 && getYSlopeAnimation() == 0 && (cCornerHeight[0][0].animation() != 0 || cCornerHeight[1][0].animation() != 0 || cCornerHeight[0][1].animation() != 0 || cCornerHeight[1][1].animation() != 0);
  }

  void TerrainBrush::renderPreview(const TerrainType* type, float pinnedX, float pinnedY, float pinnedZ, float cursorX, float cursorY, float cursorZ, bool steppedBottom) const {
    float mStartX = std::min(pinnedX, static_cast<float>(cursorX));
    float mStartY = std::min(pinnedY, static_cast<float>(cursorY));
    float mStartZ = std::min(pinnedZ, static_cast<float>(cursorZ)) - 0.001f;
    float mEndX =   std::max(pinnedX, static_cast<float>(cursorX));
    float mEndY =   std::max(pinnedY, static_cast<float>(cursorY));
    float mEndZ =   std::max(pinnedZ, static_cast<float>(cursorZ)) + 0.001f;

    glEnable(GL_BLEND);
    if (isSplitAnimation()) {
      float mWestSlope  = cCornerHeight[0][1].animation() - cCornerHeight[0][0].animation();
      float mEastSlope  = cCornerHeight[1][1].animation() - cCornerHeight[1][0].animation();
      float mSouthSlope = cCornerHeight[1][0].animation() - cCornerHeight[0][0].animation();
      float mNorthSlope = cCornerHeight[1][1].animation() - cCornerHeight[0][1].animation();
      type->getSurfacePattern()->render(  pinnedX, pinnedY, mEndZ, cCornerHeight[0][0].animation(), cCornerHeight[1][0].animation(), cCornerHeight[0][1].animation(), cCornerHeight[1][1].animation(), cAlternativeSplit);
      if (type->isSolid()) {
        type->getWestWallPattern()->render( mStartX - 0.001f, mStartY,          mStartZ - 1, 1.0f, ((mEndZ - mStartZ) + 1) + cCornerHeight[0][0].animation(), mWestSlope,  0, Wall::Direction::WEST);
        type->getEastWallPattern()->render( mEndX   + 0.001f, mStartY,          mStartZ - 1, 1.0f, ((mEndZ - mStartZ) + 1) + cCornerHeight[1][0].animation(), mEastSlope,  0, Wall::Direction::EAST);
        type->getSouthWallPattern()->render(mStartX,          mStartY - 0.001f, mStartZ - 1, 1.0f, ((mEndZ - mStartZ) + 1) + cCornerHeight[0][0].animation(), mSouthSlope, 0, Wall::Direction::SOUTH);
        type->getNorthWallPattern()->render(mStartX,          mEndY   + 0.001f, mStartZ - 1, 1.0f, ((mEndZ - mStartZ) + 1) + cCornerHeight[0][1].animation(), mNorthSlope, 0, Wall::Direction::NORTH);
      }
    } else {
      float mXSlope = getXSlopeAnimation();
      float mYSlope = getYSlopeAnimation();
      type->getSurfacePattern()->render(mStartX - 0.5f, mEndX + 0.5f, mStartY - 0.5f, mEndY + 0.5f, mEndZ, mXSlope, mYSlope, ISurface::Direction::UP);
      if (type->isSolid()) {
        if (steppedBottom) {
          for (float x = mStartX; x < mEndX + 1.0f; x += 1.0f) {
            for (float y = mStartY; y < mEndY + 1.0f; y += 1.0f) {
              float mCellHeight = mStartZ + ((mXSlope < 0 ? ((mEndX -  x)  * -mXSlope) - 1.0f
                                                          : ((x - mStartX) *  mXSlope) - 1.0f) +
                                            (mYSlope < 0 ? ((mEndY -  y)  * -mYSlope) - 1.0f
                                                          : ((y - mStartY) *  mYSlope) - 1.0f) + 1.0f);

              float mWallLengthX = std::min(1.0f, (mEndX - x) + 1);
              float mWallLengthY = std::min(1.0f, (mEndY - y) + 1);
              type->getWestWallPattern()->render( std::min(x, mEndX) - 0.001f, std::min(y, mEndY),          mCellHeight, mWallLengthY, ((mEndZ - mStartZ) + 1) + (mYSlope < 0 ? -mYSlope : 0) + (mXSlope < 0 ? -mXSlope : 0), mYSlope, 0, Wall::Direction::WEST);
              type->getEastWallPattern()->render( std::min(x, mEndX) + 0.001f, std::min(y, mEndY),          mCellHeight, mWallLengthY, ((mEndZ - mStartZ) + 1) + (mYSlope < 0 ? -mYSlope : 0) + (mXSlope > 0 ?  mXSlope : 0), mYSlope, 0, Wall::Direction::EAST);
              type->getNorthWallPattern()->render(std::min(x, mEndX),          std::min(y, mEndY) + 0.001f, mCellHeight, mWallLengthX, ((mEndZ - mStartZ) + 1) + (mXSlope < 0 ? -mXSlope : 0) + (mYSlope > 0 ?  mYSlope : 0), mXSlope, 0, Wall::Direction::NORTH);
              type->getSouthWallPattern()->render(std::min(x, mEndX),          std::min(y, mEndY) - 0.001f, mCellHeight, mWallLengthX, ((mEndZ - mStartZ) + 1) + (mXSlope < 0 ? -mXSlope : 0) + (mYSlope < 0 ? -mYSlope : 0), mXSlope, 0, Wall::Direction::SOUTH);
            }
          }
        } else {
          type->getWestWallPattern()->render( mStartX - 0.001f, mStartY,          mStartZ - 1, (mEndY - mStartY) + 1, ((mEndZ - mStartZ) + 1) + (mYSlope < 0 ? (mEndY - mStartY) * -mYSlope + -mYSlope : 0) + (mXSlope < 0 ? ((mEndX - mStartX) + 1) * -mXSlope : 0), mYSlope, 0, Wall::Direction::WEST);
          type->getEastWallPattern()->render( mEndX   + 0.001f, mStartY,          mStartZ - 1, (mEndY - mStartY) + 1, ((mEndZ - mStartZ) + 1) + (mYSlope < 0 ? (mEndY - mStartY) * -mYSlope + -mYSlope : 0) + (mXSlope > 0 ? ((mEndX - mStartX) + 1) *  mXSlope : 0), mYSlope, 0, Wall::Direction::EAST);
          type->getNorthWallPattern()->render(mStartX,          mEndY   + 0.001f, mStartZ - 1, (mEndX - mStartX) + 1, ((mEndZ - mStartZ) + 1) + (mXSlope < 0 ? (mEndX - mStartX) * -mXSlope + -mXSlope : 0) + (mYSlope > 0 ? ((mEndY - mStartY) + 1) *  mYSlope : 0), mXSlope, 0, Wall::Direction::NORTH);
          type->getSouthWallPattern()->render(mStartX,          mStartY - 0.001f, mStartZ - 1, (mEndX - mStartX) + 1, ((mEndZ - mStartZ) + 1) + (mXSlope < 0 ? (mEndX - mStartX) * -mXSlope + -mXSlope : 0) + (mYSlope < 0 ? ((mEndY - mStartY) + 1) * -mYSlope : 0), mXSlope, 0, Wall::Direction::SOUTH);
        }
      }
    }
    glDisable(GL_BLEND);
  }

  void TerrainBrush::renderEditing(const TerrainType* type, float pinnedX, float pinnedY, float pinnedZ, float cursorX, float cursorY, float cursorZ, bool steppedBottom) const {
    renderPreview(type, pinnedX, pinnedY, pinnedZ, cursorX, cursorY, cursorZ, steppedBottom);
    if (cRuntimeEditing) {
      glPushMatrix();
      glTranslatef(cursorX, cursorY, cursorZ * 0.5f);
      for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
          glColor3f(0.0f, 1.0f, 0.0f);
          float mHeight = getHeight(x, y);
          glBegin(GL_LINES);
          Utils::renderVolumeLines(x * 0.5 - 0.05f,
                                   x * 0.5 + 0.05f,
                                   y * 0.5 - 0.05f,
                                   y * 0.5 + 0.05f,
                                   mHeight - 0.05f,
                                   mHeight + 0.05f);
          glEnd();
        }
      }
      float mHeight = getHeight(cRuntimeCursorX, cRuntimeCursorY);
      glColor3f(1.0f, 1.0f, 0.0f);
      Utils::renderVolumeCuboid(cRuntimeCursorX * 0.5 - 0.1f,
                                cRuntimeCursorX * 0.5 + 0.1f,
                                cRuntimeCursorY * 0.5 - 0.1f,
                                cRuntimeCursorY * 0.5 + 0.1f,
                                mHeight - 0.1f,
                                mHeight + 0.1f);
      glPopMatrix();
    }
  }

  float TerrainBrush::getHeight(float x, float y) const {
    if (isSplitAnimation()) {
      float mHeight;
      x *= 0.5f;
      y *= 0.5f;
      x += 0.5f;
      y += 0.5f;
      if (cAlternativeSplit) {
        if (y > x) {
          mHeight = (cCornerHeight[0][0].animation() + (cCornerHeight[1][1].animation() - cCornerHeight[0][1].animation()) * x + (cCornerHeight[0][1].animation() - cCornerHeight[0][0].animation()) * y);
        } else {
          mHeight = (cCornerHeight[0][0].animation() + (cCornerHeight[1][0].animation() - cCornerHeight[0][0].animation()) * x + (cCornerHeight[1][1].animation() - cCornerHeight[1][0].animation()) * y);
        }
      } else {
        if (x + y > 1.0f) {
          mHeight = (cCornerHeight[1][0].animation() + (cCornerHeight[1][1].animation() - cCornerHeight[1][0].animation()) * y + (cCornerHeight[0][1].animation() - cCornerHeight[1][1].animation()) * (1.0f - x));
        } else {
          mHeight = (cCornerHeight[0][0].animation() + (cCornerHeight[1][0].animation() - cCornerHeight[0][0].animation()) * x + (cCornerHeight[0][1].animation() - cCornerHeight[0][0].animation()) * y);
        }
      }
      return mHeight * 0.5f;
    } else {
      float mXSlope = getXSlopeAnimation();
      float mYSlope = getYSlopeAnimation();
      return (mXSlope * (mXSlope > 0 ? x + 1.0f : x - 1.0f) +
              mYSlope * (mYSlope > 0 ? y + 1.0f : y - 1.0f)) * 0.5f * 0.5f;
    }
    return 0.0f;
  }

  void TerrainBrush::stepWest() {
    cRuntimeCursorX = std::max(-1.0, cRuntimeCursorX - 1.0);
  }

  void TerrainBrush::stepEast() {
    cRuntimeCursorX = std::min(1.0, cRuntimeCursorX + 1.0);
  }

  void TerrainBrush::stepSouth() {
    cRuntimeCursorY = std::max(-1.0, cRuntimeCursorY - 1.0);
  }

  void TerrainBrush::stepNorth() {
    cRuntimeCursorY = std::min(1.0, cRuntimeCursorY + 1.0);
  }

  void TerrainBrush::stepLeft(double yaw) {
    yaw >=   40.0f && yaw <= 130.0f ? stepNorth():
    yaw >=  -50.0f && yaw <=  40.0f ? stepWest():
    yaw >= -140.0f && yaw <= -50.0f ? stepSouth():
                                      stepEast();
  }

  void TerrainBrush::stepRight(double yaw) {
    yaw >=   40.0f && yaw <= 130.0f ? stepSouth():
    yaw >=  -50.0f && yaw <=  40.0f ? stepEast():
    yaw >= -140.0f && yaw <= -50.0f ? stepNorth():
                                      stepWest();
  }

  void TerrainBrush::stepDown(double yaw) {
    yaw >=   40.0f && yaw <= 130.0f ? stepWest():
    yaw >=  -50.0f && yaw <=  40.0f ? stepSouth():
    yaw >= -140.0f && yaw <= -50.0f ? stepEast():
                                      stepNorth();
  }

  void TerrainBrush::stepUp(double yaw) {
    yaw >=   40.0f && yaw <= 130.0f ? stepEast():
    yaw >=  -50.0f && yaw <=  40.0f ? stepNorth():
    yaw >= -140.0f && yaw <= -50.0f ? stepWest():
                                      stepSouth();
  }

  bool TerrainBrush::input(sf::Event& event, double yaw) {
    if (cRuntimeEditing) {
      switch (event.type) {
        case sf::Event::KeyPressed: {
          switch (event.key.code) {
            case sf::Keyboard::Left:     stepLeft(yaw);   return true;
            case sf::Keyboard::Right:    stepRight(yaw);  return true;
            case sf::Keyboard::Up:       stepUp(yaw);     return true;
            case sf::Keyboard::Down:     stepDown(yaw);   return true;
            case sf::Keyboard::PageUp:   raiseSelected(); return true;
            case sf::Keyboard::PageDown: lowerSelected(); return true;
            default:                                      break;
          }
          break;
        }

        case sf::Event::JoystickButtonPressed: {
          switch (event.joystickButton.button) {
            case 4: return lowerSelected(); return true;
            case 5: return raiseSelected(); return true;
          }
          break;
        }

//         case SDL_JOYAXISMOTION: {
//           int mValue = std::abs(event.jaxis.value) < cDefAnalogueSensitivity ? 0 : (event.jaxis.value - (event.jaxis.value < 0 ? -cDefAnalogueSensitivity : cDefAnalogueSensitivity)) * (32767 / static_cast<float>(32767 - cDefAnalogueSensitivity));
//           switch (event.jaxis.axis) {
//             case 0: cRuntimeCursorXSpeed =  mValue / 400000.0f; return true;
//             case 1: cRuntimeCursorYSpeed = -mValue / 400000.0f; return true;
//           }
//           break;
//         }

        default: {
          break;
        }
      }
    }
    return false;
  }

  double TerrainBrush::getCursorXSpeed() const {
    return std::max(-1.0, std::min(1.0, cRuntimeCursorXSpeed));
  }

  double TerrainBrush::getCursorYSpeed() const {
    return std::max(-1.0, std::min(1.0, cRuntimeCursorYSpeed));
  }

  void TerrainBrush::update(unsigned int milliseconds, double yaw) {
    double mCursorXSpeed = getCursorXSpeed() / 10.0;
    double mCursorYSpeed = getCursorYSpeed() / 10.0;
    double mMovementDirection = atan2(-mCursorYSpeed, mCursorXSpeed) + 90.0f * (M_PI / 180.f);
    double mMovementSpeed = Utils::distance(0.0, 0.0, mCursorXSpeed, mCursorYSpeed);
    double mXSpeed = std::sin(yaw * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    double mYSpeed = std::cos(yaw * (M_PI / 180.0f) + mMovementDirection) * mMovementSpeed;
    cRuntimeCursorX = std::clamp(cRuntimeCursorX + mXSpeed, -1.0, 1.0);
    cRuntimeCursorY = std::clamp(cRuntimeCursorY + mYSpeed, -1.0, 1.0);
    if (std::abs(mXSpeed) == 0.0) {
      cRuntimeCursorX = std::round(cRuntimeCursorX);
      mXSpeed = 0.0;
    }
    if (std::abs(mYSpeed) == 0.0) {
      cRuntimeCursorY = std::round(cRuntimeCursorY);
      mYSpeed = 0.0;
    }
    cCornerHeight[0][0].update(milliseconds);
    cCornerHeight[1][0].update(milliseconds);
    cCornerHeight[0][1].update(milliseconds);
    cCornerHeight[1][1].update(milliseconds);
  }
}

/*
 * Copyright 2009 Martin Brentnall
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
#include "Craft.h"

typedef struct {
  GLubyte *imageData;
  GLuint bpp;
  GLuint width;
  GLuint height;
  GLuint texID;
} TextureImage;

map<int, GLuint> Craft::cTextures = map<int, GLuint>();
bool Craft::cStaticInitialised = false;
int Craft::cCraftTypes[3] = {PYRAMID, BALL, CIRCLE};
float Craft::cCraftChangeProgress = 0.0;

Craft::Craft() {
  if (!cStaticInitialised) {
    generateTextures();
    cStaticInitialised = true;
  }
  cType = &cCraftTypes[0];
  cLocation.x = 0.0;
  cLocation.y = 0.0;
  cLocation.z = 0.0;
  cDestAlpha = 1.0;
  cOldAlpha = 1.0;
  cAlphaProgress = 1.0;
  cGhost = false;
}

Craft::Craft(Point& location) {
  if (!cStaticInitialised) {
    generateTextures();
    cStaticInitialised = true;
  }
  cType = &cCraftTypes[0];
  setLocation(location);
  cDestAlpha = 1.0;
  cOldAlpha = 1.0;
  cAlphaProgress = 1.0;
  cGhost = false;
}

Craft::Craft(Point& location, int type) {
  if (!cStaticInitialised) {
    generateTextures();
    cStaticInitialised = true;
  }
  cType = &cCraftTypes[type - 1];
  setLocation(location);
  cDestAlpha = 1.0;
  cOldAlpha = 1.0;
  cAlphaProgress = 1.0;
  cGhost = false;
}

void Craft::toggleCraftTypes() {
  int mTemp = cCraftTypes[0];
  cCraftTypes[0] = cCraftTypes[1];
  cCraftTypes[1] = cCraftTypes[2];
  cCraftTypes[2] = mTemp;
  cCraftChangeProgress = 1.0;
}

void Craft::updateCraftTypes(int ticks) {
  if (cCraftChangeProgress > 0.0) {
    float mChange = ticks * TRANSITION_SPEED;
    cCraftChangeProgress -= mChange;
    if (cCraftChangeProgress < 0.0) {
      cCraftChangeProgress = 0.0;
    }
  }
}

void Craft::generateBallSprite(Image& image) {
}

void Craft::generateTexture(int type) {
  Image image(64, 64, type == CIRCLE_DISC || type == BALL_SPRITE);
  switch (type) {
    case PYRAMID_TOP:  generatePyramidTop(image);  break; 
    case PYRAMID_SIDE: generatePyramidSide(image); break;
    case BALL_SPRITE:  generateBallSprite(image);  break;
    case CIRCLE_DISC:  generateCircleDisc(image);  break;
    default:           cout << "Bad" << endl;      exit(1);
  }

  int mDepth = image.getDepth();
  GLuint mTexture;
  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, mDepth, image.sizeX, image.sizeY, 0, mDepth, GL_UNSIGNED_BYTE, image.data);
  cTextures[type] = mTexture;
}

void Craft::generateTextures() {
  generateTexture(PYRAMID_TOP);
  generateTexture(PYRAMID_SIDE);
  generateTexture(BALL_SPRITE);
  generateTexture(CIRCLE_DISC);
}

void Craft::render(float alpha, float rotation, float tilt) {
  if (cCraftChangeProgress > 0.0) {
    glEnable(GL_BLEND);
    alpha *= cCraftChangeProgress;
    switch (*cType) {
      case BALL:     renderAsPyramid(alpha);              break;
      case CIRCLE:   renderAsBall(alpha, rotation, tilt); break;
      case PYRAMID:  renderAsCircle(alpha);               break;
    }
    alpha = -alpha + 1.0;
  }
  switch (*cType) {
    case PYRAMID: renderAsPyramid(alpha);              break;
    case BALL:    renderAsBall(alpha, rotation, tilt); break;
    case CIRCLE:  renderAsCircle(alpha);               break;
  }
}

void Craft::renderAsCircle(float alpha) {
  float mBaseWidth = 0.03;
  float mTopWidth = 0.33;
  float mBaseHeight = BLOCK_HEIGHT * 0.9;
  float mTopHeight = BLOCK_HEIGHT * 1.80;
  float mHeight = cLocation.z * BLOCK_HEIGHT;

  if (cDestAlpha < 1.0 || cAlphaProgress < 1.0) {
    glEnable(GL_BLEND);
    float mBlend = cAlphaProgress < 1.0 ? sine(cOldAlpha, cDestAlpha, cAlphaProgress) : cDestAlpha;
    alpha *= mBlend;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, mHeight);
  glRotatef((-cLocation.x + cLocation.y * 2.0) * 90.0, 0.0, 0.0, 1.0);
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glColor4f(0.0, 0.0, 0.0, alpha);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight);

  glVertex3f(0.0, 0.0, mTopHeight);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0, mTopHeight);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glColor4f(1.0, 1.0, 1.0, alpha);
  glEnd();

  int mTexture = CIRCLE_DISC;
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, cTextures[mTexture]);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mTopWidth, 0.0 + mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mTopWidth, 0.0 + mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth, 0.0 - mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth, 0.0 - mTopWidth, 0.0 + mBaseHeight);
  glEnd();
  glDisable(GL_ALPHA_TEST);
  
  glEnable(GL_CULL_FACE);  
  glPopMatrix();
}

void Craft::renderAsBall(float alpha, float rotation, float tilt) {
}

void Craft::setHeight(float height) {
  cLocation.z = height;
}

void Craft::respawn() {
  cLocation.x = cRespawnPoint.x;
  cLocation.y = cRespawnPoint.y;
  cLocation.z = cRespawnPoint.z;
  stop();
}

void Craft::land() {
  cMomentum.z = 0.0;
  cLocation.z = (int) cLocation.z;
}

void Craft::renderAsPyramid(float alpha) {
}

void Craft::changeMomentum(Coordinate relativeMomentum) {
  cMomentum.x += relativeMomentum.x;
  cMomentum.y += relativeMomentum.y;
  cMomentum.z += relativeMomentum.z;
}

void Craft::resetToOrigin() {
  cLocation.x = cStartLocation.x;
  cLocation.y = cStartLocation.y;
  cLocation.z = cStartLocation.z;
  cMomentum.x = 0.0;
  cMomentum.y = 0.0;
  cMomentum.z = 0.0;
}

void Craft::setLocation(Coordinate& location) {
  cLocation.x = location.x;
  cLocation.y = location.y;
  cLocation.z = location.z;
  cStartLocation.x = location.x;
  cStartLocation.y = location.y;
  cStartLocation.z = location.z;
}

void Craft::setLocation(Point& location) {
  cLocation.x = location.x;
  cLocation.y = location.y;
  cLocation.z = location.z;
  cStartLocation.x = location.x;
  cStartLocation.y = location.y;
  cStartLocation.z = location.z;
}

void Craft::setGhost(bool enabled) {
  if (cGhost != enabled) {
    cOldAlpha = cDestAlpha;
//    cAlphaProgress = -cAlphaProgress + 1.0;
    cAlphaProgress = 0.0;
    cDestAlpha = enabled ? 0.3 : 1.0;
    cGhost = enabled;
  }
}

void Craft::updateVisuals(int ticks) {
  if (cAlphaProgress < 1.0) {
    cAlphaProgress += ticks * TRANSITION_SPEED;
    if (cAlphaProgress > 1.0) {
      cAlphaProgress = 1.0;
    }
  }
}

Coordinate Craft::requestLocation(float ticks, bool airborne, bool ice, Coordinate& momentumChange) {
  Coordinate mLocation(cLocation.x + (cMomentum.x + momentumChange.x) * ticks, cLocation.y + (cMomentum.y + momentumChange.y) * ticks, cLocation.z + (cMomentum.z + momentumChange.z) * ticks);
  return mLocation;
}

void Craft::updateLocation(Coordinate location) {
  cLocation = location;
}

void Craft::updateMomentum(float ticks) {
  float mResistance = pow(0.999, ticks);
  cMomentum.x *= mResistance;
  cMomentum.y *= mResistance;
}

bool Craft::calculateZMomentum(float xSlope, float ySlope) {
  cMomentum.z = (xSlope * cMomentum.x + ySlope * cMomentum.y);// * BLOCK_HEIGHT;
  return cMomentum.z != 0.0f;
}

// TODO: Enemy crafts should request location too
void Craft::moveTowards(Coordinate& dest, float ticks) {
  float mDistanceMoved = ticks * 0.00002;
  cMomentum.x += dest.x > cLocation.x ? mDistanceMoved : -mDistanceMoved;
  cMomentum.y += dest.y > cLocation.y ? mDistanceMoved : -mDistanceMoved;
  Coordinate mDummyChange;
  updateLocation(requestLocation(ticks, false, false, mDummyChange));
  updateMomentum(ticks);
}

void Craft::moveToCamera() {
  glTranslatef(-cLocation.x * BLOCK_SIZE, -cLocation.y * BLOCK_SIZE, -cLocation.z * BLOCK_HEIGHT);
}

void Craft::stop() {
  cMomentum.x = 0.0;
  cMomentum.y = 0.0;
  cMomentum.z = 0.0;
}

void Craft::setRespawnPoint(Point location) {
  cRespawnPoint = location;
}

Coordinate& Craft::getLocation() {
  return cLocation;
}

Point& Craft::getGridLocation() {
  Point *mPoint;
  mPoint = new Point((int) floor(cLocation.x + 0.5), (int) floor(cLocation.y + 0.5), (int) floor(cLocation.z + 0.5));
  return *mPoint;
}

void Craft::write(FILE *w) {
  fprintf(w, "<CRAFT TYPE %d LOCATION %d %d %d>\n", *cType, (int) cStartLocation.x, (int) cStartLocation.y, (int) cStartLocation.z);
}

void Craft::debug() {
  cout << "Location " << cLocation.z << "    Momentum " << cMomentum.z << endl;
}

bool Craft::bounceWall(int facing) {
  float mStopThreshold = STOP_THRESHOLD;
  if ((facing == NORTH && cMomentum.y < 0.0f) || (facing == SOUTH && cMomentum.y > 0.0f)) {
    if (cMomentum.y < STOP_THRESHOLD && cMomentum.y > -mStopThreshold) {
      cMomentum.y = 0.0f;
      return true;
    } else {
      cMomentum.y = -cMomentum.y * WALL_IMPACT_ABSORTION;
      return false;
    }
  } else if ((facing == EAST && cMomentum.x < 0.0f) || (facing == WEST && cMomentum.x > 0.0f)) {
    if (cMomentum.x < STOP_THRESHOLD && cMomentum.x > -mStopThreshold) {
      cMomentum.x = 0.0f; 
      return true;
    } else {
      cMomentum.x = -cMomentum.x * WALL_IMPACT_ABSORTION;
      return false;
    }
  }
  return false;
}

void Craft::bounceTrampoline() {
//  cMomentum.z = -cMomentum.z;
  cMomentum.z = -cMomentum.z * 1.011;
}

float Craft::getCollisionTime(Coordinate& start, Coordinate& end, float ticks) {
  // TODO: Last value should be the "predicted" end point
  // TODO: If multiple lines are required (due to, e.g. collision with wall), this needs to be taken into consideration
  return getCollisionTime(start, end, cLocation, cLocation);
}

float Craft::getCollisionTime(Coordinate& startA, Coordinate& endA, Coordinate& startB, Coordinate& endB) {
  float mXDifference = endB.x - startB.x;
  float mYDifference = endB.y - startB.y;
  float mZDifference = endB.z - startB.z;
  float mRelativeEndAX = endA.x - mXDifference;
  float mRelativeEndAY = endA.y - mYDifference;
  float mRelativeEndAZ = endA.z - mZDifference;
  Coordinate mRelativeEndA(mRelativeEndAX, mRelativeEndAY, mRelativeEndAZ);
  return getCollisionTime(startA, mRelativeEndA, startB);
}

float Craft::getCollisionTime(Coordinate& start, Coordinate end, Coordinate b) {

  // Rotate it so we're doing it in 2D
  // 'Rotate (door handle)' end and b around start to make end 'Z' the same as start 'Z'
  double mEndXZAngle = atan2(end.z - start.z, end.x - start.x);
  double mBXZAngle = atan2(b.z - start.z, b.x - start.x);
  float mEndXZHeight = end.z - start.z;
  float mEndXZBase = end.x - start.x;
  double mEndXZDistance = sqrt(mEndXZHeight * mEndXZHeight + mEndXZBase * mEndXZBase);
  end.z = start.z;
  end.x = start.x + (float) mEndXZDistance;
  float mBXZHeight = b.z - start.z;
  float mBXZBase = b.x - start.x;
  double mBXZDistance = sqrt(mBXZHeight * mBXZHeight + mBXZBase * mBXZBase);
  mBXZAngle -= mEndXZAngle;
  double mBXZCos = cos(mBXZAngle) * mBXZDistance + start.x;
  double mXZBSin = sin(mBXZAngle) * mBXZDistance + start.z;
  b.x = (float) mBXZCos;
  b.z = (float) mXZBSin;

  // 'Spin' end and b around start to make end 'Y' the same as start 'Y'
  double mEndXYAngle = atan2(end.y - start.y, end.x - start.x);
  double mBXYAngle = atan2(b.y - start.y, b.x - start.x);
  float mEndXYHeight = end.y - start.y;
  float mEndXYBase = end.x - start.x;
  double mEndXYDistance = sqrt(mEndXYHeight * mEndXYHeight + mEndXYBase * mEndXYBase);
  end.y = start.y;
  end.x = start.x + (float) mEndXYDistance;
  float mBXYHeight = b.y - start.y;
  float mBXYBase = b.x - start.x;
  double mBXYDistance = sqrt(mBXYHeight * mBXYHeight + mBXYBase * mBXYBase);
  mBXYAngle -= mEndXYAngle;
  double mBXYCos = cos(mBXYAngle) * mBXYDistance + start.x;
  double mBXYSin = sin(mBXYAngle) * mBXYDistance + start.y;
  b.x = (float) mBXYCos;
  b.y = (float) mBXYSin;

  // 'Rotate' b around start-end to make b 'Z' the same as start-end 'Z'
  float mBZYHeight = b.y - start.y;
  float mBZYBase = b.z - start.z;
  double mBZYDistance = sqrt(mBZYHeight * mBZYHeight + mBZYBase * mBZYBase);
  b.z = start.z;
  b.y = start.y + (float) mBZYDistance;

  // Do the detection
  double mStartPerpY = b.y - CRAFT_RADIUS * 2;
  double mEndPerpY = b.y + CRAFT_RADIUS * 2;
  if (start.x <= b.x && b.x <= end.x && mStartPerpY <= start.y && start.y <= mEndPerpY) {
    float mHeight = b.x - b.x;
    float mWidth = b.y - start.y;
    double mLength = sqrt(mHeight * mHeight + mWidth * mWidth);
    double mHypoteneuse = CRAFT_RADIUS * 2;
    double mBase = sqrt(mHypoteneuse * mHypoteneuse - mLength * mLength);
    float mImpactX = b.x - (float) mBase;
    // NOTE: The exit point
    float mImpactXR = b.x + (float) mBase;
    double mGradient = (mImpactX - start.x) / (end.x - start.x);
    double mGradientR = (mImpactXR - start.x) / (end.x - start.x);
    return mGradientR - mGradient;
  }
  return 0.0f;
}


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
#include "SpindizzyJewelModel.h"

unsigned int SpindizzyJewelModel::cReferenceCount = 0;
GLuint SpindizzyJewelModel::cPanelDisplayList;
GLuint SpindizzyJewelModel::cFrameDisplayList;

SpindizzyJewelModel::SpindizzyJewelModel(Vertex* location) {
  cLocation = location;
  long int mRandomNumber = random();
  cColourChannel = mRandomNumber % 3 == 0 ? &cCurrentColour.cRed 
                : (mRandomNumber % 3 == 1 ? &cCurrentColour.cGreen
                                          : &cCurrentColour.cBlue);
  cColourUp = mRandomNumber % 2 == 1 ? true : false;
  cCurrentColour.cRed   = cColourChannel == &cCurrentColour.cRed   ? (mRandomNumber % 1000) / 1000.0f : 0.0f;
  cCurrentColour.cGreen = cColourChannel == &cCurrentColour.cGreen ? (mRandomNumber % 1000) / 1000.0f : 0.0f;
  cCurrentColour.cBlue  = cColourChannel == &cCurrentColour.cBlue  ? (mRandomNumber % 1000) / 1000.0f : 0.0f;

  if (cReferenceCount == 0) {
    float mRadius = IsoRealmsConstants::BLOCK_RADIUS;
    float mLineWidth = 0.05f;
    Vertex mBottom(0.0f, 0.0f, 0.0f);
    Vertex mTop(0.0f, 0.0f,  IsoRealmsConstants::BLOCK_HEIGHT * 1.4f);
    Vertex mNorth(0.0f, mRadius, IsoRealmsConstants::BLOCK_HEIGHT * 0.7f);
    Vertex mEast(mRadius, 0.0f, IsoRealmsConstants::BLOCK_HEIGHT * 0.7f);
    Vertex mSouth(0.0f, -mRadius, IsoRealmsConstants::BLOCK_HEIGHT * 0.7f);
    Vertex mWest(-mRadius, 0.0f, IsoRealmsConstants::BLOCK_HEIGHT * 0.7f);

    cPanelDisplayList = glGenLists(1);
    glNewList(cPanelDisplayList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    renderInnerTriangle(mBottom, mNorth, mEast,  mLineWidth);
    renderInnerTriangle(mBottom, mEast,  mSouth, mLineWidth);
    renderInnerTriangle(mBottom, mSouth, mWest,  mLineWidth);
    renderInnerTriangle(mBottom, mWest,  mNorth, mLineWidth);
    renderInnerTriangle(mTop,    mNorth, mWest,  mLineWidth);
    renderInnerTriangle(mTop,    mWest,  mSouth, mLineWidth);
    renderInnerTriangle(mTop,    mSouth, mEast,  mLineWidth);
    renderInnerTriangle(mTop,    mEast,  mNorth, mLineWidth);
    glEnd();
    glEndList();
  
    cFrameDisplayList = glGenLists(1);
    glNewList(cFrameDisplayList, GL_COMPILE);
    glColor3f(1.0f, 1.0f, 0.4f);
    glBegin(GL_TRIANGLES);
    renderOuterTriangle(mBottom, mNorth, mEast,  mLineWidth);
    renderOuterTriangle(mBottom, mEast,  mSouth, mLineWidth);
    renderOuterTriangle(mBottom, mSouth, mWest,  mLineWidth);
    renderOuterTriangle(mBottom, mWest,  mNorth, mLineWidth);
    renderOuterTriangle(mTop,    mNorth, mWest,  mLineWidth);
    renderOuterTriangle(mTop,    mWest,  mSouth, mLineWidth);
    renderOuterTriangle(mTop,    mSouth, mEast,  mLineWidth);
    renderOuterTriangle(mTop,    mEast,  mNorth, mLineWidth);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEndList();
  }
  cReferenceCount++;
}

void SpindizzyJewelModel::update(int milliseconds) {
  float mColourChange = milliseconds * 0.0015;
  *cColourChannel += cColourUp ? mColourChange : -mColourChange;
  if (*cColourChannel >= 1.0 || *cColourChannel <= 0.0) {
    cColourUp = !cColourUp;
    *cColourChannel = *cColourChannel >= 1.0 ? 1.0 : 0.0;
    cColourChannel = (cColourChannel == &cCurrentColour.cRed     ? &cCurrentColour.cGreen
                   : (cColourChannel == &cCurrentColour.cGreen   ? &cCurrentColour.cBlue
                   /* cColourChannel == *cCurrentColour.cBlue */ : &cCurrentColour.cRed));
  }
}

void SpindizzyJewelModel::render() {
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glBindTexture(GL_TEXTURE_2D, 0);
  glCallList(cFrameDisplayList);
  glColor3f(cCurrentColour.cRed, cCurrentColour.cGreen, cCurrentColour.cBlue);
  glCallList(cPanelDisplayList);
  glColor3f(1.0, 1.0, 1.0);
}

SpindizzyJewelModel::~SpindizzyJewelModel() {
  cReferenceCount--;
  if (cReferenceCount == 0) {
    // TODO: Destroy display lists
  }
}

// TODO: Everything below here is pretty shitty.
void SpindizzyJewelModel::renderInnerTriangle(Vertex& a, Vertex& b, Vertex& c, float lineWidth) {
  Vertex mA;
  Vertex mB;
  Vertex mC;
  renderCellTriangle(a, b, c, mA, mB, mC, lineWidth);
  glVertex3f(mA.x, mA.y, mA.z);
  glVertex3f(mB.x, mB.y, mB.z);
  glVertex3f(mC.x, mC.y, mC.z);
}

void SpindizzyJewelModel::renderOuterTriangle(Vertex& a, Vertex& b, Vertex& c, float lineWidth) {
  Vertex mA;
  Vertex mB;
  Vertex mC;
  renderCellTriangle(a, b, c, mA, mB, mC, lineWidth);
  glVertex3f(mB.x, mB.y, mB.z);
  glVertex3f(mA.x, mA.y, mA.z);
  glVertex3f(a.x, a.y, a.z);
  glVertex3f(mB.x, mB.y, mB.z);
  glVertex3f(a.x, a.y, a.z);
  glVertex3f(b.x, b.y, b.z);

  glVertex3f(mC.x, mC.y, mC.z);
  glVertex3f(mB.x, mB.y, mB.z);
  glVertex3f(b.x, b.y, b.z);
  glVertex3f(mC.x, mC.y, mC.z);
  glVertex3f(b.x, b.y, b.z);
  glVertex3f(c.x, c.y, c.z);

  glVertex3f(mA.x, mA.y, mA.z);
  glVertex3f(mC.x, mC.y, mC.z);
  glVertex3f(c.x, c.y, c.z);
  glVertex3f(mA.x, mA.y, mA.z);
  glVertex3f(c.x, c.y, c.z);
  glVertex3f(a.x, a.y, a.z);
}

double SpindizzyJewelModel::getValue(Vertex a1, Vertex a2, Vertex b1, Vertex b2) {
  return ((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x -b1.x)) / ((b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y));
}

double SpindizzyJewelModel::distance(double a1, double a2, double b1, double b2) {
  double px = a1 - b1;
  double py = a2 - b2;
  return sqrt(px * px + py * py);
}

// Imagine that Z is depth (looking into / out of the screen).
// This function uses 'A' as the original, transforming 'B' and 'C' to create a flat-base upright triangle.
// This is achieved first by the following steps:
// We 'spin' the triangle (as you would turn a glass on a desk) so that B is at the same depth as A.
// We 'rotate' the triangle (as you would turn a door handle) so that B is in line with A.
// We 'tilt' the triangle (as you would --?) so that C is the same depth as A and B.
// We then calculate lines parallel to the inside of the triangle.
// From these lines, we get intersection points to determine where the inner points lie.
// We then transform these points in reverse to get the resulting 'cell triangle' vertices.

void SpindizzyJewelModel::renderCellTriangle(Vertex mPointA, Vertex mPointB, Vertex mPointC, Vertex& mResultA, Vertex& mResultB, Vertex& mResultC, float mWidth) {
//  double mRadiansToDegrees = 180.0 / M_PI;

  // Turn points B and C (make B.z equal to A.z)
  double mTurnB = atan2(-mPointB.z + mPointA.z, mPointB.x - mPointA.x);
  double mTurnC = atan2(-mPointC.z + mPointA.z, mPointC.x - mPointA.x);
  double mDistance = distance(mPointA.x, mPointA.z, mPointB.x, mPointB.z);
  mPointB.x = mDistance + mPointA.x;
  mPointB.z = mPointA.z;

  mDistance = distance(mPointA.x, mPointA.z, mPointC.x, mPointC.z);
  mPointC.x = cos(mTurnB - mTurnC) * mDistance + mPointA.x;
  mPointC.z = sin(mTurnB - mTurnC) * mDistance + mPointA.z;

  // Rotate points B and C (make B.y equal to A.y)
  double mRotateB = atan2(-mPointB.y + mPointA.y, mPointB.x - mPointA.x);
  double mRotateC = atan2(-mPointC.y + mPointA.y, mPointC.x - mPointA.x);
  mDistance = distance(mPointA.x,  mPointA.y, mPointB.x, mPointB.y);
  mPointB.x = mDistance + mPointA.x;
  mPointB.y = mPointA.y;

  mDistance = distance(mPointA.x, mPointA.y, mPointC.x, mPointC.y);
  mPointC.x = cos(mRotateB - mRotateC) * mDistance + mPointA.x;
  mPointC.y = sin(mRotateB - mRotateC) * mDistance + mPointA.y;

  // Tilt point C (make C.z equal to A.z and B.z)
  double mTilt = atan2(-mPointC.z + mPointA.z, mPointC.y - mPointA.y);
  mDistance = distance(mPointA.y, mPointA.z, mPointC.y, mPointC.z);
  mPointC.y = mDistance + mPointA.y;
  mPointC.z = mPointA.z;

  // Get inner points on the (now 2D) triangle
  double mAngleA = atan2(-mPointB.y + mPointA.y, mPointB.x - mPointA.x);
  double mAngleB = atan2(-mPointC.y + mPointB.y, mPointC.x - mPointB.x);
  double mAngleC = atan2(-mPointA.y + mPointC.y, mPointA.x - mPointC.x);
  Vertex mOffsetPointA(cos(mAngleA - M_PI * 0.5) * mWidth + mPointA.x, (sin(mAngleA - M_PI * 0.5) * mWidth - mPointA.y) * -1.0, 0.0);
  Vertex mOffsetPointB(cos(mAngleB - M_PI * 0.5) * mWidth + mPointB.x, (sin(mAngleB - M_PI * 0.5) * mWidth - mPointB.y) * -1.0, 0.0);
  Vertex mOffsetPointC(cos(mAngleC - M_PI * 0.5) * mWidth + mPointC.x, (sin(mAngleC - M_PI * 0.5) * mWidth - mPointC.y) * -1.0, 0.0);
  Vertex mArrowPointA(cos(mAngleA) * mWidth + mOffsetPointA.x, (sin(mAngleA) * mWidth - mOffsetPointA.y) * -1.0, 0.0);
  Vertex mArrowPointB(cos(mAngleB) * mWidth + mOffsetPointB.x, (sin(mAngleB) * mWidth - mOffsetPointB.y) * -1.0, 0.0);
  Vertex mArrowPointC(cos(mAngleC) * mWidth + mOffsetPointC.x, (sin(mAngleC) * mWidth - mOffsetPointC.y) * -1.0, 0.0);
  double mGradA = getValue(mOffsetPointA, mArrowPointA, mOffsetPointB, mArrowPointB);
  double mGradB = getValue(mOffsetPointB, mArrowPointB, mOffsetPointC, mArrowPointC);
  double mGradC = getValue(mOffsetPointC, mArrowPointC, mOffsetPointA, mArrowPointA);
  mResultB.set(mOffsetPointA.x + mGradA * (mArrowPointA.x - mOffsetPointA.x), mOffsetPointA.y + mGradA * (mArrowPointA.y - mOffsetPointA.y), mPointA.z);
  mResultC.set(mOffsetPointB.x + mGradB * (mArrowPointB.x - mOffsetPointB.x), mOffsetPointB.y + mGradB * (mArrowPointB.y - mOffsetPointB.y), mPointA.z);
  mResultA.set(mOffsetPointC.x + mGradC * (mArrowPointC.x - mOffsetPointC.x), mOffsetPointC.y + mGradC * (mArrowPointC.y - mOffsetPointC.y), mPointA.z);

  // Untilt results
  mDistance = distance(mPointA.y, mPointA.z, mResultC.y, mResultC.z);
  mResultC.z = cos(mTilt + M_PI / 2) * mDistance + mPointA.z;
  mResultC.y = sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;
  mDistance = distance(mPointA.y, mPointA.z, mResultB.y, mResultB.z);
  mResultB.z = cos(mTilt + M_PI / 2) * mDistance + mPointA.z;
  mResultB.y = sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;
  mDistance = distance(mPointA.y, mPointA.z, mResultA.y, mResultA.z);
  mResultA.z = cos(mTilt + M_PI / 2) * mDistance + mPointA.z;
  mResultA.y = sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;

  // Unrotate results
  mDistance = distance(mPointA.x, mPointA.y, mResultA.x, mResultA.y);
  double mCurrentAngle = atan2(-mResultA.y + mPointA.y, mResultA.x - mPointA.x);
  mResultA.y = cos(mCurrentAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
  mResultA.x = sin(-((mCurrentAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;
  mDistance = distance(mPointA.x, mPointA.y, mResultB.x, mResultB.y);
  mCurrentAngle = atan2(-mResultB.y + mPointA.y, mResultB.x - mPointA.x);
  mResultB.y = cos(mCurrentAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
  mResultB.x = sin(-((mCurrentAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;
  mDistance = distance(mPointA.x, mPointA.y, mResultC.x, mResultC.y);
  mCurrentAngle = atan2(-mResultC.y + mPointA.y, mResultC.x - mPointA.x);
  mResultC.y = cos(mCurrentAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
  mResultC.x = sin(-((mCurrentAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;

  // Unturn results
  mDistance = distance(mPointA.x, mPointA.z, mResultA.x, mResultA.z);
  mCurrentAngle = atan2(-mResultA.z + mPointA.z, mResultA.x - mPointA.x);
  mResultA.z = cos(mCurrentAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
  mResultA.x = sin(-((mCurrentAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
  mDistance = distance(mPointA.x, mPointA.z, mResultB.x, mResultB.z);
  mCurrentAngle = atan2(-mResultB.z + mPointA.z, mResultB.x - mPointA.x);
  mResultB.z = cos(mCurrentAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
  mResultB.x = sin(-((mCurrentAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
  mDistance = distance(mPointA.x, mPointA.z, mResultC.x, mResultC.z);
  mCurrentAngle = atan2(-mResultC.z + mPointA.z, mResultC.x - mPointA.x);
  mResultC.z = cos(mCurrentAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
  mResultC.x = sin(-((mCurrentAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
}

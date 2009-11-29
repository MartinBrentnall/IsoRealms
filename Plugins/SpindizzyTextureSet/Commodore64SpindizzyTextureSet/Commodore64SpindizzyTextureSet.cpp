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
#include "Commodore64SpindizzyTextureSet.h"

/* Generation functions */
Image* Commodore64SpindizzyTextureSet::generateIce(Colour* colour) {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, false);
  mImage->drawSquare(colour, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generatePlain() {
  Image* mImage = generateIce(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSplitPlain() {
  Image* mImage = generateIce(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateTrampoline() {
  Image* mImage = generateIce(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::BACKGROUND), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchSquare() {
  Image* mImage = generatePlain();
  int mOuterSquare = (int) (RESOLUTION * 0.2);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchSquareHalf() {
  Image* mImage = generateSwitchSquare();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchSquareBoth() {
  Image* mImage = generateSwitchSquare();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), mInnerSquare, RESOLUTION - mInnerSquare
                                                 , mInnerSquare, mHalfSquare
                                                 , mHalfSquare, RESOLUTION - mInnerSquare);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchDiamond() {
  Image* mImage = generatePlain();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  mImage->drawDiamond(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterSquare);
  mImage->drawDiamond(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare);
  return mImage;
}


Image* Commodore64SpindizzyTextureSet::generateSwitchDiamondHalf() {
  Image* mImage = generateSwitchDiamond();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchDiamondBoth() {
  Image* mImage = generateSwitchDiamond();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, mInnerSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchCircle() {
  Image* mImage = generatePlain();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchCircleBoth() {
  Image* mImage = generatePlain();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateSwitchCircleHalf() {
  Image* mImage = generatePlain();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  mImage->drawSemiCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateArrow() {
  Image* mImage = generatePlain();
  int mInnerPoint = (int) (RESOLUTION * 0.40);
  int mOuterPoint = (int) (RESOLUTION * 0.15);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterPoint, mHalfSquare, mInnerPoint, RESOLUTION - mInnerPoint);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::WALL), mHalfSquare, mOuterPoint, RESOLUTION - mOuterPoint, mHalfSquare, mHalfSquare, RESOLUTION - mOuterPoint);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateWallNorthSouth() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.85);
  Image* mImage = generateIce(&mGridColour);
  Colour mWallFloorMix(*(cColourScheme->getColour(IFourColourSupport::FLOOR)), *(cColourScheme->getColour(IFourColourSupport::WALL)));
  mImage->drawSquare(&mWallFloorMix, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateWallEastWest() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.70);
  Image* mImage = generateIce(&mGridColour);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateIceWall() {
  Image* mImage = generateIce(cColourScheme->getColour(IFourColourSupport::EXTRA));
  int mIceWidth = (int) (RESOLUTION * 0.1);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), 0, RESOLUTION, RESOLUTION, RESOLUTION, mIceWidth, RESOLUTION - mIceWidth);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), mIceWidth, RESOLUTION - mIceWidth, RESOLUTION, RESOLUTION, RESOLUTION - mIceWidth, RESOLUTION - mIceWidth);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::makeTransparent() {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, true);
  mImage->drawSquare(TRANSPARENT, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::Commodore64SpindizzyTextureSet::generateLiftSquare() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = (int) (mImage->sizeX * 0.16);
  int mSquareOuterFill = (int) (mImage->sizeX * 0.20);
  int mSquareInnerFill = (int) (mImage->sizeX * 0.30);
  int mSquareInnerEdge = (int) (mImage->sizeX * 0.34);

  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(YELLOW,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftSquareHalf() {
  Image* mImage = generateLiftSquare();
  mImage->drawTriangle(BLACK,  int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56), int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44));
  mImage->drawTriangle(PURPLE, int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftSquareBoth() {
  Image* mImage = generateLiftSquareHalf();
  mImage->drawTriangle(BLACK,  int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44), int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56));
  mImage->drawTriangle(PURPLE, int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftDiamond() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->sizeX * 0.16);
  int mSquareOuterFill = int(mImage->sizeX * 0.20);
  int mSquareInnerFill = int(mImage->sizeX * 0.26);
  int mSquareInnerEdge = int(mImage->sizeX * 0.30);

  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawDiamond(BLACK,       mImage->sizeX);
  mImage->drawDiamond(YELLOW,      int(mImage->sizeX * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->sizeX * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->sizeX * 0.78));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftDiamondHalf() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->sizeX * 0.16);
  int mSquareOuterFill = int(mImage->sizeX * 0.20);
  int mSquareInnerFill = int(mImage->sizeX * 0.26);
  int mSquareInnerEdge = int(mImage->sizeX * 0.30);

  mImage->drawDiamond(BLACK,       mImage->sizeX);
  mImage->drawDiamond(YELLOW,      int(mImage->sizeX * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->sizeX * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->sizeX * 0.78));
  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawHalfDiamond2(BLACK,       int(mImage->sizeX * 0.50));
  mImage->drawHalfDiamond( YELLOW,      int(mImage->sizeX * 0.44));
  mImage->drawHalfDiamond( BLACK,       int(mImage->sizeX * 0.34));
  mImage->drawHalfDiamond( TRANSPARENT, int(mImage->sizeX * 0.28));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftDiamondBoth() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->sizeX * 0.16);
  int mSquareOuterFill = int(mImage->sizeX * 0.20);
  int mSquareInnerFill = int(mImage->sizeX * 0.26);
  int mSquareInnerEdge = int(mImage->sizeX * 0.30);

  mImage->drawDiamond(BLACK,       mImage->sizeX);
  mImage->drawDiamond(YELLOW,      int(mImage->sizeX * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->sizeX * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->sizeX * 0.78));
  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftCircle() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(BLACK,       int(mImage->sizeX * 0.9 * 0.5));
  mImage->drawCircle(PURPLE,      int(mImage->sizeX * 0.9 * 0.46));
  mImage->drawCircle(BLACK,       int(mImage->sizeX * 0.9 * 0.34));
  mImage->drawCircle(TRANSPARENT, int(mImage->sizeX * 0.9 * 0.30));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftCircleHalf() {
  Image* mImage = generateLiftCircle();
  mImage->drawSemiCircle(YELLOW, int(mImage->sizeX * 0.9 * 0.34));
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::generateLiftCircleBoth() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(BLACK,  int(mImage->sizeX * 0.9 * 0.5));
  mImage->drawCircle(PURPLE, int(mImage->sizeX * 0.9 * 0.46));
  mImage->drawCircle(YELLOW, int(mImage->sizeX * 0.9 * 0.34));
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::registerTexture(Image* image) {
  GLuint mTextureID;
  int mDepth = image->getDepth();
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, mDepth, image->sizeX, image->sizeY, 0, mDepth, GL_UNSIGNED_BYTE, image->data);
  return mTextureID;
}

Commodore64SpindizzyTextureSet::Commodore64SpindizzyTextureSet() {
  cPlugSockets.push_back(new PlugSocket("FourColourSupport", ""));
  std::string mDummyName("FourColourSupport");
  cColourScheme = dynamic_cast<IFourColourSupport*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cColourScheme == NULL) {
    std::cout << "Dynamic cast of colour scheme dummy didn't work!" << std::endl;
  } else {
    std::cout << "dynamic_cast succeeded for dummy four colour palette" << std::endl;
  }

  // TODO: Nasty hack; get rid of.
  TRANSPARENT = new Colour(0.0, 0.0, 0.0, 0.0);
  PURPLE = new Colour(0.7, 0.0, 1.0, 1.0);
  YELLOW = new Colour(1.0, 1.0, 0.0, 1.0);
  BLACK = new Colour(0.0, 0.0, 0.0, 1.0);
  generateTextures();
}

void Commodore64SpindizzyTextureSet::generateTextures() {
  cTextures[SWITCH_CIRCLE_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchCircleBoth()));
  GLuint mSwitchCircleHalf = registerTexture(generateSwitchCircleHalf());
  cTextures[SWITCH_CIRCLE_LEFT] = new Commodore64SpindizzyTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_CIRCLE_RIGHT] = new Commodore64SpindizzyTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_CIRCLE_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchCircle()));
  cTextures[SWITCH_SQUARE_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchSquareBoth()));
  GLuint mSwitchSquareHalf = registerTexture(generateSwitchSquareHalf());
  cTextures[SWITCH_SQUARE_LEFT] = new Commodore64SpindizzyTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_SQUARE_RIGHT] = new Commodore64SpindizzyTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_SQUARE_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchSquare()));
  cTextures[SWITCH_DIAMOND_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchDiamondBoth()));
  GLuint mSwitchDiamondHalf = registerTexture(generateSwitchDiamondHalf());
  cTextures[SWITCH_DIAMOND_LEFT] = new Commodore64SpindizzyTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_DIAMOND_RIGHT] = new Commodore64SpindizzyTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_DIAMOND_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateSwitchDiamond()));
  cTextures[SWITCH_RESET] = cTextures[SWITCH_DIAMOND_NONE];
  GLuint mArrowImage = registerTexture(generateArrow());
  cTextures[ARROW_NORTH] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::NORTH);
  cTextures[ARROW_EAST] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::EAST);
  cTextures[ARROW_SOUTH] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::SOUTH);
  cTextures[ARROW_WEST] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::WEST);
  cTextures[TRAMPOLINE] = new Commodore64SpindizzyTexture(registerTexture(generateTrampoline()));
  cTextures[ICE] = new Commodore64SpindizzyTexture(registerTexture(generateIce(cColourScheme->getColour(IFourColourSupport::EXTRA))));
  cTextures[WATER] = cTextures[ICE];
  cTextures[PLAIN] = new Commodore64SpindizzyTexture(registerTexture(generatePlain()));
  GLuint mSplitPlain = registerTexture(generateSplitPlain());
  cTextures[PLAIN_SPLIT_NE_SW] = new Commodore64SpindizzyTexture(mSplitPlain, Commodore64SpindizzyTexture::NORTH);
  cTextures[PLAIN_SPLIT_NW_SE] = new Commodore64SpindizzyTexture(mSplitPlain, Commodore64SpindizzyTexture::EAST);
  cTextures[WALL_NORTH] = new Commodore64SpindizzyTexture(registerTexture(generateWallNorthSouth()), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_EAST] = new Commodore64SpindizzyTexture(registerTexture(generateWallEastWest()), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_SOUTH] = cTextures[WALL_NORTH];
  cTextures[WALL_WEST] = cTextures[WALL_EAST];
  cTextures[WALL_ICE] = new Commodore64SpindizzyTexture(registerTexture(generateIceWall()), ISpindizzyTexture::TILED);

  GLuint mLiftCircleHalf = registerTexture(generateLiftCircleHalf());
  cTextures[LIFT_CIRCLE_LEFT] = new Commodore64SpindizzyTexture(mLiftCircleHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_CIRCLE_RIGHT] = new Commodore64SpindizzyTexture(mLiftCircleHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_CIRCLE_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateLiftCircle()));
  cTextures[LIFT_CIRCLE_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateLiftCircleBoth()));
  GLuint mLiftSquareHalf = registerTexture(generateLiftSquareHalf());
  cTextures[LIFT_SQUARE_LEFT] = new Commodore64SpindizzyTexture(mLiftSquareHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_SQUARE_RIGHT] = new Commodore64SpindizzyTexture(mLiftSquareHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_SQUARE_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateLiftSquare()));
  cTextures[LIFT_SQUARE_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateLiftSquareBoth()));
  GLuint mLiftDiamondHalf = registerTexture(generateLiftDiamondHalf());
  cTextures[LIFT_DIAMOND_LEFT] = new Commodore64SpindizzyTexture(mLiftDiamondHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_DIAMOND_RIGHT] = new Commodore64SpindizzyTexture(mLiftDiamondHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_DIAMOND_NONE] = new Commodore64SpindizzyTexture(registerTexture(generateLiftDiamond()));
  cTextures[LIFT_DIAMOND_BOTH] = new Commodore64SpindizzyTexture(registerTexture(generateLiftDiamondBoth()));
}

void Commodore64SpindizzyTextureSet::destroyTextures() {
  // TODO: Iterate and destroy objects in the cTextures map
/*  for (unsigned int i = 0; i < cTextures.size(); i++) {
    delete cTextures[i];
  }*/
}

std::string Commodore64SpindizzyTextureSet::getName() {
  return "C64 Texture Set";
}

std::vector<PlugSocket*> Commodore64SpindizzyTextureSet::getPlugSockets() {
  return cPlugSockets;
}

void Commodore64SpindizzyTextureSet::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "FourColourSupport") {
    destroyTextures();
    cColourScheme->removeChangeListener(this);
    cColourScheme = dynamic_cast<IFourColourSupport*>(plugin);
    if (cColourScheme == NULL) {
      std::cout << "Warning: dynamic_cast failed for dummy four colour scheme!" << std::endl;
    } else {
      std::cout << "dynamic_cast succeeded for real four colour support!" << std::endl;
    }
    cColourScheme->addChangeListener(this);
    generateTextures();
  } else {
    // TODO: Throw something
  }
}

void Commodore64SpindizzyTextureSet::fourColourPaletteChanged(IFourColourSupport*) {
  destroyTextures();
  generateTextures();
}

IPlugin* Commodore64SpindizzyTextureSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "FourColourSupport") {
    return cColourScheme;
  } 
  // TODO: Throw something
  return NULL;
}

void Commodore64SpindizzyTextureSet::notifyZoneAction(Zone* zone) {
  // Nothing to do.
}

void Commodore64SpindizzyTextureSet::initPlugin(Zone* zone) {
  // Nothing to do.
}

void Commodore64SpindizzyTextureSet::setEditingInfo(IComponentContainer* componentContainer) {
  // We don't need to know.  Nothing to do.
}

ISpindizzyTexture* Commodore64SpindizzyTextureSet::getTexture(TextureType type) {
  return cTextures[type];
}

std::vector<ICommandInfo*> Commodore64SpindizzyTextureSet::getCommandInfo() {
  std::vector<ICommandInfo*> mEmptyVector;
  return mEmptyVector;
}

void Commodore64SpindizzyTextureSet::save(DOMNodeWriter* node) {
  // Nothing to do.
}

void Commodore64SpindizzyTextureSet::load(DOMNodeWrapper* node) {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new Commodore64SpindizzyTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

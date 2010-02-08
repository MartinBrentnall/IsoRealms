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
Image* Commodore64SpindizzyTextureSet::makePlainImage(Colour* colour) {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, false);
  mImage->drawSquare(colour, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::makeTileImage() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateIce() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generatePlain() {
  Image* mImage = makeTileImage();
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSplitPlain() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateTrampoline() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::BACKGROUND), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2);
  return convertToTexture(mImage);
}

Image* Commodore64SpindizzyTextureSet::makeSwitchSquareImage() {
  Image* mImage = makeTileImage();
  int mOuterSquare = (int) (RESOLUTION * 0.2);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchSquare() {
  Image* mImage = makeSwitchSquareImage();
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchSquareHalf() {
  Image* mImage = makeSwitchSquareImage();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchSquareBoth() {
  Image* mImage = makeSwitchSquareImage();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), mInnerSquare, RESOLUTION - mInnerSquare
                                                 , mInnerSquare, mHalfSquare
                                                 , mHalfSquare, RESOLUTION - mInnerSquare);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return convertToTexture(mImage);
}

Image* Commodore64SpindizzyTextureSet::makeSwitchDiamondImage() {
  Image* mImage = makeTileImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  mImage->drawDiamond(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterSquare);
  mImage->drawDiamond(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerSquare);
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchDiamond() {
  Image* mImage = makeSwitchDiamondImage();
  return convertToTexture(mImage);
}


GLuint Commodore64SpindizzyTextureSet::generateSwitchDiamondHalf() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchDiamondBoth() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, mInnerSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircle() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircleBoth() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircleHalf() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  mImage->drawSemiCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateArrow() {
  Image* mImage = makeTileImage();
  int mInnerPoint = (int) (RESOLUTION * 0.40);
  int mOuterPoint = (int) (RESOLUTION * 0.15);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterPoint, mHalfSquare, mInnerPoint, RESOLUTION - mInnerPoint);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::WALL), mHalfSquare, mOuterPoint, RESOLUTION - mOuterPoint, mHalfSquare, mHalfSquare, RESOLUTION - mOuterPoint);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateWallNorthSouth() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.70);
  Image* mImage = makePlainImage(&mGridColour);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateWallEastWest() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.85);
  Image* mImage = makePlainImage(&mGridColour);
  Colour mWallFloorMix(*(cColourScheme->getColour(IFourColourSupport::FLOOR)), *(cColourScheme->getColour(IFourColourSupport::WALL)));
  mImage->drawSquare(&mWallFloorMix, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateIceWall() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  int mIceWidth = (int) (RESOLUTION * 0.1);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), 0, RESOLUTION, RESOLUTION, RESOLUTION, mIceWidth, RESOLUTION - mIceWidth);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), mIceWidth, RESOLUTION - mIceWidth, RESOLUTION, RESOLUTION, RESOLUTION - mIceWidth, RESOLUTION - mIceWidth);
  return convertToTexture(mImage);
}

Image* Commodore64SpindizzyTextureSet::makeTransparent() {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, true);
  mImage->drawSquare(TRANSPARENT, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* Commodore64SpindizzyTextureSet::makeLiftSquareImage() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = (int) (mImage->getWidth() * 0.16);
  int mSquareOuterFill = (int) (mImage->getWidth() * 0.20);
  int mSquareInnerFill = (int) (mImage->getWidth() * 0.30);
  int mSquareInnerEdge = (int) (mImage->getWidth() * 0.34);

  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(YELLOW,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateLiftSquare() {
  Image* mImage = makeLiftSquareImage();
  return convertToTexture(mImage);
}

Image* Commodore64SpindizzyTextureSet::makeLiftSquareHalfImage() {
  Image* mImage = makeLiftSquareImage();
  mImage->drawTriangle(BLACK,  int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56), int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44));
  mImage->drawTriangle(PURPLE, int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateLiftSquareHalf() {
  Image* mImage = makeLiftSquareHalfImage();
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftSquareBoth() {
  Image* mImage = makeLiftSquareHalfImage();
  mImage->drawTriangle(BLACK,  int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44), int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56));
  mImage->drawTriangle(PURPLE, int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftDiamond() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawDiamond(BLACK,       mImage->getWidth());
  mImage->drawDiamond(YELLOW,      int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->getWidth() * 0.78));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftDiamondHalf() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawDiamond(BLACK,       mImage->getWidth());
  mImage->drawDiamond(YELLOW,      int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->getWidth() * 0.78));
  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawHalfDiamond2(BLACK,       int(mImage->getWidth() * 0.50));
  mImage->drawHalfDiamond( YELLOW,      int(mImage->getWidth() * 0.44));
  mImage->drawHalfDiamond( BLACK,       int(mImage->getWidth() * 0.34));
  mImage->drawHalfDiamond( TRANSPARENT, int(mImage->getWidth() * 0.28));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftDiamondBoth() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawDiamond(BLACK,       mImage->getWidth());
  mImage->drawDiamond(YELLOW,      int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(BLACK,       int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT, int(mImage->getWidth() * 0.78));
  mImage->drawSquare(BLACK,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(PURPLE,      mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(BLACK,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return convertToTexture(mImage);
}

Image* Commodore64SpindizzyTextureSet::makeLiftCircleImage() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(BLACK,       int(mImage->getWidth() * 0.9 * 0.5));
  mImage->drawCircle(PURPLE,      int(mImage->getWidth() * 0.9 * 0.46));
  mImage->drawCircle(BLACK,       int(mImage->getWidth() * 0.9 * 0.34));
  mImage->drawCircle(TRANSPARENT, int(mImage->getWidth() * 0.9 * 0.30));
  return mImage;
}

GLuint Commodore64SpindizzyTextureSet::generateLiftCircle() {
  Image* mImage = makeLiftCircleImage();
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftCircleHalf() {
  Image* mImage = makeLiftCircleImage();
  mImage->drawSemiCircle(YELLOW, int(mImage->getWidth() * 0.9 * 0.34));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::generateLiftCircleBoth() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(BLACK,  int(mImage->getWidth() * 0.9 * 0.5));
  mImage->drawCircle(PURPLE, int(mImage->getWidth() * 0.9 * 0.46));
  mImage->drawCircle(YELLOW, int(mImage->getWidth() * 0.9 * 0.34));
  return convertToTexture(mImage);
}

GLuint Commodore64SpindizzyTextureSet::convertToTexture(Image* image) {
  GLuint mTextureID = image->generateTexture();
  cTextureIDs.push_back(mTextureID);
  delete image;
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
  cTextures[SWITCH_CIRCLE_BOTH] = new Commodore64SpindizzyTexture(generateSwitchCircleBoth());
  GLuint mSwitchCircleHalf = generateSwitchCircleHalf();
  cTextures[SWITCH_CIRCLE_LEFT] = new Commodore64SpindizzyTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_CIRCLE_RIGHT] = new Commodore64SpindizzyTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_CIRCLE_NONE] = new Commodore64SpindizzyTexture(generateSwitchCircle());
  cTextures[SWITCH_SQUARE_BOTH] = new Commodore64SpindizzyTexture(generateSwitchSquareBoth());
  GLuint mSwitchSquareHalf = generateSwitchSquareHalf();
  cTextures[SWITCH_SQUARE_LEFT] = new Commodore64SpindizzyTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_SQUARE_RIGHT] = new Commodore64SpindizzyTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_SQUARE_NONE] = new Commodore64SpindizzyTexture(generateSwitchSquare());
  cTextures[SWITCH_DIAMOND_BOTH] = new Commodore64SpindizzyTexture(generateSwitchDiamondBoth());
  GLuint mSwitchDiamondHalf = generateSwitchDiamondHalf();
  cTextures[SWITCH_DIAMOND_LEFT] = new Commodore64SpindizzyTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_DIAMOND_RIGHT] = new Commodore64SpindizzyTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_DIAMOND_NONE] = new Commodore64SpindizzyTexture(generateSwitchDiamond());
  cTextures[SWITCH_RESET] = cTextures[SWITCH_DIAMOND_NONE];
  GLuint mArrowImage = generateArrow();
  cTextures[ARROW_NORTH] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::NORTH);
  cTextures[ARROW_EAST] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::EAST);
  cTextures[ARROW_SOUTH] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::SOUTH);
  cTextures[ARROW_WEST] = new Commodore64SpindizzyTexture(mArrowImage, Commodore64SpindizzyTexture::WEST);
  cTextures[TRAMPOLINE] = new Commodore64SpindizzyTexture(generateTrampoline());
  cTextures[ICE] = new Commodore64SpindizzyTexture(generateIce());
  cTextures[WATER] = cTextures[ICE];
  cTextures[PLAIN] = new Commodore64SpindizzyTexture(generatePlain());
  GLuint mSplitPlain = generateSplitPlain();
  cTextures[PLAIN_SPLIT_NE_SW] = new Commodore64SpindizzyTexture(mSplitPlain, Commodore64SpindizzyTexture::NORTH);
  cTextures[PLAIN_SPLIT_NW_SE] = new Commodore64SpindizzyTexture(mSplitPlain, Commodore64SpindizzyTexture::EAST);
  cTextures[WALL_NORTH] = new Commodore64SpindizzyTexture(generateWallNorthSouth(), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_EAST] = new Commodore64SpindizzyTexture(generateWallEastWest(), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_SOUTH] = cTextures[WALL_NORTH];
  cTextures[WALL_WEST] = cTextures[WALL_EAST];
  cTextures[WALL_ICE] = new Commodore64SpindizzyTexture(generateIceWall(), ISpindizzyTexture::TILED);

  GLuint mLiftCircleHalf = generateLiftCircleHalf();
  cTextures[LIFT_CIRCLE_LEFT] = new Commodore64SpindizzyTexture(mLiftCircleHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_CIRCLE_RIGHT] = new Commodore64SpindizzyTexture(mLiftCircleHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_CIRCLE_NONE] = new Commodore64SpindizzyTexture(generateLiftCircle());
  cTextures[LIFT_CIRCLE_BOTH] = new Commodore64SpindizzyTexture(generateLiftCircleBoth());
  GLuint mLiftSquareHalf = generateLiftSquareHalf();
  cTextures[LIFT_SQUARE_LEFT] = new Commodore64SpindizzyTexture(mLiftSquareHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_SQUARE_RIGHT] = new Commodore64SpindizzyTexture(mLiftSquareHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_SQUARE_NONE] = new Commodore64SpindizzyTexture(generateLiftSquare());
  cTextures[LIFT_SQUARE_BOTH] = new Commodore64SpindizzyTexture(generateLiftSquareBoth());
  GLuint mLiftDiamondHalf = generateLiftDiamondHalf();
  cTextures[LIFT_DIAMOND_LEFT] = new Commodore64SpindizzyTexture(mLiftDiamondHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[LIFT_DIAMOND_RIGHT] = new Commodore64SpindizzyTexture(mLiftDiamondHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[LIFT_DIAMOND_NONE] = new Commodore64SpindizzyTexture(generateLiftDiamond());
  cTextures[LIFT_DIAMOND_BOTH] = new Commodore64SpindizzyTexture(generateLiftDiamondBoth());
}

void Commodore64SpindizzyTextureSet::destroyTextures() {
  for (std::map<TextureType, ISpindizzyTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (i->first != SWITCH_RESET && i->first != WATER && i->first != WALL_SOUTH && i->first != WALL_WEST) {
      delete i->second;
    }
  }
  glDeleteTextures(cTextureIDs.size(), &cTextureIDs[0]);
  cTextureIDs.clear();
}

std::string Commodore64SpindizzyTextureSet::getName() {
  return "C64 Texture Set";
}

std::vector<PlugSocket*> Commodore64SpindizzyTextureSet::getPlugSockets() {
  return cPlugSockets;
}

void Commodore64SpindizzyTextureSet::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "FourColourSupport" && plugin != cColourScheme) { // TODO: Test against dummy (NULL)
    std::string mDummyName("FourColourSupport");
    IFourColourSupport* mPalette = dynamic_cast<IFourColourSupport*>(plugin == NULL ? PluginRegistry::getDummyPlugin(mDummyName) : plugin);
  
    if (mPalette == NULL) {
      std::cout << "Warning: dynamic_cast failed for dummy four colour scheme!" << std::endl;
    } else {
      destroyTextures();
      cColourScheme->removeChangeListener(this);
      cColourScheme = mPalette;
      cColourScheme->addChangeListener(this);
      generateTextures();
    }
  } else {
    // TODO: Throw something
  }
}

void Commodore64SpindizzyTextureSet::fourColourPaletteChanged(IFourColourSupport*) {
  // TODO: This isn't good enough because we can't garauntee that the generated texture ID's will match the destroyed ones.
  destroyTextures();
  generateTextures();
}

IPlugin* Commodore64SpindizzyTextureSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "FourColourSupport") {
    return PluginRegistry::isDummyPlugin(cColourScheme) ? NULL : cColourScheme;
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

Commodore64SpindizzyTextureSet::~Commodore64SpindizzyTextureSet() {
  delete TRANSPARENT;
  delete PURPLE;
  delete YELLOW;
  delete BLACK;
  for (unsigned int i = 0; i < cPlugSockets.size(); i++) {
    delete cPlugSockets[i];
  }
  destroyTextures();
}

extern "C" IPlugin* create() {
  return new Commodore64SpindizzyTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

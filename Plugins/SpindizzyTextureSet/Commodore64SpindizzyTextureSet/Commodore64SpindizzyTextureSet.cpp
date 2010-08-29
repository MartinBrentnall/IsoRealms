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

const std::string Commodore64SpindizzyTextureSet::PLAIN                = "Plain";
const std::string Commodore64SpindizzyTextureSet::PLAIN_NE_SPLIT       = "PlainNESplit";
const std::string Commodore64SpindizzyTextureSet::PLAIN_NW_SPLIT       = "PlainNWSplit";
const std::string Commodore64SpindizzyTextureSet::ICE                  = "Ice";
const std::string Commodore64SpindizzyTextureSet::ARROW_WEST           = "ArrowWest";
const std::string Commodore64SpindizzyTextureSet::ARROW_EAST           = "ArrowEast";
const std::string Commodore64SpindizzyTextureSet::ARROW_SOUTH          = "ArrowSouth";
const std::string Commodore64SpindizzyTextureSet::ARROW_NORTH          = "ArrowNorth";
const std::string Commodore64SpindizzyTextureSet::SWITCH_CIRCLE_BOTH   = "SwitchCircleFilled";
const std::string Commodore64SpindizzyTextureSet::SWITCH_CIRCLE_LEFT   = "SwitchCircleLeft";
const std::string Commodore64SpindizzyTextureSet::SWITCH_CIRCLE_RIGHT  = "SwitchCircleRight";
const std::string Commodore64SpindizzyTextureSet::SWITCH_CIRCLE_NONE   = "SwitchCircleEmpty";
const std::string Commodore64SpindizzyTextureSet::SWITCH_SQUARE_BOTH   = "SwitchSquareFilled";
const std::string Commodore64SpindizzyTextureSet::SWITCH_SQUARE_LEFT   = "SwitchSquareLeft";
const std::string Commodore64SpindizzyTextureSet::SWITCH_SQUARE_RIGHT  = "SwitchSquareRight";
const std::string Commodore64SpindizzyTextureSet::SWITCH_SQUARE_NONE   = "SwitchSquareEmpty";
const std::string Commodore64SpindizzyTextureSet::SWITCH_DIAMOND_BOTH  = "SwitchDiamondFilled";
const std::string Commodore64SpindizzyTextureSet::SWITCH_DIAMOND_LEFT  = "SwitchDiamondLeft";
const std::string Commodore64SpindizzyTextureSet::SWITCH_DIAMOND_RIGHT = "SwitchDiamondRight";
const std::string Commodore64SpindizzyTextureSet::SWITCH_DIAMOND_NONE  = "SwitchDiamondEmpty";
const std::string Commodore64SpindizzyTextureSet::TRAMPOLINE           = "Trampoline";
const std::string Commodore64SpindizzyTextureSet::WATER                = "Water";
const std::string Commodore64SpindizzyTextureSet::WALL_ICE             = "WallIce";
const std::string Commodore64SpindizzyTextureSet::WALL_WEST            = "WallWest";
const std::string Commodore64SpindizzyTextureSet::WALL_EAST            = "WallEast";
const std::string Commodore64SpindizzyTextureSet::WALL_SOUTH           = "WallSouth";
const std::string Commodore64SpindizzyTextureSet::WALL_NORTH           = "WallNorth";

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
  return convertToTexture(mImage, ICE);
}

GLuint Commodore64SpindizzyTextureSet::generatePlain() {
  Image* mImage = makeTileImage();
  return convertToTexture(mImage, PLAIN);
}

GLuint Commodore64SpindizzyTextureSet::generateSplitPlain() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2));
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::FLOOR), RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage, PLAIN_NE_SPLIT);
}

GLuint Commodore64SpindizzyTextureSet::generateTrampoline() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::BACKGROUND), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2);
  return convertToTexture(mImage, TRAMPOLINE);
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
  return convertToTexture(mImage, SWITCH_SQUARE_NONE);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchSquareHalf() {
  Image* mImage = makeSwitchSquareImage();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::EXTRA), RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return convertToTexture(mImage, SWITCH_SQUARE_LEFT);
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
  return convertToTexture(mImage, SWITCH_SQUARE_BOTH);
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
  return convertToTexture(mImage, SWITCH_DIAMOND_NONE);
}


GLuint Commodore64SpindizzyTextureSet::generateSwitchDiamondHalf() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage, SWITCH_DIAMOND_LEFT);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchDiamondBoth() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, mInnerSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage, SWITCH_DIAMOND_BOTH);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircle() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_NONE);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircleBoth() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_BOTH);
}

GLuint Commodore64SpindizzyTextureSet::generateSwitchCircleHalf() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::EXTRA), mOuterCircle);
  mImage->drawCircle(cColourScheme->getColour(IFourColourSupport::WALL), mInnerCircle);
  mImage->drawSemiCircle(cColourScheme->getColour(IFourColourSupport::FLOOR), mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_LEFT);
}

GLuint Commodore64SpindizzyTextureSet::generateArrow() {
  Image* mImage = makeTileImage();
  int mInnerPoint = (int) (RESOLUTION * 0.40);
  int mOuterPoint = (int) (RESOLUTION * 0.15);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), mOuterPoint, mHalfSquare, mInnerPoint, RESOLUTION - mInnerPoint);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::WALL), mHalfSquare, mOuterPoint, RESOLUTION - mOuterPoint, mHalfSquare, mHalfSquare, RESOLUTION - mOuterPoint);
  return convertToTexture(mImage, ARROW_EAST);
}

GLuint Commodore64SpindizzyTextureSet::generateWallNorthSouth() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.70);
  Image* mImage = makePlainImage(&mGridColour);
  mImage->drawSquare(cColourScheme->getColour(IFourColourSupport::WALL), GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage, WALL_NORTH);
}

GLuint Commodore64SpindizzyTextureSet::generateWallEastWest() {
  Colour mGridColour = cColourScheme->getColour(IFourColourSupport::EXTRA)->brightness(0.85);
  Image* mImage = makePlainImage(&mGridColour);
  Colour mWallFloorMix(*(cColourScheme->getColour(IFourColourSupport::FLOOR)), *(cColourScheme->getColour(IFourColourSupport::WALL)));
  mImage->drawSquare(&mWallFloorMix, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage, WALL_EAST);
}

GLuint Commodore64SpindizzyTextureSet::generateIceWall() {
  Image* mImage = makePlainImage(cColourScheme->getColour(IFourColourSupport::EXTRA));
  int mIceWidth = (int) (RESOLUTION * 0.1);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), 0, RESOLUTION, RESOLUTION, RESOLUTION, mIceWidth, RESOLUTION - mIceWidth);
  mImage->drawTriangle(cColourScheme->getColour(IFourColourSupport::BACKGROUND), mIceWidth, RESOLUTION - mIceWidth, RESOLUTION, RESOLUTION, RESOLUTION - mIceWidth, RESOLUTION - mIceWidth);
  return convertToTexture(mImage, WALL_ICE);
}

GLuint Commodore64SpindizzyTextureSet::convertToTexture(Image* image, const std::string& type) {
  std::map<std::string, GLuint>::iterator i = cTextureIDs.find(type);
  GLuint mTextureID;
  if (i == cTextureIDs.end()) {
    mTextureID = image->generateTexture();
    cTextureIDs[type] = mTextureID;
  } else {
    mTextureID = i->second;
    image->generateTexture(mTextureID);
  }
  delete image;
  return mTextureID;
}

Commodore64SpindizzyTextureSet::Commodore64SpindizzyTextureSet() {
  cPlugSockets.push_back(new PlugSocket("FourColourSupport"));
  assignDummyPlugin(&cColourScheme, "FourColourSupport");

  cTextures[SWITCH_CIRCLE_BOTH]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_CIRCLE_LEFT]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_CIRCLE_RIGHT]  = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_CIRCLE_NONE]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_BOTH]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_RIGHT]  = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_LEFT]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_NONE]   = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_NONE]  = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_RIGHT] = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_LEFT]  = new Commodore64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_BOTH]  = new Commodore64SpindizzyTexture();
  cTextures[ARROW_NORTH]          = new Commodore64SpindizzyTexture();
  cTextures[ARROW_EAST]           = new Commodore64SpindizzyTexture();
  cTextures[ARROW_SOUTH]          = new Commodore64SpindizzyTexture();
  cTextures[ARROW_WEST]           = new Commodore64SpindizzyTexture();
  cTextures[TRAMPOLINE]           = new Commodore64SpindizzyTexture();
  cTextures[ICE]                  = new Commodore64SpindizzyTexture();
  cTextures[PLAIN]                = new Commodore64SpindizzyTexture();
  cTextures[PLAIN_NE_SPLIT]       = new Commodore64SpindizzyTexture();
  cTextures[PLAIN_NW_SPLIT]       = new Commodore64SpindizzyTexture();
  cTextures[WALL_NORTH]           = new Commodore64SpindizzyTexture();
  cTextures[WALL_EAST]            = new Commodore64SpindizzyTexture();
  cTextures[WALL_ICE]             = new Commodore64SpindizzyTexture();
  
  cBackgroundTexture = new PlainColourTexture();

  cTextures[WALL_SOUTH]   = cTextures[WALL_NORTH];
  cTextures[WALL_WEST]    = cTextures[WALL_EAST];
  cTextures[WATER]        = cTextures[ICE];

  generateTextures();
}

void Commodore64SpindizzyTextureSet::generateTextures() {
  // TODO: Clean-up old ones
  cTextures[SWITCH_CIRCLE_BOTH]->setTexture(generateSwitchCircleBoth());
  GLuint mSwitchCircleHalf = generateSwitchCircleHalf();
  cTextures[SWITCH_CIRCLE_RIGHT]->setTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_CIRCLE_LEFT]->setTexture(mSwitchCircleHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_CIRCLE_NONE]->setTexture(generateSwitchCircle());
  cTextures[SWITCH_SQUARE_BOTH]->setTexture(generateSwitchSquareBoth());
  GLuint mSwitchSquareHalf = generateSwitchSquareHalf();
  cTextures[SWITCH_SQUARE_RIGHT]->setTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_SQUARE_LEFT]->setTexture(mSwitchSquareHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_SQUARE_NONE]->setTexture(generateSwitchSquare());
  cTextures[SWITCH_DIAMOND_BOTH]->setTexture(generateSwitchDiamondBoth());
  GLuint mSwitchDiamondHalf = generateSwitchDiamondHalf();
  cTextures[SWITCH_DIAMOND_RIGHT]->setTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::NORTH);
  cTextures[SWITCH_DIAMOND_LEFT]->setTexture(mSwitchDiamondHalf, Commodore64SpindizzyTexture::SOUTH);
  cTextures[SWITCH_DIAMOND_NONE]->setTexture(generateSwitchDiamond());
  GLuint mArrowImage = generateArrow();
  cTextures[ARROW_NORTH]->setTexture(mArrowImage, Commodore64SpindizzyTexture::NORTH);
  cTextures[ARROW_EAST]->setTexture(mArrowImage, Commodore64SpindizzyTexture::EAST);
  cTextures[ARROW_SOUTH]->setTexture(mArrowImage, Commodore64SpindizzyTexture::SOUTH);
  cTextures[ARROW_WEST]->setTexture(mArrowImage, Commodore64SpindizzyTexture::WEST);
  cTextures[TRAMPOLINE]->setTexture(generateTrampoline());
  cTextures[ICE]->setTexture(generateIce());
  cTextures[WATER] = cTextures[ICE];
  cTextures[PLAIN]->setTexture(generatePlain());
  GLuint mSplitPlain = generateSplitPlain();
  cTextures[PLAIN_NE_SPLIT]->setTexture(mSplitPlain, Commodore64SpindizzyTexture::NORTH);
  cTextures[PLAIN_NW_SPLIT]->setTexture(mSplitPlain, Commodore64SpindizzyTexture::EAST);
  cTextures[WALL_NORTH]->setTexture(generateWallNorthSouth(), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_EAST]->setTexture(generateWallEastWest(), ISpindizzyTexture::COLUMN_CAPPED);
  cTextures[WALL_SOUTH] = cTextures[WALL_NORTH];
  cTextures[WALL_WEST] = cTextures[WALL_EAST];
  cTextures[WALL_ICE]->setTexture(generateIceWall(), ISpindizzyTexture::TILED);

  cBackgroundTexture->setTexture(cColourScheme->getColour(IFourColourSupport::BACKGROUND));
}

void Commodore64SpindizzyTextureSet::destroyTextures() {
  for (std::map<std::string, Commodore64SpindizzyTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (i->first != WATER && i->first != WALL_SOUTH && i->first != WALL_WEST) {
      delete i->second;
    }
  }
  delete cBackgroundTexture;
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

std::vector<PlugSocket*> Commodore64SpindizzyTextureSet::getPlugSockets() {
  return cPlugSockets;
}

void Commodore64SpindizzyTextureSet::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "FourColourSupport") {
    IFourColourSupport* mPreviousColourScheme = cColourScheme;
    if (assignPlugin(plugin, &cColourScheme, *socket)) {
      mPreviousColourScheme->removeChangeListener(this);
      cColourScheme->addChangeListener(this);
      generateTextures();
    }
  } else {
    // TODO: Throw something
  }
}

void Commodore64SpindizzyTextureSet::fourColourPaletteChanged(IFourColourSupport*) {
  generateTextures();
}

IPlugin* Commodore64SpindizzyTextureSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "FourColourSupport") {return cColourScheme;}
  // TODO: Throw something
  return NULL;
}

ISpindizzyTexture* Commodore64SpindizzyTextureSet::getTexture(const std::string& name) {
  if (name == "Background") {
    return cBackgroundTexture;
  }
  std::map<std::string, Commodore64SpindizzyTexture*>::iterator i = cTextures.find(name);
  return i != cTextures.end() ? i->second : cTextures.begin()->second;
}

Commodore64SpindizzyTextureSet::~Commodore64SpindizzyTextureSet() {
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

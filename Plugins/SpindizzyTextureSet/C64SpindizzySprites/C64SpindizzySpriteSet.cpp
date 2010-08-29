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
#include "C64SpindizzySpriteSet.h"

const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_BOTH   = "LiftCircleFilled";
const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_LEFT   = "LiftCircleLeft";
const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_RIGHT  = "LiftCircleRight";
const std::string C64SpindizzySpriteSet::LIFT_CIRCLE_NONE   = "LiftCircleEmpty";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_BOTH   = "LiftSquareFilled";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_LEFT   = "LiftSquareLeft";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_RIGHT  = "LiftSquareRight";
const std::string C64SpindizzySpriteSet::LIFT_SQUARE_NONE   = "LiftSquareEmpty";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_BOTH  = "LiftDiamondFilled";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_LEFT  = "LiftDiamondLeft";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_RIGHT = "LiftDiamondRight";
const std::string C64SpindizzySpriteSet::LIFT_DIAMOND_NONE  = "LiftDiamondEmpty";

C64SpindizzySpriteSet::C64SpindizzySpriteSet() {
  cPlugSockets.push_back(new PlugSocket("FourColourSupport"));
  assignDummyPlugin(&cColourScheme, "FourColourSupport");

  TRANSPARENT = new Colour(0.0, 0.0, 0.0, 0.0);

  cTextures[LIFT_CIRCLE_LEFT]   = new C64SpindizzySprite();
  cTextures[LIFT_CIRCLE_RIGHT]  = new C64SpindizzySprite();
  cTextures[LIFT_CIRCLE_NONE]   = new C64SpindizzySprite();
  cTextures[LIFT_CIRCLE_BOTH]   = new C64SpindizzySprite();
  cTextures[LIFT_SQUARE_LEFT]   = new C64SpindizzySprite();
  cTextures[LIFT_SQUARE_RIGHT]  = new C64SpindizzySprite();
  cTextures[LIFT_SQUARE_NONE]   = new C64SpindizzySprite();
  cTextures[LIFT_SQUARE_BOTH]   = new C64SpindizzySprite();
  cTextures[LIFT_DIAMOND_LEFT]  = new C64SpindizzySprite();
  cTextures[LIFT_DIAMOND_RIGHT] = new C64SpindizzySprite();
  cTextures[LIFT_DIAMOND_NONE]  = new C64SpindizzySprite();
  cTextures[LIFT_DIAMOND_BOTH]  = new C64SpindizzySprite();
  
  generateTextures();
}

/* Generation functions */
Image* C64SpindizzySpriteSet::makePlainImage(Colour* colour) {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, false);
  mImage->drawSquare(colour, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* C64SpindizzySpriteSet::makeTileImage() {
  Image* mImage = makePlainImage(cOutlineColour);
  mImage->drawSquare(cColour1, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

Image* C64SpindizzySpriteSet::makeTransparent() {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, true);
  mImage->drawSquare(TRANSPARENT, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* C64SpindizzySpriteSet::makeLiftSquareImage() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = (int) (mImage->getWidth() * 0.16);
  int mSquareOuterFill = (int) (mImage->getWidth() * 0.20);
  int mSquareInnerFill = (int) (mImage->getWidth() * 0.30);
  int mSquareInnerEdge = (int) (mImage->getWidth() * 0.34);

  mImage->drawSquare(cOutlineColour, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(cColour2,       mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(cOutlineColour, mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT,    mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return mImage;
}

GLuint C64SpindizzySpriteSet::generateLiftSquare() {
  Image* mImage = makeLiftSquareImage();
  return convertToTexture(mImage, LIFT_SQUARE_NONE);
}

Image* C64SpindizzySpriteSet::makeLiftSquareHalfImage() {
  Image* mImage = makeLiftSquareImage();
  mImage->drawTriangle(cOutlineColour, int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56), int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44));
  mImage->drawTriangle(cColour1,       int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return mImage;
}

GLuint C64SpindizzySpriteSet::generateLiftSquareHalf() {
  Image* mImage = makeLiftSquareHalfImage();
  return convertToTexture(mImage, LIFT_SQUARE_LEFT);
}

GLuint C64SpindizzySpriteSet::generateLiftSquareBoth() {
  Image* mImage = makeLiftSquareHalfImage();
  mImage->drawTriangle(cOutlineColour, int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.44), int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.56));
  mImage->drawTriangle(cColour1,       int(RESOLUTION - EDGE_WIDTH * 2.4), int(EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5),  int(EDGE_WIDTH * 2.4), int(RESOLUTION - EDGE_WIDTH * 2.4), int(RESOLUTION * 0.5));
  return convertToTexture(mImage, LIFT_SQUARE_BOTH);
}

GLuint C64SpindizzySpriteSet::generateLiftDiamondBoth() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawSquare(cOutlineColour,  mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(cColour1,        mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(cOutlineColour,  mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT,     mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawDiamond(cOutlineColour, mImage->getWidth());
  mImage->drawDiamond(cColour2,       int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(cOutlineColour, int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT,    int(mImage->getWidth() * 0.78));
  return convertToTexture(mImage, LIFT_DIAMOND_BOTH);
}

GLuint C64SpindizzySpriteSet::generateLiftDiamondHalf() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawDiamond(cOutlineColour,      mImage->getWidth());
  mImage->drawDiamond(cColour2,            int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(cOutlineColour,      int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT,         int(mImage->getWidth() * 0.78));
  mImage->drawSquare(cOutlineColour,       mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(cColour1,             mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(cOutlineColour,       mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT,          mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  mImage->drawHalfDiamond2(cOutlineColour, int(mImage->getWidth() * 0.50));
  mImage->drawHalfDiamond( cColour2,       int(mImage->getWidth() * 0.44));
  mImage->drawHalfDiamond( cOutlineColour, int(mImage->getWidth() * 0.34));
  mImage->drawHalfDiamond( TRANSPARENT,    int(mImage->getWidth() * 0.28));
  return convertToTexture(mImage, LIFT_DIAMOND_LEFT);
}

GLuint C64SpindizzySpriteSet::generateLiftDiamond() {
  Image* mImage = makeTransparent();
  int mSquareOuterEdge = int(mImage->getWidth() * 0.16);
  int mSquareOuterFill = int(mImage->getWidth() * 0.20);
  int mSquareInnerFill = int(mImage->getWidth() * 0.26);
  int mSquareInnerEdge = int(mImage->getWidth() * 0.30);

  mImage->drawDiamond(cOutlineColour, mImage->getWidth());
  mImage->drawDiamond(cColour2,       int(mImage->getWidth() * 0.94));
  mImage->drawDiamond(cOutlineColour, int(mImage->getWidth() * 0.84));
  mImage->drawDiamond(TRANSPARENT,    int(mImage->getWidth() * 0.78));
  mImage->drawSquare(cOutlineColour,  mSquareOuterEdge, RESOLUTION - mSquareOuterEdge, mSquareOuterEdge, RESOLUTION - mSquareOuterEdge);
  mImage->drawSquare(cColour1,        mSquareOuterFill, RESOLUTION - mSquareOuterFill, mSquareOuterFill, RESOLUTION - mSquareOuterFill);
  mImage->drawSquare(cOutlineColour,  mSquareInnerFill, RESOLUTION - mSquareInnerFill, mSquareInnerFill, RESOLUTION - mSquareInnerFill);
  mImage->drawSquare(TRANSPARENT,     mSquareInnerEdge, RESOLUTION - mSquareInnerEdge, mSquareInnerEdge, RESOLUTION - mSquareInnerEdge);
  return convertToTexture(mImage, LIFT_DIAMOND_NONE);
}

Image* C64SpindizzySpriteSet::makeLiftCircleImage() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(cOutlineColour, int(mImage->getWidth() * 0.9 * 0.5));
  mImage->drawCircle(cColour1,       int(mImage->getWidth() * 0.9 * 0.46));
  mImage->drawCircle(cOutlineColour, int(mImage->getWidth() * 0.9 * 0.34));
  mImage->drawCircle(TRANSPARENT,    int(mImage->getWidth() * 0.9 * 0.30));
  return mImage;
}

GLuint C64SpindizzySpriteSet::generateLiftCircle() {
  Image* mImage = makeLiftCircleImage();
  return convertToTexture(mImage, LIFT_CIRCLE_NONE);
}

GLuint C64SpindizzySpriteSet::generateLiftCircleHalf() {
  Image* mImage = makeLiftCircleImage();
  mImage->drawSemiCircle(cColour2, int(mImage->getWidth() * 0.9 * 0.34));
  return convertToTexture(mImage, LIFT_CIRCLE_LEFT);
}

GLuint C64SpindizzySpriteSet::generateLiftCircleBoth() {
  Image* mImage = makeTransparent();
  mImage->drawCircle(cOutlineColour, int(mImage->getWidth() * 0.9 * 0.5));
  mImage->drawCircle(cColour1,      int(mImage->getWidth() * 0.9 * 0.46));
  mImage->drawCircle(cColour2,       int(mImage->getWidth() * 0.9 * 0.34));
  return convertToTexture(mImage, LIFT_CIRCLE_BOTH);
}

GLuint C64SpindizzySpriteSet::convertToTexture(Image* image, const std::string& type) {
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

void C64SpindizzySpriteSet::generateTextures() {
  cColour1 = cColourScheme->getColour(cColour1Name);
  cColour2 = cColourScheme->getColour(cColour2Name);
  cColour3 = cColourScheme->getColour(cColour3Name);
  cOutlineColour = cColourScheme->getColour(cOutlineColourName);
  
  GLuint mLiftCircleHalf = generateLiftCircleHalf();
  cTextures[LIFT_CIRCLE_LEFT]->setTexture(mLiftCircleHalf, C64SpindizzySprite::NORTH);
  cTextures[LIFT_CIRCLE_RIGHT]->setTexture(mLiftCircleHalf, C64SpindizzySprite::SOUTH);
  cTextures[LIFT_CIRCLE_NONE]->setTexture(generateLiftCircle());
  cTextures[LIFT_CIRCLE_BOTH]->setTexture(generateLiftCircleBoth());
  GLuint mLiftSquareHalf = generateLiftSquareHalf();
  cTextures[LIFT_SQUARE_LEFT]->setTexture(mLiftSquareHalf, C64SpindizzySprite::NORTH);
  cTextures[LIFT_SQUARE_RIGHT]->setTexture(mLiftSquareHalf, C64SpindizzySprite::SOUTH);
  cTextures[LIFT_SQUARE_NONE]->setTexture(generateLiftSquare());
  cTextures[LIFT_SQUARE_BOTH]->setTexture(generateLiftSquareBoth());
  GLuint mLiftDiamondHalf = generateLiftDiamondHalf();
  cTextures[LIFT_DIAMOND_LEFT]->setTexture(mLiftDiamondHalf, C64SpindizzySprite::SOUTH);
  cTextures[LIFT_DIAMOND_RIGHT]->setTexture(mLiftDiamondHalf, C64SpindizzySprite::NORTH);
  cTextures[LIFT_DIAMOND_NONE]->setTexture(generateLiftDiamond());
  cTextures[LIFT_DIAMOND_BOTH]->setTexture(generateLiftDiamondBoth());
}

void C64SpindizzySpriteSet::destroyTextures() {
  for (std::map<std::string, C64SpindizzySprite*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

std::vector<PlugSocket*> C64SpindizzySpriteSet::getPlugSockets() {
  return cPlugSockets;
}

void C64SpindizzySpriteSet::setPlugin(PlugSocket* socket, IPlugin* plugin) {
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

void C64SpindizzySpriteSet::fourColourPaletteChanged(IFourColourSupport*, const std::string&) {
  generateTextures();
}

IPlugin* C64SpindizzySpriteSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "FourColourSupport") {return cColourScheme;}
  // TODO: Throw something
  return NULL;
}

ISpindizzyTexture* C64SpindizzySpriteSet::getTexture(const std::string& name) {
  std::map<std::string, C64SpindizzySprite*>::iterator i = cTextures.find(name);
  return i != cTextures.end() ? i->second : cTextures.begin()->second;
}

void C64SpindizzySpriteSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Colour1") {
      cColour1Name = mNode->getAttribute("name");
    } else if (mValueAsString == "Colour2") {
      cColour2Name = mNode->getAttribute("name");
    } else if (mValueAsString == "Colour3") {
      cColour3Name = mNode->getAttribute("name");
    } else if (mValueAsString == "Outline") {
      cOutlineColourName = mNode->getAttribute("name");
    } else {
      // TODO: Throw something!
    }
  }
  generateTextures();
}

C64SpindizzySpriteSet::~C64SpindizzySpriteSet() {
  delete TRANSPARENT;
  for (unsigned int i = 0; i < cPlugSockets.size(); i++) {
    delete cPlugSockets[i];
  }
  destroyTextures();
}

extern "C" IPlugin* create() {
  return new C64SpindizzySpriteSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

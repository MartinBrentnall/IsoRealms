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
#include "C64SpindizzyTextureSet.h"

const std::string C64SpindizzyTextureSet::PLAIN                = "Plain";
const std::string C64SpindizzyTextureSet::PLAIN_SPLIT          = "PlainSplit";
const std::string C64SpindizzyTextureSet::ICE_WATER            = "IceWater";
const std::string C64SpindizzyTextureSet::ARROW                = "Arrow";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_BOTH   = "SwitchCircleFilled";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_ONE    = "SwitchCircleOne";
const std::string C64SpindizzyTextureSet::SWITCH_CIRCLE_NONE   = "SwitchCircleEmpty";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_BOTH   = "SwitchSquareFilled";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_ONE    = "SwitchSquareOne";
const std::string C64SpindizzyTextureSet::SWITCH_SQUARE_NONE   = "SwitchSquareEmpty";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_BOTH  = "SwitchDiamondFilled";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_ONE   = "SwitchDiamondOne";
const std::string C64SpindizzyTextureSet::SWITCH_DIAMOND_NONE  = "SwitchDiamondEmpty";
const std::string C64SpindizzyTextureSet::TRAMPOLINE           = "Trampoline";
const std::string C64SpindizzyTextureSet::WALL_ICE             = "WallIce";
const std::string C64SpindizzyTextureSet::WALL_MIXED_CAP       = "WallMixedCap";
const std::string C64SpindizzyTextureSet::WALL_MIXED_MIDDLE    = "WallMixedMiddle";
const std::string C64SpindizzyTextureSet::WALL_PLAIN_CAP       = "WallPlainCap";
const std::string C64SpindizzyTextureSet::WALL_PLAIN_MIDDLE    = "WallPlainMiddle";

/* Generation functions */
Image* C64SpindizzyTextureSet::makePlainImage(Colour* colour) {
  Image* mImage = new Image(RESOLUTION, RESOLUTION, false);
  mImage->drawSquare(colour, 0, RESOLUTION, 0, RESOLUTION);
  return mImage;
}

Image* C64SpindizzyTextureSet::makeTileImage() {
  Image* mImage = makePlainImage(cGridColour);
  mImage->drawSquare(cFloorColour, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  return mImage;
}

GLuint C64SpindizzyTextureSet::generateIce() {
  Image* mImage = makePlainImage(cGridColour);
  return convertToTexture(mImage, ICE_WATER);
}

GLuint C64SpindizzyTextureSet::generatePlain() {
  Image* mImage = makeTileImage();
  return convertToTexture(mImage, PLAIN);
}

GLuint C64SpindizzyTextureSet::generateSplitPlain() {
  Image* mImage = makePlainImage(cGridColour);
  mImage->drawTriangle(cFloorColour, GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2), GRID_WIDTH, GRID_WIDTH, RESOLUTION - (GRID_WIDTH * 2));
  mImage->drawTriangle(cFloorColour, RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH);
  return convertToTexture(mImage, PLAIN_SPLIT);
}

GLuint C64SpindizzyTextureSet::generateTrampoline() {
  Image* mImage = makePlainImage(cGridColour);
  mImage->drawSquare(cBackgroundColour, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, RESOLUTION - GRID_WIDTH);
  mImage->drawSquare(cWallColour, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2, GRID_WIDTH * 2, RESOLUTION - GRID_WIDTH * 2);
  return convertToTexture(mImage, TRAMPOLINE);
}

Image* C64SpindizzyTextureSet::makeSwitchSquareImage() {
  Image* mImage = makeTileImage();
  int mOuterSquare = (int) (RESOLUTION * 0.2);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cWallColour, mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cFloorColour, mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  return mImage;
}

GLuint C64SpindizzyTextureSet::generateSwitchSquare() {
  Image* mImage = makeSwitchSquareImage();
  return convertToTexture(mImage, SWITCH_SQUARE_NONE);
}

GLuint C64SpindizzyTextureSet::generateSwitchSquareHalf() {
  Image* mImage = makeSwitchSquareImage();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cGridColour, RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return convertToTexture(mImage, SWITCH_SQUARE_ONE);
}

GLuint C64SpindizzyTextureSet::generateSwitchSquareBoth() {
  Image* mImage = makeSwitchSquareImage();
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawTriangle(cGridColour, mInnerSquare, RESOLUTION - mInnerSquare
                                                 , mInnerSquare, mHalfSquare
                                                 , mHalfSquare, RESOLUTION - mInnerSquare);
  mImage->drawTriangle(cGridColour, RESOLUTION - mInnerSquare, mInnerSquare
                                                 , RESOLUTION - mInnerSquare, mHalfSquare
                                                 , mHalfSquare, mInnerSquare);
  return convertToTexture(mImage, SWITCH_SQUARE_BOTH);
}

Image* C64SpindizzyTextureSet::makeSwitchDiamondImage() {
  Image* mImage = makeTileImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cWallColour, mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cFloorColour, mInnerSquare, RESOLUTION - mInnerSquare, mInnerSquare, RESOLUTION - mInnerSquare);
  mImage->drawDiamond(cGridColour, mOuterSquare);
  mImage->drawDiamond(cFloorColour, mInnerSquare);
  return mImage;
}

GLuint C64SpindizzyTextureSet::generateSwitchDiamond() {
  Image* mImage = makeSwitchDiamondImage();
  return convertToTexture(mImage, SWITCH_DIAMOND_NONE);
}


GLuint C64SpindizzyTextureSet::generateSwitchDiamondHalf() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cWallColour, mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cWallColour, mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage, SWITCH_DIAMOND_ONE);
}

GLuint C64SpindizzyTextureSet::generateSwitchDiamondBoth() {
  Image* mImage = makeSwitchDiamondImage();
  int mOuterSquare = (int) (RESOLUTION * 0.175);
  int mInnerSquare = (int) (RESOLUTION * 0.3);
  mImage->drawSquare(cWallColour, mOuterSquare, RESOLUTION - mOuterSquare, mOuterSquare, mInnerSquare);
  mImage->drawSquare(cWallColour, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cWallColour, mOuterSquare, mInnerSquare, mOuterSquare, RESOLUTION - mOuterSquare);
  mImage->drawSquare(cWallColour, mOuterSquare, RESOLUTION - mOuterSquare, RESOLUTION - mInnerSquare, RESOLUTION - mOuterSquare);
  return convertToTexture(mImage, SWITCH_DIAMOND_BOTH);
}

GLuint C64SpindizzyTextureSet::generateSwitchCircle() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cGridColour, mOuterCircle);
  mImage->drawCircle(cFloorColour, mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_NONE);
}

GLuint C64SpindizzyTextureSet::generateSwitchCircleBoth() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cGridColour, mOuterCircle);
  mImage->drawCircle(cWallColour, mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_BOTH);
}

GLuint C64SpindizzyTextureSet::generateSwitchCircleHalf() {
  Image* mImage = makeTileImage();
  int mOuterCircle = (int) (RESOLUTION * 0.33);
  int mInnerCircle = (int) (RESOLUTION * 0.24);
  mImage->drawCircle(cGridColour, mOuterCircle);
  mImage->drawCircle(cWallColour, mInnerCircle);
  mImage->drawSemiCircle(cFloorColour, mInnerCircle);
  return convertToTexture(mImage, SWITCH_CIRCLE_ONE);
}

GLuint C64SpindizzyTextureSet::generateArrow() {
  Image* mImage = makeTileImage();
  int mInnerPoint = (int) (RESOLUTION * 0.40);
  int mOuterPoint = (int) (RESOLUTION * 0.15);
  int mHalfSquare = (int) (RESOLUTION * 0.5);
  mImage->drawSquare(cWallColour, mOuterPoint, mHalfSquare, mInnerPoint, RESOLUTION - mInnerPoint);
  mImage->drawTriangle(cWallColour, mHalfSquare, mOuterPoint, RESOLUTION - mOuterPoint, mHalfSquare, mHalfSquare, RESOLUTION - mOuterPoint);
  return convertToTexture(mImage, ARROW);
}

GLuint C64SpindizzyTextureSet::generateWallPlainMiddle() {
  Colour mGridColour = cGridColour->brightness(0.70);
  int mHeight = RESOLUTION / 2;
  Image* mImage = new Image(RESOLUTION, mHeight, false);
  mImage->drawSquare(&mGridColour, 0, RESOLUTION, 0, mHeight);
  mImage->drawSquare(cWallColour, GRID_WIDTH, RESOLUTION - GRID_WIDTH, 0, mHeight);
  return convertToTexture(mImage, WALL_PLAIN_MIDDLE);
}

GLuint C64SpindizzyTextureSet::generateWallMixedMiddle() {
  Colour mGridColour = cGridColour->brightness(0.85);
  Colour mWallFloorMix(*cFloorColour, *(cWallColour));
  int mHeight = RESOLUTION / 2;
  Image* mImage = new Image(RESOLUTION, mHeight, false);
  mImage->drawSquare(&mGridColour, 0, RESOLUTION, 0, mHeight);
  mImage->drawSquare(&mWallFloorMix, GRID_WIDTH, RESOLUTION - GRID_WIDTH, 0, mHeight);
  return convertToTexture(mImage, WALL_MIXED_MIDDLE);
}

GLuint C64SpindizzyTextureSet::generateWallPlainCap() {
  Colour mGridColour = cGridColour->brightness(0.70);
  int mHeight = RESOLUTION / 4;
  Image* mImage = new Image(RESOLUTION, mHeight, false);
  mImage->drawSquare(&mGridColour, 0, RESOLUTION, 0, mHeight);
  mImage->drawSquare(cWallColour, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, mHeight);
  return convertToTexture(mImage, WALL_PLAIN_CAP);
}

GLuint C64SpindizzyTextureSet::generateWallMixedCap() {
  Colour mGridColour = cGridColour->brightness(0.85);
  Colour mWallFloorMix(*cFloorColour, *(cWallColour));
  int mHeight = RESOLUTION / 4;
  Image* mImage = new Image(RESOLUTION, mHeight, false);
  mImage->drawSquare(&mGridColour, 0, RESOLUTION, 0, mHeight);
  mImage->drawSquare(&mWallFloorMix, GRID_WIDTH, RESOLUTION - GRID_WIDTH, GRID_WIDTH, mHeight);
  return convertToTexture(mImage, WALL_MIXED_CAP);
}

GLuint C64SpindizzyTextureSet::generateIceWall() {
  Image* mImage = makePlainImage(cGridColour);
  int mIceWidth = (int) (RESOLUTION * 0.1);
  mImage->drawTriangle(cBackgroundColour, 0, RESOLUTION, RESOLUTION, RESOLUTION, mIceWidth, RESOLUTION - mIceWidth);
  mImage->drawTriangle(cBackgroundColour, mIceWidth, RESOLUTION - mIceWidth, RESOLUTION, RESOLUTION, RESOLUTION - mIceWidth, RESOLUTION - mIceWidth);
  return convertToTexture(mImage, WALL_ICE);
}

GLuint C64SpindizzyTextureSet::convertToTexture(Image* image, const std::string& type) {
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

C64SpindizzyTextureSet::C64SpindizzyTextureSet() {
  cPlugSockets.push_back(new PlugSocket("Palette"));
  assignDummyPlugin(&cPalette, "Palette");

  cTextures[SWITCH_CIRCLE_BOTH]  = new C64SpindizzyTexture();
  cTextures[SWITCH_CIRCLE_ONE]   = new C64SpindizzyTexture();
  cTextures[SWITCH_CIRCLE_NONE]  = new C64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_BOTH]  = new C64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_ONE]   = new C64SpindizzyTexture();
  cTextures[SWITCH_SQUARE_NONE]  = new C64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_NONE] = new C64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_ONE]  = new C64SpindizzyTexture();
  cTextures[SWITCH_DIAMOND_BOTH] = new C64SpindizzyTexture();
  cTextures[ARROW]               = new C64SpindizzyTexture();
  cTextures[TRAMPOLINE]          = new C64SpindizzyTexture();
  cTextures[ICE_WATER]           = new C64SpindizzyTexture();
  cTextures[PLAIN]               = new C64SpindizzyTexture();
  cTextures[PLAIN_SPLIT]         = new C64SpindizzyTexture();
  cTextures[WALL_MIXED_CAP]      = new C64SpindizzyTexture();
  cTextures[WALL_MIXED_MIDDLE]   = new C64SpindizzyTexture();
  cTextures[WALL_PLAIN_CAP]      = new C64SpindizzyTexture();
  cTextures[WALL_PLAIN_MIDDLE]   = new C64SpindizzyTexture();
  cTextures[WALL_ICE]            = new C64SpindizzyTexture();
  
  cBackgroundTexture = new PlainColourTexture();

  generateTextures();
}

void C64SpindizzyTextureSet::generateTextures() {
  cFloorColour      = cPalette->getColour(cFloorColourName);
  cWallColour       = cPalette->getColour(cWallColourName);
  cGridColour       = cPalette->getColour(cGridColourName);
  cBackgroundColour = cPalette->getColour(cBackgroundColourName);
  // TODO: Clean-up old ones
  cTextures[SWITCH_CIRCLE_BOTH]->setTexture(generateSwitchCircleBoth());
  cTextures[SWITCH_CIRCLE_ONE]->setTexture(generateSwitchCircleHalf());
  cTextures[SWITCH_CIRCLE_NONE]->setTexture(generateSwitchCircle());
  cTextures[SWITCH_SQUARE_BOTH]->setTexture(generateSwitchSquareBoth());
  cTextures[SWITCH_SQUARE_ONE]->setTexture(generateSwitchSquareHalf());
  cTextures[SWITCH_SQUARE_NONE]->setTexture(generateSwitchSquare());
  cTextures[SWITCH_DIAMOND_BOTH]->setTexture(generateSwitchDiamondBoth());
  cTextures[SWITCH_DIAMOND_ONE]->setTexture(generateSwitchDiamondHalf());
  cTextures[SWITCH_DIAMOND_NONE]->setTexture(generateSwitchDiamond());
  cTextures[ARROW]->setTexture(generateArrow());
  cTextures[TRAMPOLINE]->setTexture(generateTrampoline());
  cTextures[ICE_WATER]->setTexture(generateIce());
  cTextures[PLAIN]->setTexture(generatePlain());
  cTextures[PLAIN_SPLIT]->setTexture(generateSplitPlain());
  cTextures[WALL_MIXED_CAP]->setTexture(generateWallMixedCap());
  cTextures[WALL_MIXED_MIDDLE]->setTexture(generateWallMixedMiddle());
  cTextures[WALL_PLAIN_CAP]->setTexture(generateWallPlainCap());
  cTextures[WALL_PLAIN_MIDDLE]->setTexture(generateWallPlainMiddle());
  cTextures[WALL_ICE]->setTexture(generateIceWall());

  cBackgroundTexture->setTexture(cBackgroundColour);
}

void C64SpindizzyTextureSet::destroyTextures() {
  for (std::map<std::string, C64SpindizzyTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    delete i->second;
  }
  delete cBackgroundTexture;
  for (std::map<std::string, GLuint>::iterator i = cTextureIDs.begin(); i != cTextureIDs.end(); i++) {
    glDeleteTextures(1, &(i->second));
  }
  cTextureIDs.clear();
}

std::vector<PlugSocket*> C64SpindizzyTextureSet::getPlugSockets() {
  return cPlugSockets;
}

void C64SpindizzyTextureSet::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Palette") {
    IPalette* mPreviousPalette = cPalette;
    if (assignPlugin(plugin, &cPalette, *socket)) {
      mPreviousPalette->removeChangeListener(this);
      cPalette->addChangeListener(this);
      generateTextures();
    }
  } else {
    // TODO: Throw something
  }
}

void C64SpindizzyTextureSet::paletteChanged(IPalette* palette, const std::string& name) {
  if (name == cBackgroundColourName || name == cFloorColourName || name == cGridColourName || name == cWallColourName) {
    generateTextures();
  }
}

IPlugin* C64SpindizzyTextureSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Palette") {return cPalette;}
  // TODO: Throw something
  return NULL;
}

ITexture* C64SpindizzyTextureSet::getTexture(const std::string& name) {
  if (name == "Background") {
    return cBackgroundTexture;
  }
  std::map<std::string, C64SpindizzyTexture*>::iterator i = cTextures.find(name);
  return i != cTextures.end() ? i->second : cTextures.begin()->second;
}

C64SpindizzyTextureSet::~C64SpindizzyTextureSet() {
  for (unsigned int i = 0; i < cPlugSockets.size(); i++) {
    delete cPlugSockets[i];
  }
  destroyTextures();
}

void C64SpindizzyTextureSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Floor") {
      cFloorColourName = mNode->getAttribute("name");
    } else if (mValueAsString == "Wall") {
      cWallColourName = mNode->getAttribute("name");
    } else if (mValueAsString == "Grid") {
      cGridColourName = mNode->getAttribute("name");
    } else if (mValueAsString == "Extra") {
      cBackgroundColourName = mNode->getAttribute("name");
    } else {
      // TODO: Throw something!
    }
  }
  generateTextures();
}

void C64SpindizzyTextureSet::saveColour(DOMNodeWriter* node, const std::string& type, const std::string& name) {
  DOMNodeWriter* mColourNode = node->addBranch(type);
  mColourNode->addAttribute("type", "Palette");
  mColourNode->addAttribute("name", name);
}

void C64SpindizzyTextureSet::save(DOMNodeWriter* node) {
  saveColour(node, "Floor", cFloorColourName);
  saveColour(node, "Wall", cWallColourName);
  saveColour(node, "Grid", cGridColourName);
  saveColour(node, "Extra", cBackgroundColourName);
}

extern "C" IPlugin* create() {
  return new C64SpindizzyTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

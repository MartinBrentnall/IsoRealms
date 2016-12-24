/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) anyStart later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warrantyStart of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copyStart of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Utils.h"

std::string Utils::toString(int number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::string Utils::toString(unsigned int number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::string Utils::toString(float number) {
  std::stringstream mStringStream;
  mStringStream << number;
  std::string mString = mStringStream.str();
  return mString;
}

std::vector<std::string> Utils::split(const std::string& string, char splitChar) {
  std::string mString = string;
  std::vector<std::string> mSplitStrings;
  std::string::size_type mSplitPos = mString.find_first_of(splitChar);
  while (mSplitPos != std::string::npos) {
    std::string mSplitString = mString.substr(0, mSplitPos);
    mSplitStrings.push_back(mSplitString);
    mString = mString.substr(mSplitPos + 1);
    mSplitPos = mString.find_first_of(splitChar);
  }
  mSplitStrings.push_back(mString);
  return mSplitStrings;
}

// TODO: Move this into a generic "split into words" function class... or something
std::vector<std::string> Utils::splitWords(const std::string& words, char splitChar) {
  std::string mWords = words;
  std::vector<std::string> mSplitWords;
  std::string::size_type mWordStart = mWords.find_first_not_of(splitChar);
  while (mWordStart != std::string::npos) {
    mWords = mWords.substr(mWordStart);
    std::string::size_type mWordEnd = mWords.find_first_of(splitChar);
    if (mWordEnd != std::string::npos) {
      std::string mAlignWord = mWords.substr(0, mWordEnd);
      mWords = mWords.substr(mWordEnd);
      mSplitWords.push_back(mAlignWord);
      mWordStart = mWords.find_first_not_of(splitChar);
    } else {
      mSplitWords.push_back(mWords);
      mWordStart = std::string::npos;
    }
  }
  return mSplitWords;
}


bool Utils::endsWith(const std::string& fullString, const std::string& ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  }
  return false;
}

void Utils::renderVolumeCuboid(float mWest, float mEast, float mSouth, float mNorth, float mBottom, float mTop) {
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glVertex3f(mWest, mSouth, mTop);
  glVertex3f(mEast, mSouth, mTop);
  glVertex3f(mEast, mNorth, mTop);
  glVertex3f(mWest, mNorth, mTop);

  glVertex3f(mWest, mSouth, mBottom);
  glVertex3f(mWest, mNorth, mBottom);
  glVertex3f(mEast, mNorth, mBottom);
  glVertex3f(mEast, mSouth, mBottom);

  glVertex3f(mWest, mSouth, mBottom);
  glVertex3f(mEast, mSouth, mBottom);
  glVertex3f(mEast, mSouth, mTop);
  glVertex3f(mWest, mSouth, mTop);

  glVertex3f(mEast, mSouth, mBottom);
  glVertex3f(mEast, mNorth, mBottom);
  glVertex3f(mEast, mNorth, mTop);
  glVertex3f(mEast, mSouth, mTop);

  glVertex3f(mWest, mNorth, mBottom);
  glVertex3f(mWest, mNorth, mTop);
  glVertex3f(mEast, mNorth, mTop);
  glVertex3f(mEast, mNorth, mBottom);

  glVertex3f(mWest, mSouth, mBottom);
  glVertex3f(mWest, mSouth, mTop);
  glVertex3f(mWest, mNorth, mTop);
  glVertex3f(mWest, mNorth, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

void Utils::renderVolumeLines(float xStart, float xEnd, float yStart, float yEnd, float zStart, float zEnd) {
  glVertex3f(xEnd,   yEnd,   zStart); glVertex3f(xStart, yEnd,   zStart);
  glVertex3f(xStart, yEnd,   zStart); glVertex3f(xStart, yStart, zStart);
  glVertex3f(xStart, yStart, zStart); glVertex3f(xEnd,   yStart, zStart);
  glVertex3f(xEnd,   yStart, zStart); glVertex3f(xEnd,   yEnd,   zStart);
  glVertex3f(xEnd,   yEnd,   zEnd);   glVertex3f(xStart, yEnd,   zEnd);
  glVertex3f(xStart, yEnd,   zEnd);   glVertex3f(xStart, yStart, zEnd);
  glVertex3f(xStart, yStart, zEnd);   glVertex3f(xEnd,   yStart, zEnd);
  glVertex3f(xEnd,   yStart, zEnd);   glVertex3f(xEnd,   yEnd,   zEnd);
  glVertex3f(xStart, yEnd,   zStart); glVertex3f(xStart, yEnd,   zEnd);
  glVertex3f(xStart, yStart, zStart); glVertex3f(xStart, yStart, zEnd);
  glVertex3f(xEnd,   yStart, zStart); glVertex3f(xEnd,   yStart, zEnd);
  glVertex3f(xEnd,   yEnd,   zStart); glVertex3f(xEnd,   yEnd,   zEnd);
}

void Utils::renderVolumeMarkers(float xStart, float xEnd, float yStart, float yEnd, float zStart, float zEnd, float length) {
  glVertex3f(xStart, yStart, zStart); glVertex3f(xStart + length, yStart,          zStart);
  glVertex3f(xStart, yStart, zStart); glVertex3f(xStart,          yStart + length, zStart);
  glVertex3f(xStart, yStart, zStart); glVertex3f(xStart,          yStart,          zStart + length);

  glVertex3f(xStart, yStart, zEnd);   glVertex3f(xStart + length, yStart,          zEnd);
  glVertex3f(xStart, yStart, zEnd);   glVertex3f(xStart,          yStart + length, zEnd);
  glVertex3f(xStart, yStart, zEnd);   glVertex3f(xStart,          yStart,          zEnd - length);
  
  glVertex3f(xStart, yEnd,   zStart); glVertex3f(xStart + length, yEnd,            zStart);
  glVertex3f(xStart, yEnd,   zStart); glVertex3f(xStart,          yEnd - length,   zStart);
  glVertex3f(xStart, yEnd,   zStart); glVertex3f(xStart,          yEnd,            zStart + length);

  glVertex3f(xStart, yEnd,   zEnd);   glVertex3f(xStart + length, yEnd,            zEnd);
  glVertex3f(xStart, yEnd,   zEnd);   glVertex3f(xStart,          yEnd - length,   zEnd);
  glVertex3f(xStart, yEnd,   zEnd);   glVertex3f(xStart,          yEnd,            zEnd - length);

  glVertex3f(xEnd,   yStart, zStart); glVertex3f(xEnd - length,   yStart,          zStart);
  glVertex3f(xEnd,   yStart, zStart); glVertex3f(xEnd,            yStart + length, zStart);
  glVertex3f(xEnd,   yStart, zStart); glVertex3f(xEnd,            yStart,          zStart + length);

  glVertex3f(xEnd,   yStart, zEnd);   glVertex3f(xEnd - length,   yStart,          zEnd);
  glVertex3f(xEnd,   yStart, zEnd);   glVertex3f(xEnd,            yStart + length, zEnd);
  glVertex3f(xEnd,   yStart, zEnd);   glVertex3f(xEnd,            yStart,          zEnd - length);

  glVertex3f(xEnd,   yEnd,   zStart); glVertex3f(xEnd - length,   yEnd,            zStart);
  glVertex3f(xEnd,   yEnd,   zStart); glVertex3f(xEnd,            yEnd - length,   zStart);
  glVertex3f(xEnd,   yEnd,   zStart); glVertex3f(xEnd,            yEnd,            zStart + length);

  glVertex3f(xEnd,   yEnd,   zEnd);   glVertex3f(xEnd - length,   yEnd,            zEnd);
  glVertex3f(xEnd,   yEnd,   zEnd);   glVertex3f(xEnd,            yEnd - length,   zEnd);
  glVertex3f(xEnd,   yEnd,   zEnd);   glVertex3f(xEnd,            yEnd,            zEnd - length);
}

void Utils::renderStaticVisuals(std::vector<IVisualElement*> visuals) {
  std::map<ITexture*, std::vector<IVisualElement*>*> mSortedElements;
  for (IVisualElement* mVisual : visuals) {
    ITexture* mTexture = mVisual->getTexture();
    std::vector<IVisualElement*>* mSingleTextureVisuals = mSortedElements[mTexture];
    if (mSingleTextureVisuals == nullptr) {
      mSingleTextureVisuals = new std::vector<IVisualElement*>();
      mSortedElements[mTexture] = mSingleTextureVisuals;
    }
    mSingleTextureVisuals->push_back(mVisual);
  }
  
  for (std::pair<ITexture*, std::vector<IVisualElement*>*> mSingleTextureVisuals : mSortedElements) {
    mSingleTextureVisuals.first->set();
    for (IVisualElement* mVisual : *(mSingleTextureVisuals.second)) {
      mVisual->render();
    }
    delete mSingleTextureVisuals.second;
  }
}

bool Utils::replaceTexture(ITexture*& replacee, ITexture* destroyee, ITexture* replacement) {
  if (replacee == destroyee) {
    replacee = replacement;
    return true;
  }
  return false;
}

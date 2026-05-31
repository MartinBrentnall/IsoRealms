/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) anyStart later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warrantyStart of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copyStart of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Utils.h"

#include "IsoRealms/Assets/Literal/LiteralVertex.h"
#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/Application.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/GameControllerButton.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/GameControllerHat.h"
#include "IsoRealms/Assets/Fixed/DigitalInput/KeyboardKey.h"
#include "IsoRealms/Assets/Type/IDigitalInput.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/System.h"

namespace IsoRealms {
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

  std::string Utils::toString(long number) {
    std::stringstream mStringStream;
    mStringStream << number;
    std::string mString = mStringStream.str();
    return mString;
  }

  std::string Utils::toString(float number) {
    std::stringstream mStringStream;
    mStringStream << std::fixed << std::setw(1) << number;
    std::string mString = mStringStream.str();
    return mString;
  }

  std::string Utils::toString(double number) {
    std::stringstream mStringStream;
    mStringStream << std::fixed << std::setw(1) << number;
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

  void Utils::renderStaticVisuals(std::vector<std::unique_ptr<IVisualElement>>& visuals) {
    std::map<ITexture*, std::unique_ptr<std::vector<IVisualElement*>>> mSortedElements;
    for (std::unique_ptr<IVisualElement>& mVisual : visuals) {
      ITexture* mTexture = mVisual->getTexture();
      if (mTexture != nullptr) {
        std::map<ITexture*, std::unique_ptr<std::vector<IVisualElement*>>>::iterator mIterator = mSortedElements.find(mTexture);
        if (mIterator == mSortedElements.end()) {
//        std::vector<std::unique_ptr<IVisualElement>>* mSingleTextureVisuals = mSortedElements[mTexture];
          mSortedElements.emplace(mTexture, std::make_unique<std::vector<IVisualElement*>>());
          mIterator = mSortedElements.find(mTexture);
        }
        mIterator->second->emplace_back(mVisual.get());
      } else {
        mVisual->prepareVisual();
      }
    }

    for (const std::pair<ITexture* const, std::unique_ptr<std::vector<IVisualElement*>>>& mSingleTextureVisuals : mSortedElements) {
      mSingleTextureVisuals.first->set();
      for (IVisualElement* mVisual : *(mSingleTextureVisuals.second)) {
        mVisual->render();
      }
    }
  }

  bool Utils::replaceTexture(ITexture*& replacee, ITexture* destroyee, ITexture* replacement) {
    if (replacee == destroyee) {
      replacee = replacement;
      return true;
    }
    return false;
  }

  float Utils::luminance(const IColour& colour) {
    return colour.getRed() * LUMINANCE_RED + colour.getGreen() * LUMINANCE_GREEN + colour.getBlue() * LUMINANCE_BLUE;
  }

  float Utils::restrict(float low, float high, float val) {
    if (val < low) {
      return low;
    }
    if (val > high) {
      return high;
    }
    return val;
  }

  int Utils::restrict(int low, int high, int val) {
    if (val < low) {
      return low;
    }
    if (val > high) {
      return high;
    }
    return val;
  }

  float Utils::sine(float min, float max, float gradient) {
    float mMiddle = (max - min) * gradient + min;
    float mLowerMiddle = (mMiddle - min) * gradient + min;
    float mUpperMiddle = (max - mMiddle) * gradient + mMiddle;
    return (mUpperMiddle - mLowerMiddle) * gradient + mLowerMiddle;
  }
  
  void Utils::renderIconBranch() {
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.5f);
    glVertex2f(-1.0f,  0.5f);
    glVertex2f( 0.0f,  0.0f);
    glVertex2f( 1.0f,  0.5f);
    glVertex2f( 0.0f,  1.0f);
    glColor3f(0.7f, 0.7f, 0.35f);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f( 0.0f, -1.0f);
    glVertex2f( 0.0f,  0.0f);
    glVertex2f(-1.0f,  0.5f);
    glColor3f(0.4f, 0.4f, 0.2f);
    glVertex2f( 0.0f, -1.0f);
    glVertex2f( 1.0f, -0.5f);
    glVertex2f( 1.0f,  0.5f);
    glVertex2f( 0.0f,  0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
  }
  
  void Utils::renderIconLeaf() {
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 1.0f, 1.0f);
    glVertex2f( 0.8f,  0.1f);
    glVertex2f( 0.0f,  0.9f);
    glVertex2f(-0.8f,  0.9f);
    glColor3f(0.4f, 0.6f, 0.6f);
    glVertex2f(-0.8f, -1.0f);
    glVertex2f( 0.8f, -1.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.6f, 0.8f, 0.8f);
    glVertex2f( 0.8f,  0.1f);
    glVertex2f( 0.0f,  0.9f);
    glColor3f(0.4f, 0.6f, 0.6f);
    glVertex2f( 0.0f,  0.1f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  
  void Utils::renderIconCustom() {
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.4f);
    glVertex2f( 1.0f, -0.4f);
    glVertex2f(-0.1f,  0.7f);
    glColor3f(0.5f, 0.5f, 0.2f);
    glVertex2f(-0.7f,  0.1f);
    glVertex2f( 0.4f, -1.0f);
    
    glColor3f(1.0f, 1.0f, 0.7f);
    glVertex2f(-1.0f,  1.0f);
    glColor3f(0.5f, 0.5f, 0.35f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-0.7f,  0.1f);
    glColor3f(1.0f, 1.0f, 0.7f);
    glVertex2f(-0.1f,  0.7f);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
    glVertex2f(-0.85f,  0.55f);
    glVertex2f(-0.55f,  0.85f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  
  void Utils::renderIconAdd() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    renderRectangle(-1.0f, -0.4f, 1.0f, 0.4f);
    renderRectangle(-0.4f, -1.0f, 0.4f, 1.0f);
    
    glColor3f(0.4f, 0.4f, 1.0f);
    renderRectangle(-0.8f, -0.2f, 0.8f, 0.2f);
    renderRectangle(-0.2f, -0.8f, 0.2f, 0.8f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void Utils::renderIconNone() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f( 1.0f, -0.5f);
    glVertex2f(-0.5f,  1.0f);
    glVertex2f(-1.0f,  0.5f);
    glVertex2f( 0.5f, -1.0f);

    glVertex2f( 0.5f,  1.0f);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(-0.5f, -1.0f);
    glVertex2f( 1.0f,  0.5f);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f( 0.8f, -0.5f);
    glVertex2f(-0.5f,  0.8f);
    glVertex2f(-0.8f,  0.5f);
    glVertex2f( 0.5f, -0.8f);

    glVertex2f( 0.5f,  0.8f);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(-0.5f, -0.8f);
    glVertex2f( 0.8f,  0.5f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  
  void Utils::renderIconTick() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f( 0.5f,  0.9f);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-0.3f, -0.9f);
    glVertex2f( 1.0f,  0.4f);

    glVertex2f(-0.5f,  0.3f);
    glVertex2f(-1.0f, -0.2f);
    glVertex2f(-0.3f, -0.9f);
    glVertex2f( 0.2f, -0.4f);
    
    glColor3f(0.0f, 0.75f, 0.0f);
    glVertex2f( 0.5f,  0.7f);
    glVertex2f(-0.6f, -0.4f);
    glVertex2f(-0.3f, -0.7f);
    glVertex2f( 0.8f,  0.4f);

    glVertex2f(-0.5f,  0.1f);
    glVertex2f(-0.8f, -0.2f);
    glVertex2f(-0.3f, -0.7f);
    glVertex2f( 0.0f, -0.4f);
    
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  
  void Utils::renderIconTerminal() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.8f, 0.8f, 0.8f);
    renderRoundedRectangle(-1.0f, -0.9f, 1.0f, 0.9f, 0.4f);
    glColor3f(0.2f, 0.2f, 0.2f);
    renderRoundedRectangle(-0.9f, -0.8f, 0.9f, 0.8f, 0.4f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.0f,  0.0f);
    glVertex2f(-0.4f,  0.4f);
    glVertex2f(-0.5f,  0.3f);
    glVertex2f(-0.1f, -0.1f);

    glVertex2f( 0.0f,  0.0f);
    glVertex2f(-0.1f,  0.1f);
    glVertex2f(-0.5f, -0.3f);
    glVertex2f(-0.4f, -0.4f);
    glEnd();
    Utils::renderBar(0.2f, -0.5f, 0.5f, -0.4f);
    glColor3f(1.0f, 1.0f, 1.0f);
  }

  void Utils::renderIconLock() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.2f, 0.2f, 0.2f);
    renderRing(0.0f, 0.2f, 0.2f, 0.8f);
    renderRoundedRectangle(-1.0f, -1.0f, 1.0f, 0.2f, 0.4f);
    glColor3f(0.8f, 0.9f, 1.0f);
    renderRing(0.0f, 0.2f, 0.45f, 0.55f);
    renderRoundedRectangle(-0.75f, -0.75f, 0.75f, -0.05f, 0.2f);
    glColor3f(0.0f, 0.0f, 0.0f);
    renderRectangle(-0.2f, -0.6f, 0.2f, 0.0f);
  }

  void Utils::renderRingSection(float x, float y, float innerRadius, float outerRadius, float startAngle, float endAngle) {
    glBegin(GL_TRIANGLE_STRIP);
    unsigned int mStartAngle = static_cast<unsigned int>(32 * std::min(startAngle, endAngle));
    unsigned int mEndAngle   = static_cast<unsigned int>(32 * std::max(startAngle, endAngle));
    for (unsigned int i = mStartAngle; i <= mEndAngle; ++i) {
      float mCircle = static_cast<float>(-((static_cast<float>(i) * M_PI) * (2.0 / 32)) + M_PI * 2.0);
      float mPlotInnerX = (std::sin(mCircle) * innerRadius) + x;
      float mPlotInnerY = (std::cos(mCircle) * innerRadius) + y;
      float mPlotOuterX = (std::sin(mCircle) * outerRadius) + x;
      float mPlotOuterY = (std::cos(mCircle) * outerRadius) + y;
      glVertex2f(mPlotInnerX, mPlotInnerY);
      glVertex2f(mPlotOuterX, mPlotOuterY);
    }
    glEnd();
  }
  
  void Utils::renderRing(float x, float y, float innerRadius, float outerRadius) {
    renderRingSection(x, y, innerRadius, outerRadius, 0.0f, 1.0f);
  }
  
  void Utils::renderCircle(float x, float y, float radius) {
    renderCurve(x, y, radius, 0.0f, 1.0f);
  }
  
  void Utils::renderOval(float x, float y, float width, float height, float startAngle, float endAngle) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    unsigned int mStartAngle = static_cast<unsigned int>(32 * std::min(startAngle, endAngle));
    unsigned int mEndAngle   = static_cast<unsigned int>(32 * std::max(startAngle, endAngle));
    for (unsigned int i = mStartAngle; i <= mEndAngle; ++i) {
      float mCircle = static_cast<float>(-((static_cast<float>(i) * M_PI) * (2.0 / 32)) + M_PI * 2.0);
      float mPlotX  = (std::sin(mCircle) * width)  + x;
      float mPlotY  = (std::cos(mCircle) * height) + y;
      glVertex2f(mPlotX, mPlotY);
    }
    glEnd();
  }

  void Utils::renderCurve(float x, float y, float radius, float startAngle, float endAngle) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    unsigned int mStartAngle = static_cast<unsigned int>(32 * std::min(startAngle, endAngle));
    unsigned int mEndAngle   = static_cast<unsigned int>(32 * std::max(startAngle, endAngle));
    for (unsigned int i = mStartAngle; i <= mEndAngle; ++i) {
      float mCircle = static_cast<float>(-((static_cast<float>(i) * M_PI) * (2.0 / 32)) + M_PI * 2.0);
      float mPlotX  = (std::sin(mCircle) * radius) + x;
      float mPlotY  = (std::cos(mCircle) * radius) + y;
      glVertex2f(mPlotX, mPlotY);
    }
    glEnd();
  }

  void Utils::renderCurveLine(float x, float y, float radius, float startAngle, float endAngle) {
    glBegin(GL_LINE_STRIP);
    unsigned int mStartAngle = static_cast<unsigned int>(32 * std::min(startAngle, endAngle));
    unsigned int mEndAngle   = static_cast<unsigned int>(32 * std::max(startAngle, endAngle));
    for (unsigned int i = mStartAngle; i <= mEndAngle; ++i) {
      float mCircle = static_cast<float>(-((static_cast<float>(i) * M_PI) * (2.0 / 32)) + M_PI * 2.0);
      float mPlotX  = (std::sin(mCircle) * radius) + x;
      float mPlotY  = (std::cos(mCircle) * radius) + y;
      glVertex3f(mPlotX, mPlotY, 0.0f);
    }
    glEnd();
  }

  void Utils::renderRectangle(float left, float bottom, float right, float top) {
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left,  top);
    glVertex2f(left,  bottom);
  }
  
  void Utils::renderBar(float left, float bottom, float right, float top) {
    renderCurve(left,  bottom + (top - bottom) / 2.0f, (top - bottom) / 2.0f, 0.0f, 0.5f);
    renderCurve(right, bottom + (top - bottom) / 2.0f, (top - bottom) / 2.0f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    renderRectangle(left, bottom, right, top);
    glEnd();
  }

  void Utils::renderRoundedRectangleLines(float left, float bottom, float right, float top, float curveSize) {
    glBegin(GL_LINES);
    glVertex2f(left  + curveSize, top);
    glVertex2f(right - curveSize, top);
    glVertex2f(left  + curveSize, bottom);
    glVertex2f(right - curveSize, bottom);
    glVertex2f(left,              top    - curveSize);
    glVertex2f(left,              bottom + curveSize);
    glVertex2f(right,             top    - curveSize);
    glVertex2f(right,             bottom + curveSize);
    glEnd();
    renderCurveLine(right - curveSize, top    - curveSize, curveSize, 0.75f, 1.0f);
    renderCurveLine(right - curveSize, bottom + curveSize, curveSize, 0.5f,  0.75f);
    renderCurveLine(left  + curveSize, bottom + curveSize, curveSize, 0.25f, 0.5f);
    renderCurveLine(left  + curveSize, top    - curveSize, curveSize, 0.0f,  0.25f);
  }

  void Utils::renderRoundedRectangle(float left, float bottom, float right, float top, float curveSize) {
    glBegin(GL_QUADS);
    renderRectangle(left + curveSize, top    - curveSize, right - curveSize, top);
    renderRectangle(left,             bottom + curveSize, right,             top    - curveSize);
    renderRectangle(left + curveSize, bottom,             right - curveSize, bottom + curveSize);
    glEnd();
    renderCurve(right - curveSize, top    - curveSize, curveSize, 0.75f, 1.0f);
    renderCurve(right - curveSize, bottom + curveSize, curveSize, 0.5f,  0.75f);
    renderCurve(left  + curveSize, bottom + curveSize, curveSize, 0.25f, 0.5f);
    renderCurve(left  + curveSize, top    - curveSize, curveSize, 0.0f,  0.25f);
  }

  void Utils::renderBezier(const Point2D& start, const Point2D& controlA, const Point2D& controlB, const Point2D& end, int resolution) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < resolution; i++) {
      float mInterpolation = i / static_cast<float>(resolution);
      Point2D mAB    = start.midPoint(   controlA, mInterpolation);
      Point2D mBC    = controlA.midPoint(controlB, mInterpolation);
      Point2D mCD    = controlB.midPoint(end,      mInterpolation);
      Point2D mABBC  = mAB.midPoint(     mBC,      mInterpolation);
      Point2D mBCCD  = mBC.midPoint(     mCD,      mInterpolation);
      Point2D mFinal = mABBC.midPoint(   mBCCD,    mInterpolation);
      glVertex2f(mFinal.getX(), mFinal.getY());
    }
    glEnd();
  }

  void Utils::calculateColour(float hue, float saturation, float lightness, float& red, float& green, float& blue) {
    red = 0.0f;
    green = 0.0f;
    blue = 0.0f;
    if (hue <= 1.0f / 3.0f) {
      green = hue * 6.0f;
      red = -green + 2.0f;
    } else if (hue >= 2.0 / 3.0f) {
      hue -= 2.0f / 3.0f;
      red = hue * 6.0f;
      blue = -red + 2.0f;
    } else {
      hue -= 1.0f / 3.0f;
      blue = hue * 6.0f;
      green = -blue + 2.0f;
    }
    red   = std::min(1.0f, std::min(1.0f, red)   * lightness);
    green = std::min(1.0f, std::min(1.0f, green) * lightness);
    blue  = std::min(1.0f, std::min(1.0f, blue)  * lightness);
    red   += (lightness - red)   * saturation;
    green += (lightness - green) * saturation;
    blue  += (lightness - blue)  * saturation;
    if (std::isnan(red))   {red   = 0.0f;}
    if (std::isnan(green)) {green = 0.0f;}
    if (std::isnan(blue))  {blue  = 0.0f;}
  }

  float Utils::getMiddle(float a, float b, float c) {
    return a > b ? (b > c ? b : (a > c ? c : a))
                 : (a > c ? a : (b > c ? c : b));
  }

  float Utils::getLightness(float red, float green, float blue) {
    return std::max({red, green, blue});
  }

  float Utils::getSaturation(float red, float green, float blue) {
    float mSaturation = std::min({red, green, blue}) / std::max({red, green, blue});
    return std::isnan(mSaturation) ? 0.0f : mSaturation;
  }

  float Utils::getHue(float red, float green, float blue) {
    float mLight = std::max({red, green, blue});
    float mMiddle = getMiddle(red, green, blue);
    float mDark = std::min({red, green, blue});
    float mHueShift = (mMiddle - mDark) / (mLight - mDark);
    mHueShift /= 6.0f;
    float mHue;
    if (red >= green && red >= blue) {
      mHue = 0.0f;
      if (blue > green) {
        mHueShift = -mHueShift;
      };
    } else if (green >= red && green >= blue) {
      mHue = 1.0f / 3.0f;
      if (red > blue) {
        mHueShift = -mHueShift;
      };
    } else {
      mHue = 2.0f / 3.0f;
      if (green > red) {
        mHueShift = -mHueShift;
      };
    }
    mHue += mHueShift;
    if (mHue < 0.0f) {
      mHue += 1.0f;
    }
    return std::isnan(mHue) ? 0.0f : mHue;
  }

  double Utils::distance(double x1, double y1, double x2, double y2) {
    double px = x1 - x2;
    double py = y1 - y2;
    return sqrt(px * px + py * py);
  }

  std::unique_ptr<IDigitalInput> Utils::toDigitalInputMapping(HatHandler& hatHandler, sf::Event& event) {
//     switch (event.type) {
//       case sf::Event::KeyPressed:            return std::make_unique<KeyboardKey>(event.key.code);
//       case sf::Event::JoystickButtonPressed: return std::make_unique<GameControllerButton>(event.joystickButton.button);
//       case sf::Event::JoystickMoved: {
//         if (event.joystickMove.axis == sf::Joystick::Axis::PovX) {
//           if (event.joystickMove.position < 0) {
//             return std::make_unique<GameControllerHat>(hatHandler, HatHandler::Direction::HAT_LEFT);
//           } else if (event.joystickMove.position > 0) {
//             return std::make_unique<GameControllerHat>(hatHandler, HatHandler::Direction::HAT_RIGHT);
//           }
//         } else if (event.joystickMove.axis == sf::Joystick::Axis::PovY) {
// #if _WIN32
//           if (event.joystickMove.position > 0) {
//             return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_UP);
//           } else if (event.joystickMove.position < 0) {
//             return std::make_unique<HatMapping>(hatHandler, HatHandler::Direction::HAT_DOWN);
//           }
// #elif __linux__
//           if (event.joystickMove.position < 0) {
//             return std::make_unique<GameControllerHat>(hatHandler, HatHandler::Direction::HAT_UP);
//           } else if (event.joystickMove.position > 0) {
//             return std::make_unique<GameControllerHat>(hatHandler, HatHandler::Direction::HAT_DOWN);
//           }
// #endif
//         }
//         break;
//       }
//       default: break;
//     }
    return nullptr;
  }

  void Utils::shadowPrint(float x, float y, IFont& font, float fontSize, const IColour& colour, float shadowOffset, IFont::Alignment alignment, const std::string& text) {
    glColor3f(0.0f, 0.0f, 0.0f);
    font.print(x + shadowOffset, y - shadowOffset, fontSize, alignment, text);
    colour.set();
    font.print(x, y, fontSize, alignment, text);
  }

  void Utils::renderTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c) {
    glVertex3f(static_cast<float>(a.x), static_cast<float>(a.y), static_cast<float>(a.z));
    glVertex3f(static_cast<float>(b.x), static_cast<float>(b.y), static_cast<float>(b.z));
    glVertex3f(static_cast<float>(c.x), static_cast<float>(c.y), static_cast<float>(c.z));
  }

  void Utils::renderInnerTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c, float lineWidth) {
    LiteralVertex mA;
    LiteralVertex mB;
    LiteralVertex mC;
    calculateTriangle(a, b, c, mA, mB, mC, lineWidth);
    glVertex3f(static_cast<float>(mA.x), static_cast<float>(mA.y), static_cast<float>(mA.z));
    glVertex3f(static_cast<float>(mB.x), static_cast<float>(mB.y), static_cast<float>(mB.z));
    glVertex3f(static_cast<float>(mC.x), static_cast<float>(mC.y), static_cast<float>(mC.z));
  }

  void Utils::renderOuterTriangle(LiteralVertex& a, LiteralVertex& b, LiteralVertex& c, float lineWidth) {
    LiteralVertex mA;
    LiteralVertex mB;
    LiteralVertex mC;
    calculateTriangle(a, b, c, mA, mB, mC, lineWidth);
    glVertex3f(static_cast<float>(mB.x), static_cast<float>(mB.y), static_cast<float>(mB.z));
    glVertex3f(static_cast<float>(mA.x), static_cast<float>(mA.y), static_cast<float>(mA.z));
    glVertex3f(static_cast<float>(a.x),  static_cast<float>(a.y),  static_cast<float>(a.z));
    glVertex3f(static_cast<float>(mB.x), static_cast<float>(mB.y), static_cast<float>(mB.z));
    glVertex3f(static_cast<float>(a.x),  static_cast<float>(a.y),  static_cast<float>(a.z));
    glVertex3f(static_cast<float>(b.x),  static_cast<float>(b.y),  static_cast<float>(b.z));

    glVertex3f(static_cast<float>(mC.x), static_cast<float>(mC.y), static_cast<float>(mC.z));
    glVertex3f(static_cast<float>(mB.x), static_cast<float>(mB.y), static_cast<float>(mB.z));
    glVertex3f(static_cast<float>(b.x),  static_cast<float>(b.y),  static_cast<float>(b.z));
    glVertex3f(static_cast<float>(mC.x), static_cast<float>(mC.y), static_cast<float>(mC.z));
    glVertex3f(static_cast<float>(b.x),  static_cast<float>(b.y),  static_cast<float>(b.z));
    glVertex3f(static_cast<float>(c.x),  static_cast<float>(c.y),  static_cast<float>(c.z));

    glVertex3f(static_cast<float>(mA.x), static_cast<float>(mA.y), static_cast<float>(mA.z));
    glVertex3f(static_cast<float>(mC.x), static_cast<float>(mC.y), static_cast<float>(mC.z));
    glVertex3f(static_cast<float>(c.x),  static_cast<float>(c.y),  static_cast<float>(c.z));
    glVertex3f(static_cast<float>(mA.x), static_cast<float>(mA.y), static_cast<float>(mA.z));
    glVertex3f(static_cast<float>(c.x),  static_cast<float>(c.y),  static_cast<float>(c.z));
    glVertex3f(static_cast<float>(a.x),  static_cast<float>(a.y),  static_cast<float>(a.z));
  }

  // Taking Z as depth, use input vertex A as the fixed based point, input
  // vertices B and C are transformed to create a 2D flat-base upright triangle
  // from which the output triangle vertices are then calculated.  This
  // transformation is then reversed on the vertices of the output triangle
  // to give the final result.
  void Utils::calculateTriangle(LiteralVertex mPointA, LiteralVertex mPointB, LiteralVertex mPointC, LiteralVertex& mResultA, LiteralVertex& mResultB, LiteralVertex& mResultC, float mWidth) {

    // Yaw the triangle such that B is at the same depth as A (Turn points B and
    // C to make B.z equal to A.z).
    double mTurnB    = atan2(-mPointB.z + mPointA.z, mPointB.x - mPointA.x);
    double mTurnC    = atan2(-mPointC.z + mPointA.z, mPointC.x - mPointA.x);
    double mDistance = Utils::distance(mPointA.x, mPointA.z, mPointB.x, mPointB.z);
    mPointB.x        = mDistance + mPointA.x;
    mPointB.z        = mPointA.z;

    mDistance = Utils::distance(mPointA.x, mPointA.z, mPointC.x, mPointC.z);
    mPointC.x = std::cos(mTurnB - mTurnC) * mDistance + mPointA.x;
    mPointC.z = std::sin(mTurnB - mTurnC) * mDistance + mPointA.z;

    // Roll the triangle such that B is in line with A (Rotate points B and C to
    // make B.y equal to A.y).
    double mRotateB = atan2(-mPointB.y + mPointA.y, mPointB.x - mPointA.x);
    double mRotateC = atan2(-mPointC.y + mPointA.y, mPointC.x - mPointA.x);
    mDistance       = Utils::distance(mPointA.x,  mPointA.y, mPointB.x, mPointB.y);
    mPointB.x       = mDistance + mPointA.x;
    mPointB.y       = mPointA.y;

    mDistance = Utils::distance(mPointA.x, mPointA.y, mPointC.x, mPointC.y);
    mPointC.x = std::cos(mRotateB - mRotateC) * mDistance + mPointA.x;
    mPointC.y = std::sin(mRotateB - mRotateC) * mDistance + mPointA.y;

    // Pitch the triangle such that C is the same depth as A and B (Pitch point C
    // to make C.z equal to A.z and B.z).
    double mTilt = atan2(-mPointC.z + mPointA.z, mPointC.y - mPointA.y);
    mDistance    = Utils::distance(mPointA.y, mPointA.z, mPointC.y, mPointC.z);
    mPointC.y    = mDistance + mPointA.y;
    mPointC.z    = mPointA.z;

    // Calculate vertices of the output triangle.
    double mAngleA = atan2(-mPointB.y + mPointA.y, mPointB.x - mPointA.x);
    double mAngleB = atan2(-mPointC.y + mPointB.y, mPointC.x - mPointB.x);
    double mAngleC = atan2(-mPointA.y + mPointC.y, mPointA.x - mPointC.x);
    LiteralVertex mOffsetPointA(std::cos(mAngleA - M_PI * 0.5) * mWidth + mPointA.x, (std::sin(mAngleA - M_PI * 0.5) * mWidth - mPointA.y) * -1.0, 0.0);
    LiteralVertex mOffsetPointB(std::cos(mAngleB - M_PI * 0.5) * mWidth + mPointB.x, (std::sin(mAngleB - M_PI * 0.5) * mWidth - mPointB.y) * -1.0, 0.0);
    LiteralVertex mOffsetPointC(std::cos(mAngleC - M_PI * 0.5) * mWidth + mPointC.x, (std::sin(mAngleC - M_PI * 0.5) * mWidth - mPointC.y) * -1.0, 0.0);
    LiteralVertex mArrowPointA(std::cos(mAngleA) * mWidth + mOffsetPointA.x, (std::sin(mAngleA) * mWidth - mOffsetPointA.y) * -1.0, 0.0);
    LiteralVertex mArrowPointB(std::cos(mAngleB) * mWidth + mOffsetPointB.x, (std::sin(mAngleB) * mWidth - mOffsetPointB.y) * -1.0, 0.0);
    LiteralVertex mArrowPointC(std::cos(mAngleC) * mWidth + mOffsetPointC.x, (std::sin(mAngleC) * mWidth - mOffsetPointC.y) * -1.0, 0.0);
    double mGradA = getValue(mOffsetPointA, mArrowPointA, mOffsetPointB, mArrowPointB);
    double mGradB = getValue(mOffsetPointB, mArrowPointB, mOffsetPointC, mArrowPointC);
    double mGradC = getValue(mOffsetPointC, mArrowPointC, mOffsetPointA, mArrowPointA);
    mResultB.set(mOffsetPointA.x + mGradA * (mArrowPointA.x - mOffsetPointA.x), mOffsetPointA.y + mGradA * (mArrowPointA.y - mOffsetPointA.y), mPointA.z);
    mResultC.set(mOffsetPointB.x + mGradB * (mArrowPointB.x - mOffsetPointB.x), mOffsetPointB.y + mGradB * (mArrowPointB.y - mOffsetPointB.y), mPointA.z);
    mResultA.set(mOffsetPointC.x + mGradC * (mArrowPointC.x - mOffsetPointC.x), mOffsetPointC.y + mGradC * (mArrowPointC.y - mOffsetPointC.y), mPointA.z);

    // Reverse pitch of the output triangle.
    mDistance  = Utils::distance(mPointA.y, mPointA.z, mResultC.y, mResultC.z);
    mResultC.z = std::cos(  mTilt + M_PI / 2)  * mDistance + mPointA.z;
    mResultC.y = std::sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;
    mDistance  = Utils::distance(mPointA.y, mPointA.z, mResultB.y, mResultB.z);
    mResultB.z = std::cos(  mTilt + M_PI / 2)  * mDistance + mPointA.z;
    mResultB.y = std::sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;
    mDistance  = Utils::distance(mPointA.y, mPointA.z, mResultA.y, mResultA.z);
    mResultA.z = std::cos(  mTilt + M_PI / 2)  * mDistance + mPointA.z;
    mResultA.y = std::sin(-(mTilt - M_PI / 2)) * mDistance + mPointA.y;

    // Reverse roll of the output triangle.
    mDistance     = Utils::distance(mPointA.x, mPointA.y, mResultA.x, mResultA.y);
    double mAngle = atan2(-mResultA.y + mPointA.y, mResultA.x - mPointA.x);
    mResultA.y    = std::cos(mAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
    mResultA.x    = std::sin(-((mAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;
    mDistance     = Utils::distance(mPointA.x, mPointA.y, mResultB.x, mResultB.y);
    mAngle        = atan2(-mResultB.y + mPointA.y, mResultB.x - mPointA.x);
    mResultB.y    = std::cos(mAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
    mResultB.x    = std::sin(-((mAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;
    mDistance     = Utils::distance(mPointA.x, mPointA.y, mResultC.x, mResultC.y);
    mAngle        = atan2(-mResultC.y + mPointA.y, mResultC.x - mPointA.x);
    mResultC.y    = std::cos(mAngle + mRotateB + M_PI / 2) * mDistance + mPointA.y;
    mResultC.x    = std::sin(-((mAngle + mRotateB) - M_PI / 2)) * mDistance + mPointA.x;

    // Reverse yaw of the output triangle.
    mDistance  = Utils::distance(mPointA.x, mPointA.z, mResultA.x, mResultA.z);
    mAngle     = atan2(-mResultA.z + mPointA.z, mResultA.x - mPointA.x);
    mResultA.z = std::cos(mAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
    mResultA.x = std::sin(-((mAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
    mDistance  = Utils::distance(mPointA.x, mPointA.z, mResultB.x, mResultB.z);
    mAngle     = atan2(-mResultB.z + mPointA.z, mResultB.x - mPointA.x);
    mResultB.z = std::cos(mAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
    mResultB.x = std::sin(-((mAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
    mDistance  = Utils::distance(mPointA.x, mPointA.z, mResultC.x, mResultC.z);
    mAngle     = atan2(-mResultC.z + mPointA.z, mResultC.x - mPointA.x);
    mResultC.z = std::cos(mAngle + mTurnB + M_PI / 2) * mDistance + mPointA.z;
    mResultC.x = std::sin(-((mAngle + mTurnB) - M_PI / 2)) * mDistance + mPointA.x;
  }

  double Utils::getValue(LiteralVertex a1, LiteralVertex a2, LiteralVertex b1, LiteralVertex b2) {
    return ((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x -b1.x)) / ((b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y));
  }
}

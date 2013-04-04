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
#include "ComponentColourSelector.h"

float ComponentColourSelector::MIN_CHANNEL_VALUE = 0.0f;
float ComponentColourSelector::MAX_CHANNEL_VALUE = 1.0f;

ComponentColourSelector::ComponentColourSelector(Colour* colour, IResourceAccessor* resources) : RectangularComponent("IsoRealms/GUI/Dialogs/ComponentColourSelector", resources) {
  cColour = colour;
  cResources = resources;
  setupComponent();
}

void ComponentColourSelector::setColour(Colour* colour) {
  cColour = colour;
  refresh();
}

void ComponentColourSelector::addColourSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cListeners.push_back(listener);
}

void ComponentColourSelector::removeColourSelectionListener(IResourceSelectionListener<IColour>* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
      return;
    }
  }
}

void ComponentColourSelector::refresh() {
  cRed = cColour->getRed();
  cGreen = cColour->getGreen();
  cBlue = cColour->getBlue();
  cAlpha = cColour->getAlpha();
  setFloatValue("redValue", cRed);
  setFloatValue("greenValue", cGreen);
  setFloatValue("blueValue", cBlue);
  setFloatValue("alphaValue", cAlpha);
  updateHSLSliders();  
}

void ComponentColourSelector::setupComponent() {
  cUpdating = false;
  setRenderer(new ChannelRenderer(this, &MIN_CHANNEL_VALUE, &cGreen,            &cBlue,             &MAX_CHANNEL_VALUE, &cGreen,            &cBlue),             "redValue");
  setRenderer(new ChannelRenderer(this, &cRed,              &MIN_CHANNEL_VALUE, &cBlue,             &cRed,              &MAX_CHANNEL_VALUE, &cBlue),             "greenValue");
  setRenderer(new ChannelRenderer(this, &cRed,              &cGreen,            &MIN_CHANNEL_VALUE, &cRed,              &cGreen,            &MAX_CHANNEL_VALUE), "blueValue");
  setRenderer(new HueRenderer(this), "hueValue");
  setRenderer(new SaturationRenderer(this), "saturationValue");
  setRenderer(new LightnessRenderer(this), "lightnessValue");
  setRenderer(new AlphaRenderer(this), "alphaValue");
  setRenderer(new PreviewRenderer(this), "previewPanel");
  addFloatListener(new SliderListener(this, &cRed), "redValue");
  addFloatListener(new SliderListener(this, &cGreen), "greenValue");
  addFloatListener(new SliderListener(this, &cBlue), "blueValue");
  addFloatListener(new SliderListener(this, &cAlpha), "alphaValue");
  addFloatListener(new HueListener(this, &cHue), "hueValue");
  addFloatListener(new HueListener(this, &cSaturation), "saturationValue");
  addFloatListener(new HueListener(this, &cLightness), "lightnessValue");
  addStringListener(new StringListener(this, &cRed), "redValueInput");
  addStringListener(new StringListener(this, &cGreen), "greenValueInput");
  addStringListener(new StringListener(this, &cBlue), "blueValueInput");
  addStringListener(new StringListener(this, &cAlpha), "alphaValueInput");
  addStringListener(new StringListenerHSL(this, &cHue), "hueValueInput");
  addStringListener(new StringListenerHSL(this, &cSaturation), "saturationValueInput");
  addStringListener(new StringListenerHSL(this, &cLightness), "lightnessValueInput");
  cRed = cColour->getRed();
  cGreen = cColour->getGreen();
  cBlue = cColour->getBlue();
  cAlpha = cColour->getAlpha();
  updateHSLSliders();
}

void ComponentColourSelector::updateColour() {
  if (isnan(cRed))        {cRed        = 0.0f;}
  if (isnan(cGreen))      {cGreen      = 0.0f;}
  if (isnan(cBlue))       {cBlue       = 0.0f;}
  if (isnan(cHue))        {cHue        = 0.0f;}
  if (isnan(cSaturation)) {cSaturation = 0.0f;}
  if (isnan(cLightness))  {cLightness  = 0.0f;}
  cColour->setRed(cRed);
  cColour->setGreen(cGreen);
  cColour->setBlue(cBlue);
  cColour->setAlpha(cAlpha);
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->resourceSelected(cColour);
  }
  setFloatValue("redValue", cRed);
  setFloatValue("greenValue", cGreen);
  setFloatValue("blueValue", cBlue);
  setFloatValue("alphaValue", cAlpha);
  setFloatValue("hueValue", cHue);
  setFloatValue("saturationValue", cSaturation);
  setFloatValue("lightnessValue", cLightness);
  setStringValue("redValueInput", cRed);
  setStringValue("greenValueInput", cGreen);
  setStringValue("blueValueInput", cBlue);
  setStringValue("alphaValueInput", cAlpha);
  setStringValue("hueValueInput", cHue);
  setStringValue("saturationValueInput", cSaturation);
  setStringValue("lightnessValueInput", cLightness);
  cResources->notifyChange(cColour);
}

void ComponentColourSelector::calculateHue(float& red, float& green, float& blue) {
  float mHue = cHue;
  red = 0.0f;
  green = 0.0f;
  blue = 0.0f;
  if (mHue <= 1.0f / 3.0f) {
    green = mHue * 6.0f;
    red = -green + 2.0f;
  } else if (mHue >= 2.0 / 3.0f) {
    mHue -= 2.0f / 3.0f;
    red = mHue * 6.0;
    blue = -red + 2.0f;
  } else {
    mHue -= 1.0f / 3.0f;
    blue = mHue * 6.0;
    green = -blue + 2.0f;
  }
  red = min(1.0f, red);
  green = min(1.0f, green);
  blue = min(1.0f, blue);
}

void ComponentColourSelector::updateHSLSliders() {
  if (cUpdating) {
    return;
  }
  cLightness = getLightness();
  float mMiddle = getMiddle();
  float mDark = getDarkness();
  float mHueShift = (mMiddle - mDark) / (cLightness - mDark);
  mHueShift /= 6.0f;
  if (cRed >= cGreen && cRed >= cBlue) {
    cHue = 0.0f;
    if (cBlue > cGreen) {
      mHueShift = -mHueShift;
    };
  } else if (cGreen >= cRed && cGreen >= cBlue) {
    cHue = 1.0f / 3.0f;
    if (cRed > cBlue) {
      mHueShift = -mHueShift;
    };
  } else {
    cHue = 2.0f / 3.0f;
    if (cGreen > cRed) {
      mHueShift = -mHueShift;
    };
  }
  cHue += mHueShift;
  if (cHue < 0.0f) {
    cHue += 1.0f;
  }
  cUpdating = true;
  cSaturation = mDark / cLightness;
  cUpdating = false;
  updateColour();
}

void ComponentColourSelector::updateRGBSliders() {
  if (cUpdating) {
    return;
  }
  calculateHue(cRed, cGreen, cBlue);
  cRed *= cLightness;
  cGreen *= cLightness;
  cBlue *= cLightness;
  cUpdating = true;
  float mMax = max(cRed, max(cGreen, cBlue));
  cRed += (mMax - cRed) * cSaturation;
  cGreen += (mMax - cGreen) * cSaturation;
  cBlue += (mMax - cBlue) * cSaturation;
  cUpdating = false;
  updateColour();
}

float ComponentColourSelector::getMiddle() {
  std::vector<float> cNumbers;
  cNumbers.push_back(cRed);
  unsigned int mGreenLocation = cGreen > cRed ? 1 : 0;
  cNumbers.insert(cNumbers.begin() + mGreenLocation, cGreen);
  if (cBlue < cNumbers[0]) {
    cNumbers.insert(cNumbers.begin() + 0, cBlue);
  } else if (cBlue < cNumbers[1]) {
    cNumbers.insert(cNumbers.begin() + 1, cBlue);
  } else {
    cNumbers.push_back(cBlue);
  }
  return cNumbers[1];
}

float ComponentColourSelector::getDarkness() {
  return min(cRed, min(cGreen, cBlue));
}

float ComponentColourSelector::getLightness() {
  return max(cRed, max(cGreen, cBlue));
}

void ComponentColourSelector::renderBackground(IRectangularComponent* slider) {
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  bool mBeginDark = false;
  glBegin(GL_QUADS);
  glColor3f(0.3f, 0.3f, 0.3f);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glColor3f(0.6f, 0.6f, 0.6f);
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  float mCellSizeY = 0.015f;
  float mCellSizeX = mCellSizeY * mAspectRatio;
  for (float mX = mLeft; mX < mRight; mX += mCellSizeX) {
    bool mSkip = mBeginDark;
    for (float mY = mBottom; mY < mTop; mY += mCellSizeY) {
      if (mSkip) {
	glVertex2f(mX,                           mY);
	glVertex2f(min(mRight, mX + mCellSizeX), mY);
	glVertex2f(min(mRight, mX + mCellSizeX), min(mTop, mY + mCellSizeY));
	glVertex2f(mX,                           min(mTop, mY + mCellSizeY));
      }
      mSkip = !mSkip;
    }
    mBeginDark = !mBeginDark;
  }
  glEnd();
}

ComponentColourSelector::ChannelRenderer::ChannelRenderer(ComponentColourSelector* parent, float* redStart, float* greenStart, float* blueStart, float* redEnd, float* greenEnd, float* blueEnd) {
  cParent = parent;
  cRedStart = redStart;
  cGreenStart = greenStart;
  cBlueStart = blueStart;
  cRedEnd = redEnd;
  cGreenEnd = greenEnd;
  cBlueEnd = blueEnd;
}

void ComponentColourSelector::ChannelRenderer::render(SliderComponent* slider) {
  cParent->renderBackground(slider);
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(*cRedEnd, *cGreenEnd, *cBlueEnd, cParent->cAlpha);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glColor4f(*cRedStart, *cGreenStart, *cBlueStart, cParent->cAlpha);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glEnd();
}

ComponentColourSelector::HueRenderer::HueRenderer(ComponentColourSelector* parent) {
  cParent = parent;
}

void ComponentColourSelector::HueRenderer::render(SliderComponent* slider) {
  cParent->renderBackground(slider);
  float mDark = cParent->getDarkness();
  float mLight = cParent->getLightness();
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  float mMid1 = mLeft + ((mRight - mLeft) / 6) * 1;
  float mMid2 = mLeft + ((mRight - mLeft) / 6) * 2;
  float mMid3 = mLeft + ((mRight - mLeft) / 6) * 3;
  float mMid4 = mLeft + ((mRight - mLeft) / 6) * 4;
  float mMid5 = mLeft + ((mRight - mLeft) / 6) * 5;
  glEnable(GL_BLEND);
  glBegin(GL_TRIANGLE_STRIP);
  glColor4f(mLight, mDark, mDark, cParent->cAlpha);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glColor4f(mLight, mLight, mDark, cParent->cAlpha);
  glVertex2f(mMid1, mTop);
  glVertex2f(mMid1, mBottom);
  glColor4f(mDark, mLight, mDark, cParent->cAlpha);
  glVertex2f(mMid2, mTop);
  glVertex2f(mMid2, mBottom);
  glColor4f(mDark, mLight, mLight, cParent->cAlpha);
  glVertex2f(mMid3, mTop);
  glVertex2f(mMid3, mBottom);
  glColor4f(mDark, mDark, mLight, cParent->cAlpha);
  glVertex2f(mMid4, mTop);
  glVertex2f(mMid4, mBottom);
  glColor4f(mLight, mDark, mLight, cParent->cAlpha);
  glVertex2f(mMid5, mTop);
  glVertex2f(mMid5, mBottom);
  glColor4f(mLight, mDark, mDark, cParent->cAlpha);
  glVertex2f(mRight, mTop);
  glVertex2f(mRight, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

ComponentColourSelector::SaturationRenderer::SaturationRenderer(ComponentColourSelector* parent) {
  cParent = parent;
}

void ComponentColourSelector::SaturationRenderer::render(SliderComponent* slider) {
  cParent->renderBackground(slider);
  float mRed = 0.0f;
  float mGreen = 0.0f;
  float mBlue = 0.0f;
  cParent->calculateHue(mRed, mGreen, mBlue);
  mRed *= cParent->cLightness;
  mGreen *= cParent->cLightness;
  mBlue *= cParent->cLightness;
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  float mLight = cParent->getLightness();
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(mLight, mLight, mLight, cParent->cAlpha);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glColor4f(mRed, mGreen, mBlue, cParent->cAlpha);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

ComponentColourSelector::LightnessRenderer::LightnessRenderer(ComponentColourSelector* parent) {
  cParent = parent;
}

void ComponentColourSelector::LightnessRenderer::render(SliderComponent* slider) {
  cParent->renderBackground(slider);
  float mRed = 0.0f;
  float mGreen = 0.0f;
  float mBlue = 0.0f;
  cParent->calculateHue(mRed, mGreen, mBlue);
  float mMax = max(mRed, max(mGreen, mBlue));
  mRed += (mMax - mRed) * cParent->cSaturation;
  mGreen += (mMax - mGreen) * cParent->cSaturation;
  mBlue += (mMax - mBlue) * cParent->cSaturation;
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  glEnable(GL_BLEND);
  glBegin(GL_TRIANGLE_STRIP);
  glColor4f(0.0f, 0.0f, 0.0f, cParent->cAlpha);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glColor4f(mRed, mGreen, mBlue, cParent->cAlpha);
  glVertex2f(mRight, mTop);
  glVertex2f(mRight, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

ComponentColourSelector::AlphaRenderer::AlphaRenderer(ComponentColourSelector* parent) {
  cParent = parent;
}

void ComponentColourSelector::AlphaRenderer::render(SliderComponent* slider) {
  cParent->renderBackground(slider);
  float mLeft = slider->getLeft();
  float mRight = slider->getRight();
  float mTop = slider->getTop();
  float mBottom = slider->getBottom();
  glEnable(GL_BLEND);
  glBegin(GL_TRIANGLE_STRIP);
  glColor4f(cParent->cRed, cParent->cGreen, cParent->cBlue, 0.0f);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glColor4f(cParent->cRed, cParent->cGreen, cParent->cBlue, 1.0f);
  glVertex2f(mRight, mTop);
  glVertex2f(mRight, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

ComponentColourSelector::PreviewRenderer::PreviewRenderer(ComponentColourSelector* parent) {
  cParent = parent;
}

void ComponentColourSelector::PreviewRenderer::render(PanelComponent* panel) {
  cParent->renderBackground(panel);
  float mLeft = panel->getLeft();
  float mRight = panel->getRight();
  float mTop = panel->getTop();
  float mBottom = panel->getBottom();
  glColor4f(cParent->cRed, cParent->cGreen, cParent->cBlue, cParent->cAlpha);
  glEnable(GL_BLEND);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mRight, mBottom);
  glEnd();
  glDisable(GL_BLEND);
}

ComponentColourSelector::SliderListener::SliderListener(ComponentColourSelector* parent, float* channel) {
  cChannel = channel;
  cParent = parent;
}
    
void ComponentColourSelector::SliderListener::floatValueChanged(float value) {
  (*cChannel) = value;
  cParent->updateHSLSliders();
}

ComponentColourSelector::HueListener::HueListener(ComponentColourSelector* parent, float* channel) {
  cParent = parent;
  cChannel = channel;
}

void ComponentColourSelector::HueListener::floatValueChanged(float value) {
  (*cChannel) = value;
  cParent->updateRGBSliders();
}

ComponentColourSelector::StringListener::StringListener(ComponentColourSelector* parent, float* channel) {
  cParent = parent;
  cChannel = channel;
}

void ComponentColourSelector::StringListener::valueChanged(const std::string& value) {
  (*cChannel) = min(1.0f, max(0.0f, atof(value.c_str())));
  cParent->updateHSLSliders();
}
                         
ComponentColourSelector::StringListenerHSL::StringListenerHSL(ComponentColourSelector* parent, float* channel) {
  cParent = parent;
  cChannel = channel;
}

void ComponentColourSelector::StringListenerHSL::valueChanged(const std::string& value) {
  (*cChannel) = min(1.0f, max(0.0f, atof(value.c_str())));
  cParent->updateRGBSliders();
}


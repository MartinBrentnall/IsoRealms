/*
 * Copyright 2015 Martin Brentnall
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
#include "SliderComponent.h"

SliderComponent::SliderComponent() {
  cValue = 0.5f;
  cSliderUpdating = false;
  cDragging = false;
  cRenderer = NULL;
}

void SliderComponent::setRenderer(ISliderRenderer* renderer) {
  cRenderer = renderer;
}

void SliderComponent::addFloatListener(IFloatListener* listener) {
  cValueListeners.push_back(listener);
}

void SliderComponent::render() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cRenderer != NULL) {
    cRenderer->render(this);
  }
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  float mHandleLocation = mLeft + ((mRight - mLeft) * cValue);
  glVertex2f(mHandleLocation, mTop);
  glVertex2f(mHandleLocation, mBottom);
  glEnd();
}

void SliderComponent::update(unsigned int milliseconds) {
  // Nothing to do
}

bool SliderComponent::mouseMotion(Uint16 x) {
  if (cDragging) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(x);
    float mLeft = getLeft();
    float mRight = getRight();
    float mWidth = mRight - mLeft;
    float mPosition = mX - mLeft;
    setValue(mPosition / mWidth);
    fireChange();
    return true;
  }
  return false;
}

bool SliderComponent::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    cDragging = true;
    return mouseMotion(event.button.x);
  }
  return false;
}

bool SliderComponent::keyDown(SDLKey key, SDLMod mod) {
  switch (key) {
    case SDLK_LEFT: {
      setValue(cValue - 0.02f);
      fireChange();
      return true;
    }

    case SDLK_RIGHT: {
      setValue(cValue + 0.02f);
      fireChange();
      return true;
    }

    case SDLK_HOME: {
      setValue(0.0f);
      fireChange();
      return true;
    }

    case SDLK_END: {
      setValue(1.0f);
      fireChange();
      return true;
    }
    
    default: {
      return false;
    }
  }
  return false;
}

bool SliderComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
	return true;
      }
      break;
    }
    
    case SDL_MOUSEMOTION: {
      if (cDragging) {
        return mouseMotion(event.motion.x);
      }
      break;
    }
    
    case SDL_MOUSEBUTTONUP: {
      cDragging = false;
      return true;
    }
    
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym, event.key.keysym.mod);
    }
  }
  return false;
}

void SliderComponent::fireChange() {
  cSliderUpdating = true;
  for (unsigned int i = 0; i < cValueListeners.size(); i++) {
    cValueListeners[i]->floatValueChanged(cValue);
  }
  cSliderUpdating = false;
}

void SliderComponent::setValue(float value) {
  if (!cSliderUpdating) {
    cValue = min(max(0.0f, value), 1.0f);
  }
}

float SliderComponent::getValue() {
  return cValue;
}

float SliderComponent::getWidth() {
  return 0.3f;
}

float SliderComponent::getHeight() {
  return 0.05f;
}

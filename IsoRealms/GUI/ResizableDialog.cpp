/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ResizableDialog.h"

const float ResizableDialog::RESIZE_HANDLE_SIZE = 0.05f;

ResizableDialog::ResizableDialog(IComponentContainer* componentContainer, const std::string& title, float x, float y, float width, float height) : Dialog(componentContainer, title, x, y, width, height) {
  cResizing = false;
}

ResizableDialog::ResizableDialog(IComponentContainer* componentContainer, const std::string& dialogDescriptionFile, IResourceAccessor* resourceAccessor) : Dialog(componentContainer, dialogDescriptionFile, resourceAccessor) {
  cResizing = false;
}

void ResizableDialog::renderDialogContent() {
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_STRIP);
  glColor3f(0.8f, 0.6f, 1.0f);
  // Resize handle
  glVertex2f(mRight,                                     mBottom + RESIZE_HANDLE_SIZE);
  glVertex2f(mRight - 0.01f * mAspectRatio,              mBottom + RESIZE_HANDLE_SIZE);
  glVertex2f(mRight - 0.01f * mAspectRatio,              mBottom + 0.01f); 
  glVertex2f(mRight - RESIZE_HANDLE_SIZE * mAspectRatio, mBottom + 0.01f); 
  glVertex2f(mRight - RESIZE_HANDLE_SIZE * mAspectRatio, mBottom);
  glEnd();

  renderResizableDialogContent();
}

void ResizableDialog::updateDialogContent(int ticks) {
  updateResizableDialogContent(ticks);
}

bool ResizableDialog::mouseButtonDown(SDL_Event& event) {
  float mRight = getRight();
  float mBottom = getBottom();
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    if (mX >= mRight - RESIZE_HANDLE_SIZE * mAspectRatio && mX <= mRight && mY >= mBottom && mY <= mBottom + RESIZE_HANDLE_SIZE) {
      cResizing = true;
      // TODO: Bring container to front?
      return true;
    }
  }
  return false;
}

bool ResizableDialog::mouseMotion(SDL_Event& event) {
  if (cResizing) {
    Configuration* mConfiguration = Configuration::getInstance();
    ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
    float mX = mScreen->getXLocation(event.motion.xrel) + 1.0f;
    float mY = mScreen->getYLocation(event.motion.yrel) - 1.0f;
    resize(mX, -mY);
    translate(0, mY);
    return true;
  }
  return false;
}

bool ResizableDialog::inputDialogContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (mouseButtonDown(event)) {
        return true;
      }
      break;
    }

    case SDL_MOUSEBUTTONUP: {
      bool cWasResizing = cResizing;
      cResizing = false;
      if (cWasResizing) {
        return true;
      }
      break;
    }

    case SDL_MOUSEMOTION: {
      if (mouseMotion(event)) {
        return true;
      }
      break;
    }

    default: {
      break;
    }
  }
  return inputResizableDialogContent(event);
}

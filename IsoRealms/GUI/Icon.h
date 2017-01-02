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
#ifndef ICON_H
#define ICON_H

#include <SDL/SDL.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/ISizedComponent.h>
#include <IsoRealms/MultipleClickDetector.h>

#include "IResourceBrowser.h"

template <class T> class Icon:public ISizedComponent {
  private:
  T* cResource;
  IResourceBrowser<T>* cBrowser;
  MultipleClickDetector cMultipleClickDetector;
  bool cSelected;
  
  public:
  Icon(IResourceBrowser<T>* browser, T* resource) {
    cBrowser = browser;
    cResource = resource;
    cSelected = false;
  }
    
  /******************************\
   * Implements ISizedComponent *
  \******************************/
  void update(unsigned int milliseconds) {
    cMultipleClickDetector.update(milliseconds);
    updateIcon(milliseconds);
  }

  virtual bool input(SDL_Event& event) {
    switch (event.type) {
      case SDL_MOUSEBUTTONDOWN: {
        Configuration* mConfiguration = Configuration::getInstance();
        ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
        float mX = mScreen->getXLocation(event.button.x);
        float mY = mScreen->getYLocation(event.button.y);
        if (contains(mX, mY)) {
          cMultipleClickDetector.input(event);
          cSelected = true;
          cBrowser->setSelected(this);
          iconSelected();
          if (cMultipleClickDetector.getClicks() == MultipleClickDetector::DOUBLE_CLICK && event.button.button == SDL_BUTTON_LEFT) {
            editResource();
          }
          return true;
        }
      }
    }
    return false;
  }
  
  void editResource() {
    cBrowser->editResource(cResource);
  }

  void removeResource() {
    cBrowser->removeResource(cResource);
  }
  
  T* getResource() {
    return cResource;
  }
  
  std::string getResourceLocation() {
    return cBrowser->getResourceLocation(cResource);
  }
  
  void setSelected(bool selected) {
    cSelected = selected;
  }
  
  virtual void render() {
    renderIcon();
    if (cSelected) {
      float mLeft = getLeft();
      float mRight = getRight();
      float mTop = getTop();
      float mBottom = getBottom();
      glLineWidth(2.0f);
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_LINE_LOOP);
      glVertex2f(mLeft,  mBottom);
      glVertex2f(mRight, mBottom);
      glVertex2f(mRight, mTop);
      glVertex2f(mLeft,  mTop);
      glEnd();
      glLineWidth(1.0f);
    }
  }
  
  virtual void updateIcon(unsigned int) {
  }
  
  virtual void renderIcon() {
  }
  
  virtual void iconSelected() {
  }
};

#endif

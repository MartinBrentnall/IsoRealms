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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <IsoRealms/Collision.h>
#include <IsoRealms/IModule.h>
#include <IsoRealms/IsoRealmsConstants.h>

#include "IElement.h"
#include "IElementType.h"
#include "PickedElement.h"

class Element:public IElement {
  public:
  virtual void updateRuntime(unsigned int milliseconds) {
  }
  
  virtual void renderEditing() {
    renderRuntime();
  }
  
  virtual void updateEditing(unsigned int milliseconds) {
  }
  
  virtual void input(SDL_Event&) {
  }
  
  virtual bool inputEditor(SDL_Event&, ILayer*) {
    return false;
  }
  
  virtual bool isVisualRuntime() {
    return true;
  }
  
  virtual bool isVisualEditing() {
    return true;
  }
  
  virtual bool isDynamicRuntime() {
    return true;
  }
  
  virtual bool isDynamicEditing() {
    return true;
  }
  
  virtual bool isInteractive() {
    return false;
  }

  bool initElement(IUniverse*, unsigned int) {
    return true;
  }
  
  virtual void removingElement() {
  };

  virtual void added() {
  }
  
  virtual void renderStaticEditing() {
  }

  virtual void initialiseResource(DOMNodeWrapper*, IResourceAccessor*) {
  }
  
  virtual void focusGained(ILayerEditingContext*) {
  }
  
  virtual void focusLost(ILayerEditingContext*) {
  }
  
  virtual bool isSelectable() {
    return true;
  }
  
  virtual void cursorMoved(ILayerEditingContext*, Vertex&, Vertex&) {
  }
  
  virtual void cursorAppeared(ILayerEditingContext*, Vertex&) {
  }

  virtual void processCursorMovement(ILayerEditingContext* editingContext, Vertex& start, Vertex& end) {
    IElementBounds* mBounds = getBounds();
    float mSouth  = mBounds->getSouth();
    float mWest   = mBounds->getWest();
    float mBottom = mBounds->getBottom();
    float mNorth  = mBounds->getNorth();
    float mEast   = mBounds->getEast();
    float mTop    = mBounds->getTop();
    bool mContainsStart = Collision::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
    bool mContainsEnd   = Collision::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
    if (isSelectable()) {
      if (!mContainsStart && mContainsEnd) {
        editingContext->addCursorElement(this);
        focusGained(editingContext);
      } else if (mContainsStart && !mContainsEnd) {
        focusLost(editingContext);
      }
    }
    if (mContainsStart || mContainsEnd) {
      cursorMoved(editingContext, start, end);
    }
  }
  
  virtual void processCursorAppearance(ILayerEditingContext* editingContext, Vertex& location) {
    IElementBounds* mBounds = getBounds();
    float mSouth  = mBounds->getSouth();
    float mWest   = mBounds->getWest();
    float mBottom = mBounds->getBottom();
    float mNorth  = mBounds->getNorth();
    float mEast   = mBounds->getEast();
    float mTop    = mBounds->getTop();
    if (Collision::contains(location, mWest, mEast, mSouth, mNorth, mBottom, mTop)) {
      if (isSelectable()) {
        editingContext->addCursorElement(this);
        focusGained(editingContext);
      }
      cursorAppeared(editingContext, location);
    }
  }
  
  virtual PickedElement* pickElement(Vertex& start, Vertex& end) {
    IElementBounds* mBounds = getBounds();
    float mSouth  = mBounds->getSouth();
    float mWest   = mBounds->getWest();
    float mBottom = mBounds->getBottom();
    float mNorth  = mBounds->getNorth();
    float mEast   = mBounds->getEast();
    float mTop    = mBounds->getTop();
    CollisionVertex* mPickedLocation = Collision::getEntryPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop);
    return mPickedLocation != nullptr ? new PickedElement(mPickedLocation, this) : nullptr;
  }

  virtual void reset() {
  }
  
  ~Element() {}
};

#endif

/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "LayerHUD.h"

LayerHUD::LayerHUD(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources, ILayerType* layerType, IElementRelationManager* manager) {
  cLayerType = layerType;
  cSelectedElement = nullptr;
  
  HUDComponentPosition* mScreenElement = new HUDComponentPosition(manager, &ScreenRelative::LEFT_EDGE, &ScreenRelative::RIGHT_EDGE, &ScreenRelative::BOTTOM_EDGE, &ScreenRelative::TOP_EDGE);
  manager->addRelatableElement(this, "screen", mScreenElement);
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      resources->loadElement(mNode, cache, nullptr, this, false);
    } else {
      // TODO: Throw
    }
  }
}

// std::string LayerHUD::getSource(HUDComponentPosition* component) {
//   for (std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.begin(); i != cComponentsByName.end(); i++) {
//     if (i->second->isComponent(component)) {
//       return i->first;
//     }
//   }
//   return "";
// }

void LayerHUD::resourceSelected(IElementType* elementType) {
  // TODO: Implement this!
}

void LayerHUD::initRuntime() {
  // Nothing to do
}

void LayerHUD::initEditor() {
  // Nothing to do
}

void LayerHUD::renderRuntime() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();

  glScalef(mAspectRatio, 1.0f, 1.0f);
  for (IElement* mElement : cElements) {
    mElement->renderRuntime();
  }
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

void LayerHUD::renderEditing() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();

  glScalef(mAspectRatio, 1.0f, 1.0f);
  for (IElement* mElement : cElements) {
    mElement->renderEditing();
  }
  if (cSelectedElement != nullptr) {
    cSelectedElement->renderSelectionHighlight();
  }
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

void LayerHUD::updateRuntime(unsigned int milliseconds) {
  for (IElement* mElement : cElements) {
    mElement->updateRuntime(milliseconds);
  }
}

void LayerHUD::updateEditing(unsigned int milliseconds) {
  for (IElement* mElement : cElements) {
    mElement->updateEditing(milliseconds);
  }
}

bool LayerHUD::input(SDL_Event& event) {
  return false;
}

bool LayerHUD::inputEditor(SDL_Event& event) {
  if (cSelectedElement != nullptr && cSelectedElement->inputEditor(event, this)) {
    return true;
  }
  
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      PickedElement* mPickedElement = nullptr;
      for (int i = cElements.size() - 1; i >= 0; i--) {
        ElementPickRay* mPickRay = getPickRay(event.button.x, event.button.y);
        mPickedElement = cElements[i]->pickElement(mPickRay->cStart, mPickRay->cEnd);
        if (mPickedElement != nullptr) {
          
          // HUD renders without depth, so we take the first element with a hit.
          break;
        }
      }
      cSelectedElement = mPickedElement != nullptr ? mPickedElement->getElement() : nullptr;
//      fireObjectSelectedEvent(cSelectedElement);
      glPopMatrix();
      return true;
    }
  }
  
  return false;
}

ElementPickRay* LayerHUD::getPickRay(float x, float y) {
  double mModelViewMatrix[16];
  double mProjectionMatrix[16];
  GLint mViewport[4];
  
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();

  // TODO: This is duplicate from render function
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();

  glScalef(mAspectRatio, 1.0f, 1.0f);
  
  glGetDoublev(GL_MODELVIEW_MATRIX, mModelViewMatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, mProjectionMatrix);
  glGetIntegerv(GL_VIEWPORT, mViewport);
  ElementPickRay* mRay = new ElementPickRay();
  Uint16 mInvertedY = mScreen->invertY(y);
  gluUnProject(x, mInvertedY, 0.0, mModelViewMatrix, mProjectionMatrix, mViewport, &mRay->cStart.x, &mRay->cStart.y, &mRay->cStart.z);
  gluUnProject(x, mInvertedY, 1.0, mModelViewMatrix, mProjectionMatrix, mViewport, &mRay->cEnd.x,   &mRay->cEnd.y,   &mRay->cEnd.z);
  return mRay;
}

void LayerHUD::save(DOMNodeWriter* node, IResourceLocator* resources) {
  node->addAttribute("type", resources->getPath(cLayerType));
  for (IElement* mElement : cElements) {
    DOMNodeWriter* mComponentNode = node->addBranch("Element");
//     std::string mComponentName = getSource(cComponents[i]);
//     mComponentNode->addAttribute("name", mComponentName);
    BlockLocation mBlockLocation(0, 0, 0);
    mElement->save(mComponentNode, resources, mBlockLocation);
  }
}

void LayerHUD::saveCache(DOMNodeWriter* cache) {
  for (IElement* mElement : cElements) {
//    BlockLocation mBlockLocation(0, 0, 0);
    mElement->saveCache(cache);
  }
}

void LayerHUD::staticChanged() {
  // Nothing to do
}

void LayerHUD::reset() {
  // Nothing to do
}

void LayerHUD::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // TODO
}

void LayerHUD::addElement(IElement* element) {
  cElements.push_back(element);
}

void LayerHUD::removeElement(IElement* element) {
  // TODO
}

void LayerHUD::updateElement(IElement* element) {
  // Nothing to do
}

void LayerHUD::addArgumentValue(IArgument* argument) {
  // Nothing to do
}

void LayerHUD::setArguments() {
  // Nothing to do
}

void LayerHUD::unsetArguments() {
  // Nothing to do
}

BlockArea* LayerHUD::getCoverage() {
  return nullptr; // Nothing to do
}

void LayerHUD::setDirty(IElement* element) {
  // Nothing to do
}

void LayerHUD::restrictCursor(Vertex& vertex) {
  // Nothing to do
}

IUniverse* LayerHUD::getUniverse() {
  return this;
}

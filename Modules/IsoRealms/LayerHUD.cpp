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

LayerHUD::LayerHUD(DOMNodeWrapper* node, IResourceAccessor* resources, ILayerType* layerType) {
  cLayerType = layerType;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      std::string mComponentSource = mNode->getAttribute("type");
      std::string mComponentName = mNode->getAttribute("name");
      float mScale = mNode->getFloatAttribute("scale");
      if (mScale <= 0.0f) {
        mScale = 1.0f;
      }

      IHUDComponentRelation* mLeftRelation   = getRelation(mNode->getAttribute("left"),   "left");
      IHUDComponentRelation* mRightRelation  = getRelation(mNode->getAttribute("right"),  "right");
      IHUDComponentRelation* mBottomRelation = getRelation(mNode->getAttribute("bottom"), "bottom");
      IHUDComponentRelation* mTopRelation    = getRelation(mNode->getAttribute("top"),    "top");
      HUDComponentPosition* mHUDRenderer = new HUDComponentPosition(mLeftRelation, mRightRelation, mTopRelation, mBottomRelation, mScale, mScale);
      resources->loadElement(mNode, nullptr, mHUDRenderer, false);
      cComponents.push_back(mHUDRenderer);
      HUDComponentProxy* mHUDComponentProxy = getComponentProxy(mComponentName);
      mHUDComponentProxy->setHUDComponentPosition(mHUDRenderer);
    } else {
      // TODO: Throw
    }
  }
}

HUDComponentProxy* LayerHUD::getComponentProxy(const std::string& name) {
  std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.find(name);
  if (i != cComponentsByName.end()) {
    return i->second;
  }
  HUDComponentProxy* mProxy = new HUDComponentProxy();
  cComponentsByName[name] = mProxy;
  return mProxy;
}

IHUDComponentRelation* LayerHUD::getRelation(const std::string& description, const std::string& edge) {
  if (description == "") {
    return nullptr;
  }
  std::vector<std::string> mRelationWords = Utils::splitWords(description);
  if (mRelationWords.size() == 2) {
    return new HUDComponentRelation(getComponentProxy(mRelationWords[1]), mRelationWords[0], edge);
  }
  if (mRelationWords.size() == 1) {
    return new ScreenRelation(atof(mRelationWords[0].c_str()));
  }
  return nullptr;
}

std::string LayerHUD::getSource(HUDComponentPosition* component) {
  for (std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.begin(); i != cComponentsByName.end(); i++) {
    if (i->second->isComponent(component)) {
      return i->first;
    }
  }
  return "";
}

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
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->render();
  }
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
}

void LayerHUD::renderEditing() {
  // TODO:
}

void LayerHUD::updateRuntime(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->update(milliseconds);
  }
}

void LayerHUD::updateEditing(unsigned int milliseconds) {
  // TODO
}

bool LayerHUD::input(SDL_Event& event) {
  return false;
}

bool LayerHUD::inputEditor(SDL_Event& event) {
  return false; // TODO: Implement this
}

void LayerHUD::save(DOMNodeWriter* node, IResourceLocator* resources) {
  node->addAttribute("type", resources->getPath(cLayerType));
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    DOMNodeWriter* mComponentNode = node->addBranch("Element");
    std::string mComponentName = getSource(cComponents[i]);
    mComponentNode->addAttribute("name", mComponentName);
    cComponents[i]->save(mComponentNode, this, resources);
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

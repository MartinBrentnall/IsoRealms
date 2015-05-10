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
#include "ResourceLayerHUD.h"

ResourceLayerHUD::ResourceLayerHUD(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

void ResourceLayerHUD::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      std::string mComponentSource = mNode->getAttribute("type");
      float mScale = mNode->getFloatAttribute("scale");
      if (mScale <= 0.0f) {
        mScale = 1.0f;
      }

      IHUDComponentRelation* mLeftRelation   = getRelation(mNode->getAttribute("left"),   "left");
      IHUDComponentRelation* mRightRelation  = getRelation(mNode->getAttribute("right"),  "right");
      IHUDComponentRelation* mBottomRelation = getRelation(mNode->getAttribute("bottom"), "bottom");
      IHUDComponentRelation* mTopRelation    = getRelation(mNode->getAttribute("top"),    "top");
      HUDComponentPosition* mHUDRenderer = new HUDComponentPosition(mLeftRelation, mRightRelation, mTopRelation, mBottomRelation, mScale, mScale);
      resources->loadElement(mNode, nullptr, mHUDRenderer);
      cComponents.push_back(mHUDRenderer);
      HUDComponentProxy* mHUDComponentProxy = getComponentProxy(mComponentSource);
      mHUDComponentProxy->setHUDComponentPosition(mHUDRenderer);
    } else {
      // TODO: Throw
    }
  }
}

HUDComponentProxy* ResourceLayerHUD::getComponentProxy(const std::string& name) {
  std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.find(name);
  if (i != cComponentsByName.end()) {
    return i->second;
  }
  HUDComponentProxy* mProxy = new HUDComponentProxy();
  cComponentsByName[name] = mProxy;
  return mProxy;
}

IHUDComponentRelation* ResourceLayerHUD::getRelation(const std::string& description, const std::string& edge) {
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

std::string ResourceLayerHUD::getSource(HUDComponentPosition* component) {
  for (std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.begin(); i != cComponentsByName.end(); i++) {
    if (i->second->isComponent(component)) {
      return i->first;
    }
  }
  return "";
}

void ResourceLayerHUD::resourceSelected(IElementType* elementType) {
  // TODO:
}

void ResourceLayerHUD::initRuntime() {
  // Nothing to do
}

void ResourceLayerHUD::initEditor() {
  // Nothing to do
}

void ResourceLayerHUD::renderRuntime() {
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

void ResourceLayerHUD::renderEditing() {
  // TODO:
}

void ResourceLayerHUD::updateRuntime(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->update(milliseconds);
  }
}

void ResourceLayerHUD::updateEditing(unsigned int milliseconds) {
  // TODO
}

void ResourceLayerHUD::input(SDL_Event& event) {
  // Nothing to do
}

void ResourceLayerHUD::save(DOMNodeWriter* node, IResourceLocator* resources) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    DOMNodeWriter* mComponentNode = node->addBranch("Component");
    std::string mSource = getSource(cComponents[i]);
    mComponentNode->addAttribute("source", mSource);
    cComponents[i]->save(mComponentNode, this);
  }
}

ILayer* ResourceLayerHUD::getLayer(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor, bool editing) {
  initialiseResource(node, resourceAccessor);
  return this;
}

void ResourceLayerHUD::staticChanged() {
  // Nothing to do
}

void ResourceLayerHUD::reset() {
  // Nothing to do
}

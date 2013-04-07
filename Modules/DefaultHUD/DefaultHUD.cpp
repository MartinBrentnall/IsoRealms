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
#include "DefaultHUD.h"

DefaultHUD::DefaultHUD(DOMNodeWrapper* node, IResourceAccessor* resources, IDefaultHUDType* elementType) {
  cElementType = elementType;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Component") {
      std::string mComponentSource = mNode->getAttribute("source");
      float mScale = mNode->getFloatAttribute("scale");
      if (mScale <= 0.0f) {
        mScale = 1.0f;
      }

      IHUDComponentFactory* mFactory = resources->getHUDComponentType(mComponentSource);
      if (mFactory != NULL) {
        IHUDGameComponent* mHUDComponent = mFactory->getHUDComponent();
        IHUDComponentRelation* mLeftRelation   = getRelation(mNode->getAttribute("left"),   "left");
        IHUDComponentRelation* mRightRelation  = getRelation(mNode->getAttribute("right"),  "right");
        IHUDComponentRelation* mBottomRelation = getRelation(mNode->getAttribute("bottom"), "bottom");
        IHUDComponentRelation* mTopRelation    = getRelation(mNode->getAttribute("top"),    "top");
        HUDComponentPosition* mHUDRenderer = new HUDComponentPosition(mHUDComponent, mLeftRelation, mRightRelation, mTopRelation, mBottomRelation, mScale, mScale);
        cComponents.push_back(mHUDRenderer);
        HUDComponentProxy* mHUDComponentProxy = getComponentProxy(mComponentSource);
        mHUDComponentProxy->setHUDComponentPosition(mHUDRenderer);
      } else {
        std::cout << "FACTORY IS NULL!" << std::endl;
      }
    } else {
      // TODO: Throw
    }
  }
}

HUDComponentProxy* DefaultHUD::getComponentProxy(const std::string& name) {
  std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.find(name);
  if (i != cComponentsByName.end()) {
    return i->second;
  }
  HUDComponentProxy* mProxy = new HUDComponentProxy();
  cComponentsByName[name] = mProxy;
  return mProxy;
}

IHUDComponentRelation* DefaultHUD::getRelation(const std::string& description, const std::string& edge) {
  if (description == "") {
    return NULL;
  }
  std::vector<std::string> mRelationWords = Utils::splitWords(description);
  if (mRelationWords.size() == 2) {
    return new HUDComponentRelation(getComponentProxy(mRelationWords[1]), mRelationWords[0], edge);
  }
  if (mRelationWords.size() == 1) {
    return new ScreenRelation(atof(mRelationWords[0].c_str()));
  }
  return NULL;
}

std::string DefaultHUD::getSource(HUDComponentPosition* component) {
  for (std::map<std::string, HUDComponentProxy*>::iterator i = cComponentsByName.begin(); i != cComponentsByName.end(); i++) {
    if (i->second->isComponent(component)) {
      return i->first;
    }
  }
  return "";
}

IPlugin* DefaultHUD::getElementSet() {
  return NULL;
}

IElementType* DefaultHUD::getElementType() {
  return cElementType;
}

bool DefaultHUD::initElement(unsigned int) {
  return true;
  // Nothing to do
}

void DefaultHUD::renderStatic() {
  // Nothing to do
}

void DefaultHUD::renderRuntime() {
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

void DefaultHUD::renderEditing() {
  // TODO:
}

void DefaultHUD::updateRuntime(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->update(milliseconds);
  }
}

void DefaultHUD::updateEditing(unsigned int milliseconds) {
  // TODO
}

void DefaultHUD::input(SDL_Event& event) {
  // Nothing to do
}

bool DefaultHUD::isVisualRuntime() {
  return true;
}

bool DefaultHUD::isVisualEditing() {
  return false;
}

bool DefaultHUD::isDynamicRuntime() {
  return true;
}

bool DefaultHUD::isDynamicEditing() {
  return false;
}

bool DefaultHUD::isInteractive() {
  return false;
}

void DefaultHUD::save(DOMNodeWriter* node, IResourceLocator* resources, BlockLocation& location) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    DOMNodeWriter* mComponentNode = node->addBranch("Component");
    std::string mSource = getSource(cComponents[i]);
    mComponentNode->addAttribute("source", mSource);
    cComponents[i]->save(mComponentNode, this);
  }
}

void DefaultHUD::setDirty() {
}

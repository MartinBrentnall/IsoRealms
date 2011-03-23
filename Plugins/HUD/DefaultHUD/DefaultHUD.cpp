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

void DefaultHUD::registerHUDComponentFactory(IHUDComponentFactory* componentFactory) {
  std::string mFactoryName = componentFactory->getHUDComponentFactoryName();
  // TODO: Check if it already exists first!
  cHUDComponentSources[mFactoryName] = componentFactory;
}

void DefaultHUD::unregisterHUDComponentFactory(IHUDComponentFactory* componentFactory) {
  std::string mFactoryName = componentFactory->getHUDComponentFactoryName();
  // TODO: Verify that the erased factory is this one!
  // TODO: Verify that the name exists in the map!
  cHUDComponentSources.erase(mFactoryName);
}

void DefaultHUD::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Component") {
      std::string mComponentSource = mNode->getAttribute("source");
      std::string mComponentName = mNode->getAttribute("component");
      std::string mComponentAlign = mNode->getAttribute("align");
      std::string mComponentLeft = mNode->getAttribute("left");
      std::string mComponentRight = mNode->getAttribute("right");
      float mScale = mNode->getFloatAttribute("scale");
      if (mScale <= 0.0f) {
        mScale = 1.0f;
      }
      
      IHUDComponentFactory* mFactory = cHUDComponentSources[mComponentSource];
      if (mFactory != NULL) {
        IHUDGameComponent* mHUDComponent = mFactory->getHUDComponent(mComponentName);

        std::vector<std::string> mAlignWords = Utils::splitWords(mComponentAlign);
        float mX = 0.0f;
        float mY = 0.0f;
        float mXAlign = 0.0f;
        float mYAlign = 0.0f;
        for (unsigned int i = 0; i < mAlignWords.size(); i++) {
          if      (mAlignWords[i] == "left")   {mX = -1.0f; mXAlign = -1.0f;}
          else if (mAlignWords[i] == "right")  {mX =  1.0f; mXAlign =  1.0f;}
          else if (mAlignWords[i] == "top")    {mY =  1.0f; mYAlign =  1.0f;}
          else if (mAlignWords[i] == "bottom") {mY = -1.0f; mYAlign = -1.0f;}
          else {
            std::cout << "WARNING: Unknown word in alignment attribute: \"" << mAlignWords[i] << "\"" << std::endl;
          }
        }
        
        HUDComponentPosition* mHUDRenderer = new HUDComponentPosition(mHUDComponent);

        if (mComponentRight != "") {
          mXAlign = 1.0f;
          if (mComponentRight.find("/") != std::string::npos) {
            mHUDRenderer->setXPosition(cComponentsByName[mComponentRight]);
          } else {
            mX = mNode->getFloatAttribute("right");
          }
        }
        if (mComponentLeft != "") {
          mXAlign = -1.0f;
          if (mComponentLeft.find("/") != std::string::npos) {
            mHUDRenderer->setXPosition(cComponentsByName[mComponentLeft]);
          } else {
            mX = mNode->getFloatAttribute("left");
          }
        }

        // TODO: Set position
        mHUDRenderer->setScale(mScale);
        mHUDRenderer->setXPosition(mX);
        mHUDRenderer->setYPosition(mY);
        mHUDRenderer->setXAlign(mXAlign);
        mHUDRenderer->setYAlign(mYAlign);
        cComponents.push_back(mHUDRenderer);
        cComponentsByName[mComponentSource + "/" + mComponentName] = mHUDRenderer;
      } else {
        std::cout << "FACTORY IS NULL!" << std::endl;
      }
    } else {
      // TODO: Throw
    }
  }
}

void DefaultHUD::update(int milliseconds) {
  for (unsigned int i = 0; i < cComponents.size(); i++) {
    cComponents[i]->update(milliseconds);
  }
}

void DefaultHUD::render() {
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

std::vector<IDynamicElement*> DefaultHUD::getPostLoopCommands() {
  std::vector<IDynamicElement*> mThis;
  mThis.push_back(this);
  return mThis;
}

std::vector<IVisualElement*> DefaultHUD::getPostLoopRenderers() {
  std::vector<IVisualElement*> mThis;
  mThis.push_back(this);
  return mThis;
}

extern "C" IPlugin* create() {
  return new DefaultHUD();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

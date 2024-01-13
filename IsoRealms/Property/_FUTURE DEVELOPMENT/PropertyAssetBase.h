/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

#include <cmath>
#include <limits>
#include <set>
#include <stack>
#include <GL/glew.h>

#include "IsoRealms/Types.h"

#include "PropertyAsset.h"

namespace IsoRealms {
  template <class TYPE> class PropertyAssetBase : public PropertyAsset {
    private:
    IAssetBrowser* cBrowser;
    TYPE** cAsset;
    IAssetUser<TYPE>* cClient;
    std::function<void(TYPE* asset)> cAddCallback;
    std::function<void()> cRemoveCallback;
    std::function<void()> cUpdateCallback;
    const std::string& cRemoveID;
    
    virtual TYPE* getAsset(IAssetBrowser* browser, IAssetUser<TYPE>* client, const std::string& id) const = 0;
    virtual bool renderIcon(IPropertyAppearance* appearance, IAssetBrowser* browser, const std::string& id) const = 0;
    
    bool confirmSelection(const std::string& id) {
      if (cAsset != nullptr) {
        cBrowser->release(cClient, *cAsset);
        *cAsset = getAsset(cBrowser, cClient, id);
        cUpdateCallback();
        if (id == cRemoveID) {
          cRemoveCallback();
        }
      } else {
        cAddCallback(getAsset(cBrowser, cClient, id));
        // FIXME:IsoRealmsEditor: Reset back to none
      }
      return true;
    }
    
    protected:
    
    class GlobalOption : public IAssetMenuItem {
      private:
      PropertyAssetBase* cParent;
      const std::string cLabel;
      const std::string cValue;
      std::function<void()> cIcon;
      
      public:
      GlobalOption(PropertyAssetBase* parent, std::function<void()> icon, const std::string& label, const std::string& value) :
            cParent(parent),
            cLabel(label),
            cValue(value),
            cIcon(icon) {
      }
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(IPropertyAppearance* appearance, float yOffset) const override {
        glPushMatrix();
        appearance->transformForIcon(yOffset);
        cIcon();
        glPopMatrix();
        appearance->print(cLabel, yOffset, appearance->getIconWidth() + appearance->getIconTextSpacing());
      }
      
      bool executeAction() override {
        return cParent->confirmSelection(cValue);
      }
      
      float getWidth(IPropertyAppearance* appearance) const override {
        return appearance->getTextWidth(cLabel) + appearance->getIconWidth() + appearance->getIconWidth();
      }
      
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override {
        std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
        if (id == cValue) {
          mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item, 0.0f));
        }
        return mSelection;
      }
    };

    public:
    PropertyAssetBase(const std::string& label, TYPE** asset, IAssetUser<TYPE>* client, IAssetBrowser* browser, std::vector<std::pair<std::string, std::string>> options, std::vector<IAssetMenuItem*> globalOptions, std::function<void()> updateCallback = []() {}, std::function<void()> removeCallback = []() {}, const std::string& removeID = "#") :
            PropertyAsset(label, browser->getID(*asset), options, globalOptions, [this](const std::string& id) {
              return confirmSelection(id);
            }),
            cBrowser(browser),
            cAsset(asset),
            cClient(client),
            cRemoveCallback(removeCallback),
            cUpdateCallback(updateCallback),
            cRemoveID(removeID) {
    }
    
    PropertyAssetBase(const std::string& label, IAssetUser<TYPE>* client, IAssetBrowser* browser, std::vector<std::pair<std::string, std::string>> options, std::vector<IAssetMenuItem*> globalOptions, std::function<void(TYPE* asset)> addCallback) :
            PropertyAsset(label, "#", options, globalOptions, [this](const std::string& id) {
              return confirmSelection(id);
            }),
            cBrowser(browser),
            cAsset(nullptr),
            cClient(client),
            cAddCallback(addCallback),
            cRemoveID("#") {
    }
    
    /****************************\
     * Implements PropertyAsset *
    \****************************/
    bool renderIcon(IPropertyAppearance* appearance, const std::string& id) const override {
      return renderIcon(IPropertyAppearance* appearance, cBrowser, id);
    }
  };
}

/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cmath>
#include <regex>
#include <set>
 
#include <GL/glew.h>
 
#include "IsoRealms/Common/AnimatedFloat.h"
#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Editing/Choice.h"
#include "IsoRealms/Editing/IResourceAccessManager.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Editing/UISignalID.h"
#include "IsoRealms/Application.h"
#include "IsoRealms/IResourceTypeDefinition.h"
#include "IsoRealms/Utils.h"
 
#include "IPropertyEditor.h"
#include "IPropertyManager.h"
#include "Property.h"
#include "PropertyAsset.h"
 
namespace IsoRealms {
  template <typename TYPE, typename MANAGER> class PropertyResource : public Property {
    public:
    PropertyResource(PropertyMaker& owner, IResourceAccessManager& resourceAccessManager, IResourceData& resourceData, const PropertyData& data, MANAGER& manager, TYPE*& resource, Application& application, std::function<void()> removeFunction = nullptr) :
             Property(data, resourceAccessManager, removeFunction),
             cApplication(application),
             cManager(manager),
             cResource(resource),
             cResourceSeletor(*this),
             cSubProperty(owner, resourceAccessManager, resourceData, data, cResourceSeletor) {
    }

    std::string getValue() const {
      return cResource == nullptr ? "None" : cManager.getResourceID(cResource);
    }

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, getValue());
      glPushMatrix();
      glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize, mFontSize, 0.0f);
      if (cResource != nullptr) {
        if (!cResource->renderIcon()) {
          Utils::renderIconLeaf();
        }
      } else {
        Utils::renderIconNone();
      }
      glPopMatrix();
    }

    float getValueWidth(IUIStyle& style) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      return mFont->getWidth(mFontSize, getValue()) + mFontSize * 2.25f;
    }

    void confirm(IPropertyManager& manager, float y) override {
      cSubProperty.confirm(manager, y);
    }

    bool hasConfiguration() const override {
      return false;
    }

    void configure(IPropertyManager& manager) override {
      // Nothing to do.
    }

    private:
    class PropertyResourceType {
      public:
      PropertyResourceType(PropertyResource& parent) :
                cParent(parent) {
      }

      std::string getID() const {
        return cParent.getValue();
      }
      
      bool renderAssetIcon() const {
        return cParent.cResource->renderIcon();
      }

      bool hasConfiguration() const {
        return false;
      }

      void getAssetProperties(PropertyMaker& owner) {
        // Nothing to do.
      }

      Application& getApplication() const {
        return cParent.cApplication;
      }

      std::vector<std::string> getAvailableProviders() const {
        std::vector<std::string> mAvailableResources = cParent.cManager.template getAvailableResources<TYPE>();
        mAvailableResources.emplace_back("None");
        return mAvailableResources;
      }

      bool renderProviderIcon(const std::string& id) const {
        if (id == "None") {
          Utils::renderIconNone();
        } else {
          Utils::renderIconLeaf();
        }
        return true;
      }

      void setID(const std::string& id) {
        std::cout << "setID: " << id << std::endl;
        if (id == "None") {
          cParent.cResource = nullptr;
        } else {
          cParent.cResource = cParent.cManager.template get<TYPE>(id);
        }
      }

      bool isDefaultConfigured() const {
        return true;
      }

      private:
      PropertyResource& cParent;
    };

    Application& cApplication;
    MANAGER& cManager;
    TYPE*& cResource;
    PropertyResourceType cResourceSeletor;
    PropertyAsset<PropertyResourceType> cSubProperty;
  };
}
 
 
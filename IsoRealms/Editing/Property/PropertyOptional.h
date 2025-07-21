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

#include <limits>
#include <functional>
#include <string>

#include <GL/glew.h>

#include "IsoRealms/IProject.h"
#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"
#include "Property.h"

namespace IsoRealms {
  class Project;

  template<class TYPE> class PropertyOptional : public Property {
    public:
    PropertyOptional(IPropertyOwner& owner, const PropertyData& data, std::function<void(const std::string&)> choiceCallback, Project& project, IApplication& application) :
            Property(data, nullptr),
            cSimulatedType(project),
            cWrapperType(*this),
            cSubProperty(owner, data, cWrapperType),
            cChoiceCallback(choiceCallback),
            cApplication(application) {
    }

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, "None");
      glPushMatrix();
      glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize, mFontSize, 0.0f);
      Utils::renderIconNone();
      glPopMatrix();
    }

    float getValueWidth(IUIStyle& style) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      return mFont->getWidth(mFontSize, "None") + mFontSize * 2.25f;
    }

    void confirm(IPropertyManager& manager, float y) override {
      cPropertyManager = &manager;
      cSubProperty.confirm(manager, y);
    }

    bool hasConfiguration() const override {
      return false;
    }

    void configure(IPropertyManager& manager) override {
      // Nothing to do.
    }

    private:
    class OptionWrapper {
      public:
      OptionWrapper(PropertyOptional& parent) :
              cParent(parent) {
      }

      /*******************************\
       * Interface for PropertyAsset *
      \*******************************/
      std::string getID() const {
        return "None";
      }

      bool renderAssetIcon() const {
        Utils::renderIconNone();
        return true;
      }

      bool hasConfiguration() const {
        return false;
      }

      bool isDefaultConfigured() const {
        return true;
      }

      std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) {
        return std::vector<std::unique_ptr<IProperty>>();
      }

      IApplication& getApplication() const {
        return cParent.cApplication;
      }

      std::vector<std::string> getAvailableProviders() const {
        std::vector<std::string> mAvailableOptions = cParent.cSimulatedType.getAvailableProviders();
        mAvailableOptions.emplace_back("None");
        return mAvailableOptions;
      }

      bool renderProviderIcon(const std::string& id) const {
        return false; // TODO cParent.cSubProperty.renderProviderIcon(id);
      }

      void setID(const std::string& id) {
        if (id != "None") {
          cParent.cChoiceCallback(id);
//          cParent.cPropertyManager->refreshProperties(); // TODO: Causes a crash when loading a module.
        }
      }
      
      private:
      PropertyOptional& cParent;
    };
    
    TYPE cSimulatedType;
    OptionWrapper cWrapperType;
    std::string cValue;
    PropertyAsset<OptionWrapper> cSubProperty;
    std::function<void(const std::string&)> cChoiceCallback;
    IPropertyManager* cPropertyManager;
    IApplication& cApplication;
  };
}

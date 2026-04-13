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

#include <functional>
#include <string>

#include <GL/glew.h>

#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"
#include "IOptionalObject.h"
#include "Property.h"
#include "PropertyTreeSelector.h"

namespace IsoRealms {
  class Project;
  class PropertyMaker;

  class PropertyOptional : public Property {
    public:
    PropertyOptional(PropertyMaker& owner, IResourceAccessManager& resourceAccessManager, IResourceData& resourceData, const PropertyData& data, std::function<void(const std::string&)> choiceCallback, Project& project, Application& application, IOptionalObject& optionalSource);

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    class OptionWrapper : public ITreeSelectorObject {
      public:
      OptionWrapper(PropertyOptional& parent);

      /*******************************\
       * Implements ITreeSelectorObject *
      \*******************************/
      TreeItemInfo getTreeItemInfo() const override;
      std::string getTreeItemLabel() const override;
      bool renderAssetIcon() const override;
      bool hasConfiguration() const override;
      bool isDefaultConfigured() const override;
      void getAssetProperties(PropertyMaker& owner) override;
      Application& getApplication() override;
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
      bool renderTreeItemIcon(const std::string& id) const override;
      void setID(const std::string& id) override;

      private:
      PropertyOptional& cParent;
    };

    IOptionalObject& cOptionalSource;
    OptionWrapper cWrapperType;
    PropertyTreeSelector cSubProperty;
    std::function<void(const std::string&)> cChoiceCallback;
    IPropertyManager* cPropertyManager;
    Project& cProject;
    Application& cApplication;
  };
}

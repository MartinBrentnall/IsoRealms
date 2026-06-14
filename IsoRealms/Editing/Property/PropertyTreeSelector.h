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
#include <memory>
#include <string>
#include <vector>

#include <SFML/Window.hpp>

#include "IsoRealms/Application.h"
#include "IsoRealms/Common/AnimatedFloat.h"
#include "IsoRealms/Editing/Choice.h"
#include "IsoRealms/Editing/IComponentAccessManager.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Editing/UISignalID.h"
#include "IsoRealms/Project/Registry/TreeItemInfo.h"

#include "IPropertyEditor.h"
#include "IPropertyManager.h"
#include "ITreeSelectorObject.h"
#include "Property.h"

namespace IsoRealms {
  class IComponentData;

  class PropertyTreeSelector : public Property {
    public:
    PropertyTreeSelector(IComponentDefiner& definer, IComponentAccessManager& resourceAccessManager, IComponentData& resourceData, const PropertyData& data, ITreeSelectorObject& item, std::function<void()> removeFunction = nullptr);

    std::string getValue();

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    class Editor : public IPropertyEditor {
      public:
      Editor(PropertyTreeSelector& parent, IUIStyle& style, float y, float width, float height);
      void openFolder(const std::string& path);
      void updateAnimationValues();
      void render(IUIStyle& style, float y, float x, float aspectRatio) const override;
      void updatePathBar(IUIStyle& style);
      bool update(unsigned int milliseconds) override;
      bool input(UISignalID id, IUIStyle& style) override;
      bool input(sf::Event& event, IUIStyle& style) override;

      private:
      class Menu {
        public:
        Menu(Editor& parent, const std::string& path, const TreeItemInfo& currentEntry);
        unsigned int getMenuItemCount() const;
        void render(IUIStyle& style, float y, float x, float aspectRatio) const;
        float getWidth(IUIStyle& style) const;
        float getSelectedItemWidth(IUIStyle& style) const;
        void update(unsigned int milliseconds);
        float getSelectedItemAnimation() const;
        bool input(UISignalID id, IUIStyle& style);
        unsigned int getSelectedItem() const;
        std::string getPath() const;
        bool hasChildren() const;

        private:
        class IMenuEntry {
          public:
          virtual void render(IUIStyle& style, float y, float x, float aspectRatio) const = 0;
          virtual float getWidth(IUIStyle& style) const = 0;
          virtual bool confirm(IUIStyle& style) = 0;
          virtual void enter(IUIStyle& style) = 0;
          virtual bool hasChildren() const = 0;
        };

        class MenuFolder : public IMenuEntry {
          public:

          /*************************\
           * Implements IMenuEntry *
          \*************************/
          MenuFolder(Menu& parent, const std::string& folder);
          void render(IUIStyle& style, float y, float x, float aspectRatio) const override;
          float getWidth(IUIStyle& style) const override;
          bool confirm(IUIStyle& style) override;
          void enter(IUIStyle& style) override;
          bool hasChildren() const override;

          private:
          Menu& cParent;
          std::string cFolderName;
        };

        class MenuItem : public IMenuEntry {
          public:
          MenuItem(Menu& parent, const TreeItemInfo& itemInfo);

          /*************************\
           * Implements IMenuEntry *
          \*************************/
          void render(IUIStyle& style, float y, float x, float aspectRatio) const override;
          float getWidth(IUIStyle& style) const override;
          bool confirm(IUIStyle& style) override;
          void enter(IUIStyle& style) override;
          bool hasChildren() const override;

          private:
          Menu& cParent;
          TreeItemInfo cItemInfo;
          std::string cLabel;

          static std::string getLabelFromPath(const std::string& path);
        };

        void applyChange(const std::string& id);

        Editor& cParent;
        std::string cPath;
        std::vector<std::unique_ptr<IMenuEntry>> cMenuItems;
        unsigned int cSelectedIndex = 0;
        AnimatedFloat cSelectedItem;
      };

      class MenuScroller {
        public:
        enum class State {
          ACTIVE,
          INACTIVE,
          NEW
        };

        MenuScroller(Editor& parent, const std::string& path, const TreeItemInfo& currentEntry, State state);
        void render(IUIStyle& style, float y, float x, float aspectRatio) const;
        void updateActive(unsigned int milliseconds);
        void updateInactive(unsigned int milliseconds);
        void updateClosing(unsigned int milliseconds);
        bool isClosed() const;
        Menu& getMenu();

        private:
        Menu cMenu;
        int cScroll;
      };

      PropertyTreeSelector& cParent;
      IUIStyle& cStyle;

      float cY;

      AnimatedFloat cYPosition;
      AnimatedFloat cWidth;
      AnimatedFloat cHeight;
      AnimatedFloat cSelectionWidth;
      AnimatedFloat cBackArrowSize;
      int cOpenness = 0;
      bool cClosing = false;
      std::vector<std::unique_ptr<MenuScroller>> cMenus;
      std::vector<std::unique_ptr<MenuScroller>> cClosedMenus;
      std::unique_ptr<Choice> cConfirmSelection;
      std::unique_ptr<Choice> cClosedConfirmSelection;

      std::string cCurrentPath;
      std::string cActivePath;
      std::string cInactivePath;
      float cActivePartWidth;
      AnimatedFloat cPathBarWidth = 0.0f;
    };

    IComponentDefiner& cPropertyOwner;
    IComponentData& cComponentData;
    ITreeSelectorObject& cSelectedItem;
    std::string cValueLabel;
  };
}

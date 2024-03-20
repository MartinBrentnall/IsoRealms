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

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "Property.h"

namespace IsoRealms {
  class ISpecialItemEditorListener {
    public:
    virtual void setSpecialItemSelected(int item) = 0;
  };
  
  class PropertyAsset : public Property,
                        public ISpecialItemEditorListener {
    public:
    class AssetMenu;
    class ISpecialItemEditor;
    
    class ISelection {
      public:
      virtual unsigned int getItemCount() const = 0;
      virtual unsigned int getIndex() const = 0;
      virtual float getScroll() const = 0; 
      virtual bool input(sf::Event& event) = 0;
      virtual void updateEditing(unsigned int milliseconds, bool active) = 0;
      virtual bool updateForClosure(unsigned int milliseconds) = 0; 
      virtual void renderEditing(IPropertyAppearance* appearance) const = 0;
      virtual void render(IPropertyAppearance* appearance) const = 0;
      virtual float getSelectedWidth(IPropertyAppearance* appearance) const = 0;
      virtual float getMaxWidth(IPropertyAppearance* appearance) const = 0;

      virtual ~ISelection() {
      }
    };
    
    class IAssetMenuItem {
      public:
      virtual void render(IPropertyAppearance* appearance, float yOffset) const = 0;
      virtual bool executeAction() = 0;
      virtual float getWidth(IPropertyAppearance* appearance) const = 0;
      virtual std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const = 0;

      virtual ~IAssetMenuItem() {
      }
    };
    
    class AssetMenu {
      private:
      PropertyAsset* cParent;
      std::vector<std::unique_ptr<IAssetMenuItem>> cMenuItems;
      
      public:
        
      class Selection : public ISelection {
        private:
        const AssetMenu* cParent;
        unsigned int cItem;
        float cScrollAnimation;
        float cTransitionAnimation;
        
        IAssetMenuItem* getMenuItem() const;
        
        // Input functions.
        void moveCursorUp();
        void moveCursorDown();

        public:
        Selection(const AssetMenu* const parent, int item, float transitionStart);
        
        unsigned int getItemCount() const override;
        unsigned int getIndex() const override;
        float getScroll() const override;
        bool input(sf::Event& event) override;
        void updateEditing(unsigned int milliseconds, bool active) override;
        bool updateForClosure(unsigned int milliseconds) override;
        void renderEditing(IPropertyAppearance* appearance) const override;
        void render(IPropertyAppearance* appearance) const override;
        float getSelectedWidth(IPropertyAppearance* appearance) const override;
        float getMaxWidth(IPropertyAppearance* appearance) const override;
      };
    
      AssetMenu(PropertyAsset* parent, std::vector<std::pair<std::string, std::string>> ids, const std::string& prefix);
      
      void refresh(std::vector<std::pair<std::string, std::string>> ids, const std::string& prefix);
      void setItems(std::vector<std::pair<std::string, std::string>> ids);
      unsigned int getItemCount() const;
      void renderEditing(IPropertyAppearance* appearance, float yOffset) const;
      float getMaxWidth(IPropertyAppearance* appearance) const;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id) const;
    };

    class ISpecialItemEditor {
      public:
      virtual bool hasOwnPage() = 0;
      virtual float getWidth(IPropertyAppearance* appearance) = 0;
      virtual float getHeight(IPropertyAppearance* appearance) = 0;
      virtual void render(IPropertyAppearance* appearance) = 0;
      virtual void update(unsigned int milliseconds) = 0;
      virtual bool input(sf::Event& event) = 0;
      virtual std::string getValue() = 0;

      virtual ~ISpecialItemEditor() {
      }
    };
    
    
    AssetMenu cAvailable;
    std::vector<IAssetMenuItem*> cGlobalMenuItems;
    std::vector<std::unique_ptr<ISelection>> cSelection;
    std::vector<std::unique_ptr<ISelection>> cRuntimeSelectionClosed;
    float cRightSelectionTarget;
    AnimatedFloat cWidth;
    AnimatedFloat cHeight;
    AnimatedFloat cSelectionWidth;
    AnimatedFloat cScroll;
    std::unique_ptr<ISpecialItemEditor> cSpecialEditor;

    std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id) const;
    void updateAnimationValues();
    void moveCursorLeft();

    // External interfaces.
    IPropertyAppearance* cIntAppearance; /// Stored because appearance is used outside of rendering functions.

    PropertyAsset(IPropertyAppearance* appearance, const std::string& label, std::vector<std::pair<std::string, std::string>> items, std::vector<IAssetMenuItem*> globalMenuItems, std::function<bool(const std::string& item)> confirmationCallback);
    void setSelection(const std::string& item);
    
    void refreshItems(std::vector<std::pair<std::string, std::string>> items);
    
    /************************\
     * Implements IProperty *
    \************************/
    bool updateEditing(unsigned int milliseconds) override;
    void render(IPropertyAppearance* appearance) const override;
    void renderEditing(IPropertyAppearance* appearance) const override;
    IEditable* edit(IAssetRegistry* assets) override;
    void close() override;
    float getWidth(IPropertyAppearance* appearance) const override;
    bool input(sf::Event& event) override;
    
    /*****************************************\
     * Implements ISpecialItemEditorListener *
    \*****************************************/
    void setSpecialItemSelected(int item) override;

    protected:
    std::function<bool(const std::string& item)> cConfirmationCallback;
    
    private:
    
    virtual bool renderIcon(IPropertyAppearance* appearance, const std::string& id) const = 0;
    
    // Internal classes.
    class AssetMenuItemFolder : public IAssetMenuItem {
      private:
      PropertyAsset* cParent;
      std::unique_ptr<AssetMenu> cAssetMenu;
      std::string cLabel;
      
      public:
      AssetMenuItemFolder(PropertyAsset* parent, std::unique_ptr<AssetMenu> assetMenu, const std::string& label);
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(IPropertyAppearance* appearance, float yOffset) const override;
      bool executeAction() override;
      float getWidth(IPropertyAppearance* appearance) const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    };
    
    class AssetMenuItemAsset : public IAssetMenuItem {
      private:
      PropertyAsset* cParent;
      std::string cAssetID;
      std::string cLabel;
      
      public:
      AssetMenuItemAsset(PropertyAsset* parent, const std::string assetID, const std::string& label);
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(IPropertyAppearance* appearance, float yOffset) const override;
      bool executeAction() override;
      float getWidth(IPropertyAppearance* appearance) const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    };
  };
}

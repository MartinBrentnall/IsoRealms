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
#include <GL/glew.h>

#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

#include "PropertyAssetBase.h"
#include "PropertyColourChannel.h"
#include "PropertyColourHue.h"
#include "PropertyColourLightness.h"
#include "PropertyColourSaturation.h"

namespace IsoRealms {
  class PropertyColour : public PropertyAssetBase<IColour> {
    private:
      
    // Property names.
    static const std::string PROPERTY_RED;
    static const std::string PROPERTY_GREEN;
    static const std::string PROPERTY_BLUE;
    static const std::string PROPERTY_ALPHA;
    static const std::string PROPERTY_HUE;
    static const std::string PROPERTY_SATURATION;
    static const std::string PROPERTY_LIGHTNESS;
    
    IAssetBrowser* cBrowser;
    
    class FixedColour : public IAssetMenuItem {
      private:
      PropertyColour* cParent;
      
      public:
      FixedColour(PropertyColour* parent);
        
      /*****************************\
       * Implements IAssetMenuItem *
      \*****************************/
      void render(float yOffset) const override;
      bool executeAction() override;
      float getWidth() const override;
      std::vector<std::unique_ptr<ISelection>> getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const override;
    };
    
    class Editor : public ISpecialItemEditor {
      private:
      PropertyColour* cParent;
      float cDefRed;
      float cDefGreen;
      float cDefBlue;
      float cDefAlpha;
      float cLastKnownHue;
      float cLastKnownSaturation;
      float cLastKnownLightness;
      std::vector<std::unique_ptr<IProperty>> cProperties;
      int cSelectedProperty;
      AnimatedFloat cScroll;
      ISpecialItemEditorListener* cMenuCallback;
      
      public:
      Editor(PropertyColour* parent, ISpecialItemEditorListener* listener);
        
      float getMaxLabelWidth() const;
      
      /*********************************\
       * Implements ISpecialItemEditor *
      \*********************************/
      bool hasOwnPage() override;
      float getWidth(IPropertyAppearance* appearance) override;
      float getHeight(IPropertyAppearance* appearance) override;
      void render(IPropertyAppearance* appearance) override;
      void update(unsigned int milliseconds) override;
      bool input(sf::Event& event) override;
      std::string getValue() override;
    };

    GlobalOption cOptionNone;
    FixedColour cOptionFixed;
    
    public:
    PropertyColour(const std::string& label, IColour** selected, IAssetUser<IColour>* client, IAssetBrowser* browser, bool allowNone = false);

    void refresh(); // FIXME:IsoRealmsEditor: Shouldn't be necessary.  Refresh should happen automatically.
    
    /*************************************\
     * Implements PropertyAssetBase<IColour> *
    \*************************************/
    bool renderIcon(IAssetBrowser* browser, const std::string& id) const override;
    IColour* getAsset(IAssetBrowser* browser, IAssetUser<IColour>* client, const std::string& id) const override;
  };
}

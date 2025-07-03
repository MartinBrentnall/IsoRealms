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

#include <vector>

#include "IAssets.h"
#include "IAssetIdentifier.h"
#include "IAssetRemover.h"

namespace IsoRealms {
  class IModel;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFloat;
  class IFont;
  class IInteger;
  template <class OWNER, class TYPE> class IAssetListener;
  class IString;
  class ITexture;
  class IVertex;
  class Project;

  class IAssetBrowser : public IAssetIdentifier,
                        public IAssetRemover,
                        public IAssets {
    public:
    virtual std::vector<std::string> getAllActions()       = 0;
    virtual std::vector<std::string> getAllBindings()      = 0;
    virtual std::vector<std::string> getAllBindingTypes()  = 0;
    virtual std::vector<std::string> getAllBooleans()      = 0;
    virtual std::vector<std::string> getAllColours()       = 0;
    virtual std::vector<std::string> getAllFloats()        = 0;
    virtual std::vector<std::string> getAllFonts()         = 0;
    virtual std::vector<std::string> getAllEditables()     = 0;
    virtual std::vector<std::string> getAllInputHandlers() = 0;
    virtual std::vector<std::string> getAllIntegers()      = 0;
    virtual std::vector<std::string> getAllModels()        = 0;
    virtual std::vector<std::string> getAllScreens()       = 0;
    virtual std::vector<std::string> getAllStrings()       = 0;
    virtual std::vector<std::string> getAllTextures()      = 0;
    virtual std::vector<std::string> getAllVertices()      = 0;
    
    virtual void addListener(IAssetListener<IResourceData, IBoolean>*)     = 0;
    virtual void addListener(IAssetListener<IResourceData, IColour>*)      = 0;
    virtual void addListener(IAssetListener<IResourceData, IEditable>*)    = 0;
    virtual void addListener(IAssetListener<IResourceData, IFloat>*)       = 0;
    virtual void addListener(IAssetListener<IResourceData, IFont>*)        = 0;
    virtual void addListener(IAssetListener<IResourceData, IInteger>*)     = 0;
    virtual void addListener(IAssetListener<IResourceData, IModel>*)       = 0;
    virtual void addListener(IAssetListener<IResourceData, IScreen>*)      = 0;
    virtual void addListener(IAssetListener<IResourceData, IString>*)      = 0;
    virtual void addListener(IAssetListener<IResourceData, ITexture>*)     = 0;
    virtual void addListener(IAssetListener<IResourceData, IVertex>*)      = 0;
    
    virtual bool renderActionIcon(      const std::string& id) const = 0;
    virtual bool renderBindingIcon(     const std::string& id) const = 0;
    virtual bool renderBindingTypeIcon( const std::string& id) const = 0;
    virtual bool renderBooleanIcon(     const std::string& id) const = 0;
    virtual bool renderColourIcon(      const std::string& id) const = 0;
    virtual bool renderFloatIcon(       const std::string& id) const = 0;
    virtual bool renderFontIcon(        const std::string& id) const = 0;
    virtual bool renderInputHandlerIcon(const std::string& id) const = 0;
    virtual bool renderIntegerIcon(     const std::string& id) const = 0;
    virtual bool renderModelIcon(       const std::string& id) const = 0;
    virtual bool renderScreenIcon(      const std::string& id) const = 0;
    virtual bool renderStringIcon(      const std::string& id) const = 0;
    virtual bool renderTextureIcon(     const std::string& id) const = 0;
    virtual bool renderVertexIcon(      const std::string& id) const = 0;
    
    virtual bool isActionConfigurable(      const std::string& id) const = 0;
    virtual bool isBindingConfigurable(     const std::string& id) const = 0;
    virtual bool isBindingTypeConfigurable( const std::string& id) const = 0;
    virtual bool isBooleanConfigurable(     const std::string& id) const = 0;
    virtual bool isColourConfigurable(      const std::string& id) const = 0;
    virtual bool isFloatConfigurable(       const std::string& id) const = 0;
    virtual bool isFontConfigurable(        const std::string& id) const = 0;
    virtual bool isInputHandlerConfigurable(const std::string& id) const = 0;
    virtual bool isIntegerConfigurable(     const std::string& id) const = 0;
    virtual bool isModelConfigurable(       const std::string& id) const = 0;
    virtual bool isScreenConfigurable(      const std::string& id) const = 0;
    virtual bool isStringConfigurable(      const std::string& id) const = 0;
    virtual bool isTextureConfigurable(     const std::string& id) const = 0;
    virtual bool isVertexConfigurable(      const std::string& id) const = 0;
    
    virtual IProject& getProject() = 0;
  };
}

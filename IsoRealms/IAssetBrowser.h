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
  class I3DModelType;
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
    virtual std::vector<std::pair<std::string, std::string>> getAllModelTypes()    = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllActionTypes()   = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllBooleans()      = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllColours()       = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllFloats()        = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllEditables()     = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllFonts()         = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllInputHandlers() = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllIntegers()      = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllScreens()       = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllStrings()       = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllTextures()      = 0;
    virtual std::vector<std::pair<std::string, std::string>> getAllVertices()      = 0;
    
    virtual void addListener(IAssetListener<Project, I3DModelType>*) = 0;
    virtual void addListener(IAssetListener<Project, IBoolean>*)     = 0;
    virtual void addListener(IAssetListener<Project, IColour>*)      = 0;
    virtual void addListener(IAssetListener<Project, IEditable>*)    = 0;
    virtual void addListener(IAssetListener<Project, IFloat>*)       = 0;
    virtual void addListener(IAssetListener<Project, IFont>*)        = 0;
    virtual void addListener(IAssetListener<Project, IInteger>*)     = 0;
    virtual void addListener(IAssetListener<Project, IScreen>*)      = 0;
    virtual void addListener(IAssetListener<Project, IString>*)      = 0;
    virtual void addListener(IAssetListener<Project, ITexture>*)     = 0;
    virtual void addListener(IAssetListener<Project, IVertex>*)      = 0;
    
    virtual bool renderActionIcon(      const std::string& id) const = 0;
    virtual bool renderBooleanIcon(     const std::string& id) const = 0;
    virtual bool renderColourIcon(      const std::string& id) const = 0;
    virtual bool renderFloatIcon(       const std::string& id) const = 0;
    virtual bool renderInputHandlerIcon(const std::string& id) const = 0;
    virtual bool renderIntegerIcon(     const std::string& id) const = 0;
    virtual bool renderModelIcon(       const std::string& id) const = 0;
    virtual bool renderScreenIcon(      const std::string& id) const = 0;
    virtual bool renderTextureIcon(     const std::string& id) const = 0;
    virtual bool renderVertexIcon(      const std::string& id) const = 0;
    
    virtual IProject* getProject() = 0;
  };
}

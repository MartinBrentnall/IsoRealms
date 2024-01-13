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

#include <SFML/Graphics/Texture.hpp>

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a texture loaded from an image file.  File type
   * support is provided by SFML.
   */
  class FileTexture final : public ITexture {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    FileTexture(IProject* project, Basics* basics);
    FileTexture(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***********************\
     * Implements ITexture *
    \***********************/
    void set() const override;
    void hintTextureInUse(bool inUse) override;
    ITexture* getTexture() override;
    void coord(float x, float y) const override;

    ~FileTexture();

    private:

    // DOM strings.
    static const std::string ATTRIBUTE_FILE;

    // Definition data.
    std::string cDefFile; /// Filename containing the image create a texture from.

    // Runtime data.
    sf::Texture cRuntimeTexture;
//    GLuint cRuntimeTextureID; /// Internal ID of this texture.

    /**********************\
     * Internal Functions *
    \**********************/
    void reloadData(IProject* assets);
  };
}

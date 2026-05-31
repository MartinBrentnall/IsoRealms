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

#include <SFML/Graphics/Texture.hpp>

#include "IsoRealms.h"

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
    FileTexture(Basics& basics, IResourceData& data);
    FileTexture(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /***********************\
     * Implements ITexture *
    \***********************/
    void set() const override;
    void hintTextureInUse(bool inUse) override;
    ITexture* getTexture() override;
    void coord(float x, float y) const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    ~FileTexture();

    private:

    // JSON members.
    inline static const std::string JSON_FILENAME = "filename";

    // Definition data.
    File cDefFile; /// Filename containing the image create a texture from.

    // Runtime data.
    sf::Texture cRuntimeTexture;
//    GLuint cRuntimeTextureID; /// Internal ID of this texture.

    /**********************\
     * Internal Functions *
    \**********************/
    void reloadData(Project& assets);
  };
}

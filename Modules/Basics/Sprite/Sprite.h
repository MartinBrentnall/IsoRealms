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

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a model composed of a quad with a specified
   * texture applied to it.  The model can be configured to behave in various
   * ways according to the view within a 3D environment.
   */
  class Sprite final : public IModel,
                       public IModelInstance,
                       public IScreenListener {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Sprite(Basics& basics, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IModelInstance *
    \*****************************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    /******************************\
     * Implements IScreenListener *
    \******************************/
    void screenAdded(const IScreen* screen) override;
    void screenRemoved(const IScreen* screen) override;
    void screenPreRender(const IScreen* screen) override;
    void screenPostRender(const IScreen* screen) override;

    ~Sprite();

    private:
    
    // System.
    Project& cDefProject;   /// Hosting project.

    // Definition data.
    Texture cDefTexture;     /// Texture applied to this sprite.
    bool cDefBillboardYaw;   /// Rotate the sprite according to the yaw of the current view.
    bool cDefBillboardPitch; /// Tilt the sprite according to the pitch of the current view.

    // Runtime data.
    const IScreen* cRuntimeScreen; /// Screen in which the sprite is being rendered.

    /**********************\
     * Internal Functions *
    \**********************/
    float getAngle() const;
    float getTilt() const;
    void render(float size) const;
  };
}

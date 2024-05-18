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

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a 3D model composed of a quad with a specified
   * texture applied to it.  The model can be configured to behave in various
   * ways according to the view within a 3D environment.
   */
  class Sprite final : public I3DModelType,
                       public I3DModel,
                       public IScreenListener {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Sprite(IProject* project, Basics* basics);
    Sprite(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(JSONObject object, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***************************\
     * Implements I3DModelType *
    \***************************/
    I3DModel* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;

    /***********************\
     * Implements I3DModel *
    \***********************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    /******************************\
     * Implements IScreenListener *
    \******************************/
    void screenAdded(IProject* project, const IScreen* screen) override;
    void screenRemoved(const IScreen* screen) override;
    void screenPreRender(const IScreen* screen) override;
    void screenPostRender(const IScreen* screen) override;

    ~Sprite();

    private:

    // JSON members.
    static const std::string JSON_BILLBOARD_PITCH;
    static const std::string JSON_BILLBOARD_YAW;
    static const std::string JSON_TEXTURE;

    // System.
    IProject* cDefProject;   /// Hosting project.

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

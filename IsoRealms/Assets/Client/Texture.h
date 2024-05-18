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

#include <functional>

#include "IsoRealms/Assets/Type/ITexture.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  class Texture : public IAssetUser<ITexture> {
    private:
    static const std::string JSON_ANGLE;
    static const std::string JSON_SCALE_X;
    static const std::string JSON_SCALE_Y;

    IProject* cProject;
    ITexture* cTexture;
    float cDefScaleX;
    float cDefScaleY;
    float cDefAngle;

    Texture(Texture const& texture) = delete;
    Texture& operator=(Texture const& texture) = delete;

    public:
    Texture(IProject* project);

    void init(JSONObject object, const std::string& member);
    void set(JSONObject object, const std::string& member);
    void save(JSONObject object, const std::string& name) const;

    void coord(float x, float y) const;

    ITexture* operator->() const {
      return cTexture;
    }

    ITexture* operator*() const {
      return cTexture;
    }

    /***********************************\
     * Implements IAssetUser<ITexture> *
    \***********************************/
    void relinquish(ITexture* asset) override;

    virtual ~Texture();
  };
}

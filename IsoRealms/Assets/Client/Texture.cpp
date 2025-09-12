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
#include "Texture.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/IPropertyManager.h"
#include "IsoRealms/Editing/Property/PropertyNativeFloat.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  Texture::Texture(IResourceData& owner, std::function<void()> listener) :
            Asset<Texture, ITexture, IResourceData>(owner),
            cListener(listener),
            cDefScaleX(1.0f),
            cDefScaleY(1.0f),
            cDefAngle(0.0f) {
  }

  void Texture::coord(float x, float y) const {
    float mDistance = static_cast<float>(Utils::distance(0.0, 0.0, x, y));
    double mAngle = std::atan2(y, x) + cDefAngle * (M_PI / 180.0f);
    float mX = static_cast<float>(std::cos(mAngle) * mDistance);
    float mY = static_cast<float>(std::sin(mAngle) * mDistance);
    cAsset->coord(mX / cDefScaleX, mY / cDefScaleY);
  }

  bool Texture::hasClientConfiguration() const {
    return true;
  }
  
  bool Texture::isDefaultConfiguration() const {
    return cDefScaleX == 1.0f && cDefScaleY == 1.0f && cDefAngle == 0.0f;
  }

  void Texture::loadClientConfiguration(JSONObject object) {
    cDefScaleX = object.getFloat(JSON_SCALE_X, 1.0f);
    cDefScaleY = object.getFloat(JSON_SCALE_Y, 1.0f);
    cDefAngle  = object.getFloat(JSON_ANGLE);
  }

  void Texture::saveClientConfiguration(JSONObject object) const {
    object.addFloat(JSON_ANGLE,   cDefAngle);
    object.addFloat(JSON_SCALE_X, cDefScaleX, 1.0f);
    object.addFloat(JSON_SCALE_Y, cDefScaleY, 1.0f);
  }
  
  void Texture::getClientProperties(PropertyMaker& owner) {
    const Metadata& mMetadata = cManager.getProject().getApplication().getMetadata("Texture");
    owner.createPropertyNativeFloat(mMetadata.getPropertyData("ScaleX"), [this]() {return cDefScaleX;}, [this](float value) {cDefScaleX = value; stateChanged(cAsset);});
    owner.createPropertyNativeFloat(mMetadata.getPropertyData("ScaleY"), [this]() {return cDefScaleY;}, [this](float value) {cDefScaleY = value; stateChanged(cAsset);});
    owner.createPropertyNativeFloat(mMetadata.getPropertyData("Angle"),  [this]() {return cDefAngle;},  [this](float value) {cDefAngle  = value; stateChanged(cAsset);});
  }
  
  void Texture::stateChanged(ITexture* asset) {
    if (asset == cAsset && cListener != nullptr) {
      cListener();
    }
  }

  const std::string Texture::JSON_ANGLE   = "angle";
  const std::string Texture::JSON_SCALE_X = "scaleX";
  const std::string Texture::JSON_SCALE_Y = "scaleY";
}

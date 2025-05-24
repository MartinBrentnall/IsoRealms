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
#include "Texture.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Editing/Property/PropertyNativeFloat.h"

namespace IsoRealms {
  Texture::Texture(IProject& project, std::function<void()> listener) :
            Asset<ITexture, IProject>(project, project.createLiteralTexture(this)),
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

  ITexture* Texture::createLiteralAsset(IProject& project) {
    return project.createLiteralTexture(this);
  }
  
  ITexture* Texture::getAsset(IProject& project, JSONObject object) {
    return project.getTexture(this, object, cListener != nullptr ? this : nullptr);
  }
  
  ITexture* Texture::getAsset(IProject& project, const std::string& id) {
    return project.getTexture(this, id, cListener != nullptr ? this : nullptr);
  }
  
  std::vector<std::string> Texture::getAvailableProviders() const {
    return cManager.getAllTextures();
  }  

  bool Texture::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderTextureIcon(id);
  }

  bool Texture::hasConfiguration() const {
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
  
  std::vector<std::unique_ptr<IProperty>> Texture::getClientProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Scale X", [this]() {return cDefScaleX;}, [this](float value) {cDefScaleX = value; stateChanged(cAsset); return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Scale Y", [this]() {return cDefScaleY;}, [this](float value) {cDefScaleY = value; stateChanged(cAsset); return true;}));
    mProperties.emplace_back(std::make_unique<PropertyNativeFloat>("Angle",   [this]() {return cDefAngle;},  [this](float value) {cDefAngle  = value; stateChanged(cAsset); return true;}));
    return mProperties;
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

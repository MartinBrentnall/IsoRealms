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

namespace IsoRealms {
  const std::string Texture::JSON_ANGLE   = "angle";
  const std::string Texture::JSON_SCALE_X = "scaleX";
  const std::string Texture::JSON_SCALE_Y = "scaleY";

  Texture::Texture(IProject* project) :
            cProject(project),
            cTexture(cProject->createLiteralTexture(this)),
            cDefScaleX(1.0f),
            cDefScaleY(1.0f),
            cDefAngle(0.0f) {
  }

  void Texture::init(JSONObject object, const std::string& member) {
    cProject->init([this, object, member](IAssets* assets) {
      set(object, member);
    });
  }

  void Texture::set(JSONObject object, const std::string& member) {
    JSONObject mAssetObject = object.getObject(member);
    cProject->release(this, cTexture);
    cDefScaleX = mAssetObject.getFloat(JSON_SCALE_X, 1.0f);
    cDefScaleY = mAssetObject.getFloat(JSON_SCALE_Y, 1.0f);
    cDefAngle = mAssetObject.getFloat(JSON_ANGLE);
    cTexture = cProject->getTexture(this, mAssetObject);
  }

  void Texture::save(JSONObject object, const std::string& name) const {
    JSONObject mAssetObject = object.addObject(name);
    mAssetObject.addFloat(JSON_ANGLE,   cDefAngle);
    mAssetObject.addFloat(JSON_SCALE_X, cDefScaleX, 1.0f);
    mAssetObject.addFloat(JSON_SCALE_Y, cDefScaleY, 1.0f);
    cProject->save(mAssetObject, cTexture);
  }

  void Texture::coord(float x, float y) const {
    float mDistance = static_cast<float>(Utils::distance(0.0, 0.0, x, y));
    double mAngle = std::atan2(y, x) + cDefAngle * (M_PI / 180.0f);
    float mX = static_cast<float>(std::cos(mAngle) * mDistance);
    float mY = static_cast<float>(std::sin(mAngle) * mDistance);
    cTexture->coord(mX / cDefScaleX, mY / cDefScaleY);
  }

  void Texture::relinquish(ITexture* asset) {
    if (cTexture == asset) {
      cTexture = cProject->createLiteralTexture(this);
    }
  }

  Texture::~Texture() {
    if (cTexture != nullptr) {
      cProject->release(this, cTexture);
    }
  }
}

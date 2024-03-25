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
  const std::string Texture::ATTRIBUTE_ANGLE   = "angle";
  const std::string Texture::ATTRIBUTE_SCALE_X = "scaleX";
  const std::string Texture::ATTRIBUTE_SCALE_Y = "scaleY";
  const std::string Texture::ATTRIBUTE_TEXTURE = "texture";

  Texture::Texture(IProject* project) :
            cProject(project),
            cTexture(cProject->createLiteralTexture(this)),
            cDefScaleX(1.0f),
            cDefScaleY(1.0f),
            cDefAngle(0.0f) {
  }

  void Texture::init(DOMNode& node) {
    cDefScaleX = node.getFloatAttribute(ATTRIBUTE_SCALE_X, 1.0f);
    cDefScaleY = node.getFloatAttribute(ATTRIBUTE_SCALE_Y, 1.0f);
    cDefAngle  = node.getFloatAttribute(ATTRIBUTE_ANGLE);
    cProject->init([this, &node](IAssets* assets) {
      set(node);
    });
  }

  void Texture::save(DOMNodeWriter* node, const std::string& tag) const {
    DOMNodeWriter mTextureNode = node->addBranch(tag);
    cProject->save(&mTextureNode, cTexture);
    mTextureNode.addAttribute(ATTRIBUTE_ANGLE,   cDefAngle);
    mTextureNode.addAttribute(ATTRIBUTE_SCALE_X, cDefScaleX);
    mTextureNode.addAttribute(ATTRIBUTE_SCALE_Y, cDefScaleY);
  }

  void Texture::set(DOMNode& node) {
    cProject->release(this, cTexture);
    cTexture = cProject->getTexture(this, node);
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

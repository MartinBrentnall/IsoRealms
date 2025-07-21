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
#include "Sprite.h"

namespace IsoRealms::Basics {
  const std::string Sprite::JSON_BILLBOARD_PITCH = "billboardPitch";
  const std::string Sprite::JSON_BILLBOARD_YAW   = "billboardYaw";
  const std::string Sprite::JSON_TEXTURE         = "texture";

  Sprite::Sprite(IProject& project, Basics& basics, IResourceData& data) :
            cDefProject(project),
            cDefTexture(data),
            cDefBillboardYaw(false),
            cDefBillboardPitch(false),
            cRuntimeScreen(nullptr) {
    cDefProject.addScreenListener(this);
  }
  
  Sprite::Sprite(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Sprite(project, basics, data) {
    cDefTexture.init(object, JSON_TEXTURE);
    cDefBillboardYaw   = object.getBoolean(JSON_BILLBOARD_YAW);
    cDefBillboardPitch = object.getBoolean(JSON_BILLBOARD_PITCH);
  }

  void Sprite::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IModel>(this, "", "Sprite Models");
  }

  void Sprite::save(JSONObject object) const {
    cDefTexture.save(object, JSON_TEXTURE);
    object.addBoolean(JSON_BILLBOARD_YAW, cDefBillboardYaw);
    object.addBoolean(JSON_BILLBOARD_PITCH, cDefBillboardPitch);
  }

  void Sprite::hintInUse(bool inUse) {
    cDefTexture->hintTextureInUse(inUse);
  }

  bool Sprite::renderIcon() const {
    render(1.0f);
    return true;
  }

  void Sprite::render(float size) const {
    cDefTexture->set();
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    cDefTexture.coord(1.0f, 1.0f); glVertex3f( size, -size, 0.0f);
    cDefTexture.coord(1.0f, 0.0f); glVertex3f( size,  size, 0.0f);
    cDefTexture.coord(0.0f, 0.0f); glVertex3f(-size,  size, 0.0f);
    cDefTexture.coord(0.0f, 1.0f); glVertex3f(-size, -size, 0.0f);
    glEnd();
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  std::vector<std::unique_ptr<IProperty>> Sprite::getProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Texture>>(owner, owner.getPropertyData("Appearance"),      cDefTexture));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>( owner, owner.getPropertyData("BillboardYaw"),   [this]() {return cDefBillboardYaw;},   [this](bool value) {cDefBillboardYaw   = value;}, owner.getProject()));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>( owner, owner.getPropertyData("BillboardPitch"), [this]() {return cDefBillboardPitch;}, [this](bool value) {cDefBillboardPitch = value;}, owner.getProject()));
    return mProperties;
  }

  IModelInstance* Sprite::createModel() {
    return this;
  }

  bool Sprite::renderPreview() const {
    glRotatef(-getAngle(), 0.0f, 0.0f, 1.0f);
    glRotatef(-getTilt(), 1.0f, 0.0f, 0.0f);
    render(0.5f);
    return true;
  }

  bool Sprite::renderAssetIcon() const {
    render(1.0f);
    return true;
  }

  void Sprite::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Sprite::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Sprite::isDefaultConfiguration() const {
    return true;
  }

  void Sprite::update(unsigned int milliseconds) {
    // Nothing to do.
  }

  void Sprite::render() const {
    glRotatef(-getAngle(), 0.0f, 0.0f, 1.0f);
    glRotatef(-getTilt(), 1.0f, 0.0f, 0.0f);
    render(0.5f);
  }

  void Sprite::screenAdded(IProject& project, const IScreen* screen) {
    // Nothing to do.
  }

  void Sprite::screenRemoved(const IScreen* screen) {
    // Nothing to do.
  }

  void Sprite::screenPreRender(const IScreen* screen) {
    cRuntimeScreen = screen;
  }

  void Sprite::screenPostRender(const IScreen* screen) {
    cRuntimeScreen = nullptr;
  }

  float Sprite::getAngle() const {
    if (cDefBillboardYaw) {
      const IFloat* mYaw = cRuntimeScreen->getYaw();
      return mYaw != nullptr ? mYaw->getValue() : 0.0f;
    }
    return 0.0f;
  }

  float Sprite::getTilt() const {
    if (cDefBillboardPitch) {
      const IFloat* mPitch = cRuntimeScreen->getPitch();
      return mPitch != nullptr ? mPitch->getValue() : 0.0f;
    }
    return 0.0f;
  }

  Sprite::~Sprite() {
    cDefProject.removeScreenListener(this);
  }
}

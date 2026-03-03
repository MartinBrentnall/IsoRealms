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
#include "FileTexture.h"

namespace IsoRealms::Basics {
  const std::string FileTexture::JSON_FILENAME = "filename";

  FileTexture::FileTexture(Basics& basics, IResourceData& data) :
            cDefFile(data.getProject()) {
  }

  FileTexture::FileTexture(Basics& basics, IResourceData& data, JSONObject object) :
            FileTexture(basics, data) {
    cDefFile.load(JSON_FILENAME, object);
    reloadData(data.getProject());
  }

  void FileTexture::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<ITexture>(this, "", "Textures from Image Files"); // TODO: Localize this.
  }
  
  void FileTexture::save(JSONObject object) const {
    cDefFile.save(JSON_FILENAME, object);
  }

  void FileTexture::hintInUse(bool inUse) {
    // Nothing to do
  }

  bool FileTexture::renderIcon() const {
    set();
    glEnable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
    glEnd();
    return true;
  }

  void FileTexture::getProperties(PropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector<File>(metadata.getPropertyData("File"), cDefFile);
  }

  void FileTexture::removed() {
    // Nothing to do.
  }

  void FileTexture::set() const {
    glGetError(); // TODO: Suppresses SFML picking up an error from somewhere else.  Need to find where other error comes from.
    sf::Texture::bind(&cRuntimeTexture);
  }

  void FileTexture::hintTextureInUse(bool inUse) {
    // Nothing to do
  }

  ITexture* FileTexture::getTexture() {
    return this;
  }

  void FileTexture::coord(float x, float y) const {
    glTexCoord2f(x, y);
  }

  void FileTexture::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void FileTexture::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool FileTexture::isDefaultConfiguration() const {
    return true;
  }

  void FileTexture::reloadData(Project& project) {
    project.getApplication().mainThreadInit([this]() {
      std::string mFullPath = cDefFile.getPath();
      cRuntimeTexture.loadFromFile(mFullPath);
    });
  }

  FileTexture::~FileTexture() {
  }
}

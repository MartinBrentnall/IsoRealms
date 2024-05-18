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
#include "FileTexture.h"

namespace IsoRealms::Basics {
  const std::string FileTexture::JSON_FILENAME = "filename";

  FileTexture::FileTexture(IProject* project, Basics* basics) {
  }

  FileTexture::FileTexture(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            FileTexture(project, basics) {
    cDefFile = object.getString(JSON_FILENAME);
    reloadData(project);
  }

  void FileTexture::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Stored Textures");
  }
  
  void FileTexture::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }

  void FileTexture::save(JSONObject object, IAssetIdentifier* identifier) const {
    object.addString(JSON_FILENAME, cDefFile);
  }

  void FileTexture::hintInUse(bool inUse) {
    // Nothing to do
  }

  bool FileTexture::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> FileTexture::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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

  void FileTexture::reloadData(IProject* project) {
    project->mainThreadInit([this]() {
      std::string mFullPath = System::getPath(cDefFile, false);
      cRuntimeTexture.loadFromFile(mFullPath);
    });
  }

  FileTexture::~FileTexture() {
  }
}

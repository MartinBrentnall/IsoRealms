/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ResourceSoundFile.h"

ResourceSoundFile::ResourceSoundFile(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

ResourceSoundFile::ResourceSoundFile(const std::string& filename, const std::string& name) {
  cName = name;
  loadData(filename);
}

void ResourceSoundFile::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cName = node->getAttribute("name");
  cFile = node->getAttribute("file");
  loadData(cFile);
}

void ResourceSoundFile::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources) {
  node->addAttribute("file", cFile);
}

void ResourceSoundFile::loadData(const std::string& filename) {
  std::string mResource = System::getProgramResource(filename);
  cSoundData = Mix_LoadWAV(mResource.c_str());
  if (cSoundData == NULL) {
    std::cout << "WARNING: Unable to load file \"" << mResource << "\": " << Mix_GetError() <<  std::endl;
  }
}

std::string ResourceSoundFile::getName() {
  return cName;
}

void ResourceSoundFile::play() {
  if (Mix_PlayChannel(-1, cSoundData, 0) == -1) {
    std::cout << "WARNING: Unable to play file: " << Mix_GetError() << std::endl;
  }
}

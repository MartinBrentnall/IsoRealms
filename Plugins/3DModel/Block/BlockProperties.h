/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef BLOCK_PROPERTIES_H
#define BLOCK_PROPERTIES_H

#include <string>

#include "../../TextureSet/ITextureSet.h"

// TODO: This class is not required during gameplay
class BlockProperties {
  private:
  ITextureSet** cTextureSet;
  std::string cTopSurfaceTexture;
  std::string cBottomSurfaceTexture;
  std::string cWestSurfaceTextureTop;
  std::string cEastSurfaceTextureTop;
  std::string cSouthSurfaceTextureTop;
  std::string cNorthSurfaceTextureTop;
  std::string cWestSurfaceTextureBottom;
  std::string cEastSurfaceTextureBottom;
  std::string cSouthSurfaceTextureBottom;
  std::string cNorthSurfaceTextureBottom;
  
  public:
  BlockProperties(ITextureSet**);
    
  ITexture* getTopSurfaceTexture();
  ITexture* getBottomSurfaceTexture();
  ITexture* getNorthSurfaceTextureTop();
  ITexture* getEastSurfaceTextureTop();
  ITexture* getSouthSurfaceTextureTop();
  ITexture* getWestSurfaceTextureTop();
  ITexture* getNorthSurfaceTextureBottom();
  ITexture* getEastSurfaceTextureBottom();
  ITexture* getSouthSurfaceTextureBottom();
  ITexture* getWestSurfaceTextureBottom();
  
  void setTopSurfaceTexture(const std::string&);
  void setBottomSurfaceTexture(const std::string&);
  void setNorthSurfaceTextureTop(const std::string&);
  void setEastSurfaceTextureTop(const std::string&);
  void setSouthSurfaceTextureTop(const std::string&);
  void setWestSurfaceTextureTop(const std::string&);
  void setNorthSurfaceTextureBottom(const std::string&);
  void setEastSurfaceTextureBottom(const std::string&);
  void setSouthSurfaceTextureBottom(const std::string&);
  void setWestSurfaceTextureBottom(const std::string&);

  void saveProperty(DOMNodeWriter*, const std::string&, const std::string&);
  void save(DOMNodeWriter*);
};

#endif

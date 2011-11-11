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

#include <IsoRealms/ITexture.h>

// TODO: This class is not required during gameplay
class BlockProperties {
  private:
  ITexture* cTopSurfaceTexture;
  ITexture* cBottomSurfaceTexture;
  ITexture* cWestSurfaceTextureTop;
  ITexture* cEastSurfaceTextureTop;
  ITexture* cSouthSurfaceTextureTop;
  ITexture* cNorthSurfaceTextureTop;
  ITexture* cWestSurfaceTextureBottom;
  ITexture* cEastSurfaceTextureBottom;
  ITexture* cSouthSurfaceTextureBottom;
  ITexture* cNorthSurfaceTextureBottom;
  
  public:
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
  
  void setTopSurfaceTexture(ITexture*);
  void setBottomSurfaceTexture(ITexture*);
  void setNorthSurfaceTextureTop(ITexture*);
  void setEastSurfaceTextureTop(ITexture*);
  void setSouthSurfaceTextureTop(ITexture*);
  void setWestSurfaceTextureTop(ITexture*);
  void setNorthSurfaceTextureBottom(ITexture*);
  void setEastSurfaceTextureBottom(ITexture*);
  void setSouthSurfaceTextureBottom(ITexture*);
  void setWestSurfaceTextureBottom(ITexture*);

  void save(DOMNodeWriter*);
};

#endif

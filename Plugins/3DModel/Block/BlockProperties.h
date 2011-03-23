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

#include "../../SpindizzyTextureSet/ISpindizzyTextureSet.h"

class BlockProperties {
  private:
  ISpindizzyTextureSet** cTextureSet;
  std::string cTopSurfaceTexture;
  std::string cBottomSurfaceTexture;
  std::string cWestSurfaceTexture;
  std::string cEastSurfaceTexture;
  std::string cSouthSurfaceTexture;
  std::string cNorthSurfaceTexture;
  
  public:
  BlockProperties(ISpindizzyTextureSet**);
    
  ISpindizzyTexture* getTopSurfaceTexture();
  ISpindizzyTexture* getBottomSurfaceTexture();
  ISpindizzyTexture* getNorthSurfaceTexture();
  ISpindizzyTexture* getEastSurfaceTexture();
  ISpindizzyTexture* getSouthSurfaceTexture();
  ISpindizzyTexture* getWestSurfaceTexture();
  
  void setTopSurfaceTexture(const std::string&);
  void setBottomSurfaceTexture(const std::string&);
  void setNorthSurfaceTexture(const std::string&);
  void setEastSurfaceTexture(const std::string&);
  void setSouthSurfaceTexture(const std::string&);
  void setWestSurfaceTexture(const std::string&);
};

#endif

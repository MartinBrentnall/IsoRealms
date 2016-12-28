/*
 * Copyright 2016 Martin Brentnall
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
#ifndef WALL_PATTERN_TILE_H
#define WALL_PATTERN_TILE_H

#include <GL/glew.h>

#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IWallPattern.h"

class WallPatternTile:public IWallPattern {
  private:
  ITexture* cTexture;
  
  class WallPatternTileSurface:public IVisualElement {
    private:
    WallPatternTile* cParent;
    IWallSurface* cWallSurface;
      
    public:
    WallPatternTileSurface(WallPatternTile*, IWallSurface*);

    /*****************************\
     * Implemetns IVisualElement * 
    \*****************************/
    void render();
    ITexture* getTexture();
    void prepareVisual();
  };
  
  public:
  WallPatternTile(DOMNodeWrapper*, IResourceAccessor*);
  
  void render(IWallSurface*);
  void render(int, int, int, int, int, int, IWallSurface::FaceDirection);

  std::vector<IVisualElement*> getStaticVisuals(IWallSurface*);
  
  /***************************\
   * Implements IWallPattern *
  \***************************/
  bool contains(ITexture*);
  bool resourcePendingDestruction(ITexture*, ITexture*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif

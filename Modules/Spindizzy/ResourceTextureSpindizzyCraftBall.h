/*
 * Copyright 2015 Martin Brentnall
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
#ifndef RESOURCE_TEXTURE_SPINDIZZY_CRAFT_BALL_H
#define RESOURCE_TEXTURE_SPINDIZZY_CRAFT_BALL_H

#include <cmath>

#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/IResourceRegistry.h>
#include <IsoRealms/Resources/Texture/ITexture.h>
#include <IsoRealms/Resources/Texture/Texture.h>

class ResourceTextureSpindizzyCraftBall:public ITexture {
  private:
  static const float CIRCLE_RESOLUTION;
  IColour* cColourOutline;
  IColour* cColourBall;
  IColour* cColourShine;
  Texture* cTexture;

  void updateTexture();
  void renderCircle(float, IColour*);
  
  public:
  ResourceTextureSpindizzyCraftBall(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);

  void save(DOMNodeWriter*, IResourceLocator*);
  void saveCache(DOMNodeWriter*);
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  Icon<ITexture>* getResourceIcon(IResourceBrowser<ITexture>*);

  IColour* getFillColour();
  IColour* getShineColour();
  IColour* getOutlineColour();
  void setFillColour(IColour*);
  void setShineColour(IColour*);
  void setOutlineColour(IColour*);
  
  /***********************\
   * Implements ITexture *
  \***********************/
  void set();
  void save(DOMNodeWriter*);
  
  virtual ~ResourceTextureSpindizzyCraftBall();
};

#endif

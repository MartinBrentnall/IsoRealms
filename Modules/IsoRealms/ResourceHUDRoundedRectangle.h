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
#ifndef RESOURCE_HUD_ROUNDED_RECTANGLE_H
#define RESOURCE_HUD_ROUNDED_RECTANGLE_H

#include <cmath>
#include <GL/glew.h>

#include <IsoRealms/Configuration.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentType.h>
#include <IsoRealms/Resources/HUDComponents/IHUDGameComponent.h>
#include <IsoRealms/Resources/IDummyModule.h>
#include <IsoRealms/Resources/Texture/Texture.h>

class ResourceHUDRoundedRectangle:public IHUDComponentType,
                                  public IHUDGameComponent {
  private:
  static Texture* cCornerTexture;
  static unsigned int cInstanceCount;
  static const float CIRCLE_RESOLUTION;

  float cCornerSize;

  void renderCorner(float, float, float, float, float, float);
  void renderRectangle(float, float, float, float);

  public:
  ResourceHUDRoundedRectangle(IDummyModule*, DOMNodeWrapper*, IResourceRegistry*);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  /********************************\
   * Implements IHUDComponentType *
  \********************************/
  IHUDGameComponent* getHUDComponent();

  /********************************\
   * Implements IHUDGameComponent *
  \********************************/
  void render(float, float);
  void update(unsigned int);
  float getAspectRatio();
  
  void save(DOMNodeWriter*, IResourceLocator*);
  
  virtual ~ResourceHUDRoundedRectangle();  
};

#endif

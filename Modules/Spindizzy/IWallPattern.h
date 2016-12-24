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
#ifndef I_WALL_PATTERN_H
#define I_WALL_PATTERN_H

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceLocator.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IWallSurface.h"

class IWallPattern {
  public:
  virtual bool contains(ITexture*) = 0;
  virtual bool resourcePendingDestruction(ITexture*, ITexture*) = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual std::vector<IVisualElement*> getStaticVisuals(IWallSurface*) = 0;
  virtual void render(IWallSurface*) = 0;
};

#endif

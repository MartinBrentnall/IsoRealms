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
#ifndef I_RESOURCE_SELECTOR_H
#define I_RESOURCE_SELECTOR_H

#include <IsoRealms/Resources/3DModel/I3DModelType.h>
#include <IsoRealms/Resources/Boolean/IBoolean.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/Float/IFloat.h>
#include <IsoRealms/Resources/Integer/IInteger.h>
#include <IsoRealms/Resources/String/IString.h>
#include <IsoRealms/Resources/Texture/ITexture.h>

#include "IResourceSelectionListener.h"
#include "3DModel/I3DModel.h"

class IResourceSelector {
  public:
  virtual void addResourceSelectionListener(IResourceSelectionListener<I3DModelType>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<IBoolean>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<IColour>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<IFloat>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<IInteger>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<IString>*) = 0;
  virtual void addResourceSelectionListener(IResourceSelectionListener<ITexture>*) = 0;
  
  virtual void removeResourceSelectionListener(IResourceSelectionListener<I3DModelType>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IBoolean>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IColour>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IFloat>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IInteger>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<IString>*) = 0;
  virtual void removeResourceSelectionListener(IResourceSelectionListener<ITexture>*) = 0;
  
  virtual void notifyResourceReleased(IColour*) = 0;
  
  virtual void notifyResourceOwned(IColour*) = 0;
};

#endif

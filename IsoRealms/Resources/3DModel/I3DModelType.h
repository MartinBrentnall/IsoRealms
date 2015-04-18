/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_3D_MODEL_TYPE_H
#define I_3D_MODEL_TYPE_H

#include <IsoRealms/Resources/IResource.h>
#include <IsoRealms/Resources/Vertex/Vertex.h>

#include "I3DModel.h"

class I3DModelType:public IResource {
  public:

  /**
   * Returns an model instance.  Clients should not assume that the instance
   * will be unique; in cases where a model instance is sufficiently simple to
   * be reused, the same instance may be used across all usages.
   * 
   * @returns A new model instance.
   */
  virtual I3DModel* createModel(Vertex*, float = 1.0f) = 0;

  /**
   * When a model is no longer required by an entity (e.g. it was changed to a
   * different model), this method should be called with the instance that was
   * used by the entity to ensure that any necessary clean-up action is
   * performed on the model instance.
   * 
   * @param ISimpleModel*  The model to destroy.
   */
  virtual void destroyModel(I3DModel*) = 0;

  /**
   * Render an iconic representation of the model to be created by this
   * type.  This representation should fit inside -1 to +1 on each
   * axis.
   */
  virtual void renderIcon() = 0;

  /**
   * Update the iconic representation of the model to be created by this
   * type.
   * 
   * @param int  Time passed in milliseconds since last update.
   */
  virtual void updateIcon(unsigned int) = 0;

  /**
   * The destructor cleans up resources used by the model type.  It should
   * be assumed that destroyModel() has been called for all instances created
   * by this type.  Hence, instance clean-up is not necessary here.
   */
  virtual ~I3DModelType() {}
};

#endif

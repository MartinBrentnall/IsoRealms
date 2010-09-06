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
#ifndef I_SIMPLE_MODEL_FACTORY_H
#define I_SIMPLE_MODEL_FACTORY_H

#include <IsoRealms/Vertex.h>

#include "ISimpleModel.h"

class ISimpleModelFactory:public virtual IPlugin {
  public:

  /**
   * Returns an model instance.  Clients should not assume that the instance
   * will be unique; in cases where a model instance is sufficiently simple to
   * be reused, the same instance may be used across all usages.
   * 
   * @returns A new model instance.
   */
  virtual ISimpleModel* createModel(Vertex*, float = 1.0f) = 0;

  /**
   * When a model is no longer required by an entity (e.g. it was changed to a
   * different model), this method should be called with the instance that was
   * used by the entity to ensure that any necessary clean-up action is
   * performed on the model instance.
   * 
   * @param ISimpleModel*  The model to destroy.
   */
  virtual void destroyModel(ISimpleModel*) = 0;

  /**
   * The destructor cleans up resources used by the model factory.  It should
   * be assumed that destroyModel() has been called for all instances created
   * by this factory.  Hence, instance clean-up is not necessary here.
   */
  virtual ~ISimpleModelFactory() {}
};

#endif
